//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "Player.h"

#include "PlayerStates.h"
#include "SkillId.h"

#include "../Data/WeaponData.h"
#include "../IO/UI.h"
#include "../IO/UITypes/UIStatsInfo.h"
#include "../Gameplay/Stage.h"
#include "../Net/Packets/GameplayPackets.h"
#include "../Net/Packets/InventoryPackets.h"
#include "../Net/Packets/AttackAndSkillPackets.h"
#include "../Net/Packets/PlayerPackets.h"

namespace ms {
    const PlayerNullState nullstate;

    const PlayerState* get_player_state(Char::State state) {
        static PlayerStandState standing;
        static PlayerWalkState walking;
        static PlayerFallState falling;
        static PlayerProneState lying;
        static PlayerClimbState climbing;
        static PlayerSitState sitting;
        static PlayerFlyState flying;

        switch (state) {
        case Char::State::STAND:
            return &standing;
        case Char::State::WALK:
            return &walking;
        case Char::State::FALL:
            return &falling;
        case Char::State::PRONE:
            return &lying;
        case Char::State::LADDER:
        case Char::State::ROPE:
            return &climbing;
        case Char::State::SIT:
            return &sitting;
        case Char::State::SWIM:
            return &flying;
        default:
            return nullptr;
        }
    }

    constexpr int64_t HP_RECOVERY_INTERVAL = 10000;
    constexpr int64_t MP_RECOVERY_INTERVAL = 10000;
    constexpr int64_t HP_RECOVERY_LADDER_INTERVAL = 31000;

    Player::Player(const CharEntry& entry) : Char(entry.id, entry.look, entry.stats.name), stats(entry.stats) {
        attacking = false;
        underwater = false;

        auto &tm = TimerManager::get();
        hp_recovery_timer = tm.create_timer(HP_RECOVERY_INTERVAL).start();
        mp_recovery_timer = tm.create_timer(MP_RECOVERY_INTERVAL).start();
        // Only start this if Endure (WARRIOR) skill is present
        hp_recovery_ladder_timer = tm.create_timer(HP_RECOVERY_LADDER_INTERVAL);

        set_state(State::FALL);
        set_direction(true);
    }

    Player::Player() : Char(0, {}, "") {
    }

    void Player::respawn(Point<int16_t> pos, bool uw) {
        set_position(pos.x(), pos.y());
        set_portal_cooldown();
        underwater = uw;
        attacking = false;
        ladder = nullptr;
        keysdown.clear();
        physics_object.reset_movement();
        set_state(State::FALL);
//        nullstate.update_state(*this);
    }

    void Player::teleport(Point<int16_t> pos, bool uw) {
        set_position(pos.x(), pos.y());
        set_portal_cooldown();
        underwater = uw;
        attacking = false;
        ladder = nullptr;
    }

    void Player::send_action(KeyAction::Id action, bool down) {
        const PlayerState* pst = get_player_state(state);

        if (pst)
            pst->send_action(*this, action, down);

        keysdown[action] = down;
    }

    void Player::recalc_stats(bool equipchanged) {
        Weapon::Type weapontype = get_weapon_type();

        stats.set_weapontype(weapontype);
        stats.init_totalstats();

        if (equipchanged)
            inventory.recalc_stats(weapontype);

        for (auto stat : EquipStat::values) {
            int32_t inventory_total = inventory.get_stat(stat);
            stats.add_value(stat, inventory_total);
        }

        auto passive_skills = skillbook.collect_passives();

        for (auto& passive : passive_skills) {
            int32_t skill_id = passive.first;
            int32_t skill_level = passive.second;

            passive_buffs.apply_buff(stats, skill_id, skill_level);
        }

        for (const Buff& buff : buffs.values())
            active_buffs.apply_buff(stats, buff.stat, buff.value);

        stats.close_totalstats();

        if (auto statsinfo = UI::get().get_element<UIStatsInfo>())
            statsinfo->update_all_stats();

        int32_t endure_level = get_skill_level(SkillId::ENDURE);
        if (endure_level > 0) {
            hp_recovery_ladder_timer.set_duration(HP_RECOVERY_LADDER_INTERVAL - endure_level * 3 * 1000);
            if (!hp_recovery_ladder_timer.is_active()) {
                hp_recovery_ladder_timer.start();
            }
        }
    }

    void Player::change_equip(int16_t slot) {
        if (int32_t itemid = inventory.get_item_id(InventoryType::Id::EQUIPPED, slot))
            look.add_equip(itemid);
        else
            look.remove_equip(EquipSlot::by_id(slot));
    }

    void Player::use_item(int32_t itemid) {
        InventoryType::Id type = InventoryType::by_item_id(itemid);

        if (int16_t slot = inventory.find_item(type, itemid))
            if (type == InventoryType::Id::USE)
                UseItemPacket(slot, itemid).dispatch();
    }

    void Player::draw(Layer::Id layer, double viewx, double viewy, float alpha) const {
        if (layer == get_layer())
            Char::draw(viewx, viewy, alpha);
    }

    int8_t Player::update(const Physics& physics) {
        const PlayerState* pst = get_player_state(state);

        if (pst) {
            pst->update(*this);
            physics.move_object(physics_object);

            bool ani_end = Char::update(physics, get_stance_speed());

            if (ani_end && attacking) {
                attacking = false;
                nullstate.update_state(*this);
            } else {
                pst->update_state(*this);
            }
        }

        uint8_t stancebyte = facing_right ? state : state + 1;
        Movement newmove(physics_object, stancebyte);
        bool needupdate = lastmove.has_moved(newmove);

        if (needupdate) {
            hp_recovery_ladder_timer.reset();
            hp_recovery_timer.reset();

            MovePlayerPacket(newmove).dispatch();
            lastmove = newmove;
        }

        climb_cooldown.update();
        portal_cooldown.update();
        try_hp_recovery();
        try_mp_recovery();

        return get_layer();
    }

    int8_t Player::get_integer_attackspeed() const {
        int32_t weapon_id = look.get_equips().get_weapon();

        if (weapon_id <= 0)
            return 0;

        const WeaponData& weapon = WeaponData::get(weapon_id);

        int8_t base_speed = stats.get_attackspeed();
        int8_t weapon_speed = weapon.get_speed();

        return base_speed + weapon_speed;
    }

    void Player::set_direction(bool flipped) {
        if (!attacking)
            Char::set_direction(flipped);
    }

    void Player::set_state(State st) {
        if (!attacking) {
            Char::set_state(st);

            const PlayerState* pst = get_player_state(st);

            if (pst)
                pst->initialize(*this);
        }
    }

    bool Player::is_attacking() const {
        return attacking;
    }

    bool Player::can_attack() const {
        return !attacking && !is_climbing() && !is_sitting() && look.get_equips().has_weapon();
    }

    SpecialMove::ForbidReason Player::can_use(const SpecialMove& move) const {
        if (move.is_skill() && state == PRONE)
            return SpecialMove::ForbidReason::FBR_OTHER;

        if (move.is_attack() && (state == LADDER || state == ROPE))
            return SpecialMove::ForbidReason::FBR_OTHER;

        if (has_cooldown(move.get_id()))
            return SpecialMove::ForbidReason::FBR_COOLDOWN;

        int32_t level = skillbook.get_level(move.get_id());
        Weapon::Type weapon = get_weapon_type();
        const Job& job = stats.get_job();
        uint16_t hp = stats.get_stat(MapleStat::Id::HP);
        uint16_t mp = stats.get_stat(MapleStat::Id::MP);
        uint16_t bullets = inventory.get_bulletcount();

        return move.can_use(level, weapon, job, hp, mp, bullets);
    }

    Attack Player::prepare_attack(bool skill) const {
        Attack::Type attacktype;
        bool degenerate;

        if (state == PRONE) {
            degenerate = true;
            attacktype = Attack::Type::CLOSE;
        } else {
            Weapon::Type weapontype;
            weapontype = get_weapon_type();

            switch (weapontype) {
            case Weapon::Type::BOW:
            case Weapon::Type::CROSSBOW:
            case Weapon::Type::CLAW:
            case Weapon::Type::GUN: {
                degenerate = !inventory.has_projectile();
                attacktype = degenerate ? Attack::Type::CLOSE : Attack::Type::RANGED;
                break;
            }
            case Weapon::Type::WAND:
            case Weapon::Type::STAFF: {
                degenerate = !skill;
                attacktype = degenerate ? Attack::Type::CLOSE : Attack::Type::MAGIC;
                break;
            }
            default: {
                attacktype = Attack::Type::CLOSE;
                degenerate = false;
                break;
            }
            }
        }

        Attack attack;
        attack.type = attacktype;
        attack.mindamage = stats.get_mindamage();
        attack.maxdamage = stats.get_maxdamage();

        if (degenerate) {
            attack.mindamage /= 10;
            attack.maxdamage /= 10;
        }

        attack.critical = stats.get_critical();
        attack.ignoredef = stats.get_ignoredef();
        attack.accuracy = stats.get_total(EquipStat::Id::ACC);
        attack.playerlevel = stats.get_stat(MapleStat::Id::LEVEL);
        attack.range = stats.get_range();
        attack.bullet = inventory.get_bulletid();
        attack.origin = get_position();
        attack.toleft = !facing_right;
        attack.speed = get_integer_attackspeed();

        return attack;
    }

    void Player::rush(double targetx) {
        if (physics_object.is_on_ground) {
            uint16_t delay = get_attackdelay(1);
            physics_object.move_x_until(targetx, delay);
            physics_object.set_flag(PhysicsObject::Flag::TURN_AT_EDGES);
        }
    }

    bool Player::is_invincible() const {
        if (state == DIED)
            return true;

        if (has_buff(Buffstat::Id::DARKSIGHT))
            return true;

        return Char::is_invincible();
    }

    void Player::try_hp_recovery() {
        if (/*is_dead() || */is_full_health()) {
            return;
        }

        bool ladder_recovery = is_climbing() && hp_recovery_ladder_timer.is_ready();
        if (ladder_recovery) {
            hp_recovery_ladder_timer.reset();
        }

        bool idle_recovery = !is_climbing() && hp_recovery_timer.is_ready();
        if (idle_recovery) {
            hp_recovery_timer.reset();
        }

        if (ladder_recovery || idle_recovery) {
            int16_t hp = 10;

            // Check if sitting
            if (is_sitting()) {
                // TODO Check if on portable chair and check their HP/MP stats
            } else {
                // Calculate HP recovery based on learned skills
                // Swordman skill
                int32_t hp_recovery_level = get_skill_level(SkillId::IMPROVED_HP_RECOVERY);
                if (hp_recovery_level > 0) {
                    hp += hp_recovery_level < 16 ? 3 * hp_recovery_level : 50;
                }
            }

            // Check map recovery rate (e.g. sauna)
            hp *= Stage::get().get_map_info().get_recovery();

            // Send request packet
            HpMpChangeRequestPacket(hp, 0).dispatch();
            // Show heal
            show_heal(hp);
        }
    }

    void Player::try_mp_recovery() {
        if (is_full_mana()) {
            return;
        }

        if (mp_recovery_timer.is_ready()) {
            int16_t mp = 3;

            // Magician skill
            int32_t mp_recovery_level = get_skill_level(SkillId::IMPROVED_MP_RECOVERY);
            if (mp_recovery_level > 0) {
                mp += (mp_recovery_level - 1) + std::ceil((get_level() - 9) / 2.0);
            }

            // Crusader skill
            mp_recovery_level = get_skill_level(SkillId::IMPROVED_MP_RECOVERY_CRUSADER);
            if (mp_recovery_level > 0 && mp_recovery_level <= 10) {
                mp += mp_recovery_level * 2;
            } else if (mp_recovery_level > 10) {
                mp += mp_recovery_level + 10;
            }

            if (mp > 1000) mp = 1000;

            // Send request packet
            HpMpChangeRequestPacket(0, mp).dispatch();

            mp_recovery_timer.reset();
        }
    }

    MobAttackResult Player::damage(const MobAttack& attack) {
        int32_t damage = stats.calculate_damage(attack.watk);
        show_damage(damage);

        bool fromleft = attack.origin.x() > physics_object.get_x();
        bool missed = damage <= 0;
        bool immovable = ladder || state == DIED;
        bool knockback = !missed && !immovable;

        if (knockback && randomizer.above(stats.get_stance())) {
            physics_object.h_speed = fromleft ? -1.75 : 1.75;
            double v_speed = physics_object.v_speed + physics_object.v_force;
            double min_v_speed = -1.8;
            if (v_speed > min_v_speed) {
                physics_object.v_force += min_v_speed - v_speed;
            }
        }

        uint8_t direction = fromleft ? 0 : 1;

        return {attack, damage, direction};
    }

    void Player::fall_damage(double fell_from_height) {
        int32_t damage = 0.035 * (fell_from_height - get_fall_damage_min_height()) + 7;
        show_damage(damage);
        TakeDamagePacket(damage).dispatch();

        physics_object.h_speed = is_facing_right() ? 2.5 : -2.5;
        physics_object.v_force -= 2.0;
    }

    int16_t Player::get_fall_damage_min_height() const {
        return 600;
    };

    void Player::give_buff(Buff buff) {
        buffs[buff.stat] = buff;
    }

    void Player::cancel_buff(Buffstat::Id stat) {
        buffs[stat] = {};
    }

    bool Player::has_buff(Buffstat::Id stat) const {
        return buffs[stat].value > 0;
    }

    void Player::change_skill(int32_t skill_id, int32_t skill_level, int32_t masterlevel, int64_t expiration) {
        int32_t old_level = skillbook.get_level(skill_id);
        skillbook.set_skill(skill_id, skill_level, masterlevel, expiration);

        if (old_level != skill_level)
            recalc_stats(false);
    }

    void Player::add_cooldown(int32_t skill_id, int32_t cooltime) {
        cooldowns[skill_id] = cooltime;
    }

    bool Player::has_cooldown(int32_t skill_id) const {
        auto iter = cooldowns.find(skill_id);

        if (iter == cooldowns.end())
            return false;

        return iter->second > 0;
    }

    void Player::change_level(uint16_t level) {
        uint16_t oldlevel = get_level();

        if (level > oldlevel)
            show_effect_id(CharEffect::Id::LEVELUP);

        stats.set_stat(MapleStat::Id::LEVEL, level);
    }

    uint16_t Player::get_level() const {
        return stats.get_stat(MapleStat::Id::LEVEL);
    }

    int32_t Player::get_skill_level(int32_t skillid) const {
        return skillbook.get_level(skillid);
    }

    void Player::change_job(uint16_t jobid) {
        show_effect_id(CharEffect::Id::JOBCHANGE);
        stats.change_job(jobid);
    }

    void Player::set_seat(Optional<const Seat> seat) {
        if (seat) {
            set_position(seat->getpos());
            set_state(SIT);
        }
    }

    void Player::set_ladder(Optional<const Ladder> ldr) {
        ladder = ldr;

        if (ladder) {
            physics_object.set_x(ldr->get_x());

            physics_object.h_speed = 0.0;
            physics_object.v_speed = 0.0;
            physics_object.fh_layer = 7;

            set_state(ldr->is_ladder() ? LADDER : ROPE);
        }
    }

    void Player::jump_down(bool cancel_h_speed) {
        auto &phobj = get_physics_object();
        phobj.v_force -= get_jump_down_force();
        if (cancel_h_speed) phobj.h_speed = 0;
        phobj.y = phobj.ground_below_y;
        phobj.jumping_down_from_fh_id = phobj.fh_id;
        set_state(Char::State::FALL);
    }

    void Player::set_climb_cooldown(int64_t millis) {
        climb_cooldown.set_for(millis);
    }

    bool Player::can_climb() {
        return !climb_cooldown;
    }

    void Player::set_portal_cooldown() {
        portal_cooldown.set_for(1000);
    }

    bool Player::can_portal() {
        return !portal_cooldown;
    }

    float Player::get_stance_speed() const {
        if (!attacking && state == WALK && physics_object.is_flag_set(PhysicsObject::HUGGING_WALL)) {
            return static_cast<float>(stats.get_total(EquipStat::Id::SPEED)) / 100;
        }
        return Char::get_stance_speed();
    }

    float Player::get_walk_force() const {
        // Approximation based on direct comparison testing
        return 0.00431f + 0.19653f * static_cast<float>(stats.get_total(EquipStat::Id::SPEED)) / 100;
    }

    float Player::get_jump_down_force() const {
        return 1.5f;
    }

    float Player::get_jump_force() const {
        return 1.0f + 3.5f * static_cast<float>(stats.get_total(EquipStat::Id::JUMP)) / 100;
    }

    float Player::get_climb_force() const {
        // As far as I can tell, speed/jump do not actually affect climb speed in v83 client
        return 0.8f * static_cast<float>(stats.get_total(EquipStat::Id::SPEED)) / 100;
//        return 0.8f;
    }

    float Player::get_fly_force() const {
        return 0.25f;
    }

    bool Player::is_falling() const {
        return state == State::FALL;
    }

    bool Player::is_underwater() const {
        return underwater;
    }

    bool Player::is_key_down(KeyAction::Id action) const {
        return keysdown.count(action) > 0 && keysdown.at(action);
    }

    CharStats& Player::get_stats() {
        return stats;
    }

    const CharStats& Player::get_stats() const {
        return stats;
    }

    bool Player::is_dead() const {
        return stats.get_stat(MapleStat::HP) == 0;
    }

    bool Player::is_full_health() const {
        return stats.get_stat(MapleStat::HP) == stats.get_stat(MapleStat::MAXHP);
    }

    bool Player::is_full_mana() const {
        return stats.get_stat(MapleStat::MP) == stats.get_stat(MapleStat::MAXMP);
    }

    Inventory& Player::get_inventory() {
        return inventory;
    }

    const Inventory& Player::get_inventory() const {
        return inventory;
    }

    SkillBook& Player::get_skills() {
        return skillbook;
    }

    QuestLog& Player::get_quests() {
        return questlog;
    }

    TeleportRock& Player::get_teleportrock() {
        return teleportrock;
    }

    MonsterBook& Player::get_monsterbook() {
        return monsterbook;
    }

    Optional<const Ladder> Player::get_ladder() const {
        return ladder;
    }
}
