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
#include "Mob.h"

#include "../../Util/Misc.h"

#include "../../Net/Packets/GameplayPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms {
    Mob::Mob(int32_t oi, int32_t mid, int8_t mode, int8_t st, uint16_t fh, bool newspawn, int8_t tm,
             Point<int16_t> position) : MapObject(oi) {
        nl::node src = NxHelper::Mob::get_mob_node(mid);

        nl::node info = src["info"];

        level = info["level"];
        watk = info["PADamage"];
        matk = info["MADamage"];
        wdef = info["PDDamage"];
        mdef = info["MDDamage"];
        accuracy = info["acc"];
        avoid = info["eva"];
        knockback = info["pushed"];
        speed = info["speed"];
        flyspeed = info["flySpeed"];
        touchdamage = info["bodyAttack"].get_bool();
        undead = info["undead"].get_bool();
        noflip = info["noFlip"].get_bool();
        notattack = info["notAttack"].get_bool();
        canjump = src["jump"].size() > 0;
        canfly = src["fly"].size() > 0;
        canmove = src["move"].size() > 0 || canfly;

        std::string linkid = info["link"];
        nl::node link_src;
        nl::node link = src;
        if (linkid.size() > 1) {
            link_src = NxHelper::Mob::get_mob_node(std::stoi(linkid));
            link = link_src;
        }
        nl::node fly = link["fly"];

        if (canfly) {
            animations[STAND] = fly;
            animations[MOVE] = fly;
        } else {
            animations[STAND] = link["stand"];
            animations[MOVE] = link["move"];
        }

        animations[JUMP] = link["jump"];
        animations[HIT] = link["hit1"];
        animations[DIE] = link["die1"];

        name = nl::nx::String["Mob.img"][std::to_string(mid)]["name"];

//        nl::node sndsrc = nl::nx::Sound["Mob.img"][strid];
        nl::node sndsrc = nl::nx::Sound["Mob.img"][src.name()];

        hitsound = sndsrc["Damage"];
        diesound = sndsrc["Die"];

        speed += 100;
        speed *= 0.001f;

        flyspeed += 100;
        flyspeed *= 0.0005f;

        if (canfly)
            physics_object.type = PhysicsObject::Type::FLYING;

        id = mid;
        team = tm;
        set_position(position);
        set_control(mode);
        physics_object.fh_id = fh;
        physics_object.set_flag(PhysicsObject::Flag::TURN_AT_EDGES);

        hppercent = 0;
        dying = false;
        dead = false;
        fading = false;
        set_stance(st);
        flydirection = STRAIGHT;
        counter = 0;

        namelabel = Text(Text::Font::A13M, Text::Alignment::CENTER, Color::Name::WHITE, Text::Background::NAMETAG,
                         name);

        if (newspawn) {
            fadein = true;
            opacity.set(0.0f);
        } else {
            fadein = false;
            opacity.set(1.0f);
        }

        if (control && stance == STAND)
            next_move();
    }

    void Mob::set_stance(uint8_t stancebyte) {
        flip = (stancebyte % 2) == 0;

        if (!flip)
            stancebyte -= 1;

        if (stancebyte < MOVE)
            stancebyte = MOVE;

        set_stance(static_cast<Stance>(stancebyte));
    }

    void Mob::set_stance(Stance newstance) {
        if (stance != newstance) {
            stance = newstance;

            animations.at(stance).reset();
        }
    }

    int8_t Mob::update(const Physics& physics) {
        if (!active)
            return physics_object.fh_layer;

        bool aniend = animations.at(stance).update();

        if (aniend && stance == DIE)
            dead = true;

        if (fading) {
            opacity -= 0.025f;

            if (opacity.last() < 0.025f) {
                opacity.set(0.0f);
                fading = false;
                dead = true;
            }
        } else if (fadein) {
            opacity += 0.025f;

            if (opacity.last() > 0.975f) {
                opacity.set(1.0f);
                fadein = false;
            }
        }

        if (dead) {
            deactivate();

            return -1;
        }

        effects.update();
        showhp.update();

        if (!dying) {
            if (!canfly) {
                if (physics_object.is_flag_not_set(PhysicsObject::Flag::TURN_AT_EDGES)) {
                    flip = !flip;
                    physics_object.set_flag(PhysicsObject::Flag::TURN_AT_EDGES);

                    if (stance == HIT)
                        set_stance(STAND);
                }
            }

            switch (stance) {
            case MOVE:
                if (canfly) {
                    physics_object.h_force = flip ? flyspeed : -flyspeed;

                    switch (flydirection) {
                    case UPWARDS:
                        physics_object.v_force = -flyspeed;
                        break;
                    case DOWNWARDS:
                        physics_object.v_force = flyspeed;
                        break;
                    }
                } else {
                    physics_object.h_force = flip ? speed : -speed;
                }

                break;
            case HIT:
                if (canmove) {
                    double KBFORCE = physics_object.is_on_ground ? 0.2 : 0.1;
                    physics_object.h_force = flip ? -KBFORCE : KBFORCE;
                }

                break;
            case JUMP:
                physics_object.v_force = -5.0;
                break;
            }

            physics.move_object(physics_object);

            if (control) {
                counter++;

                bool next;

                switch (stance) {
                case HIT:
                    next = counter > 200;
                    break;
                case JUMP:
                    next = physics_object.is_on_ground;
                    break;
                default:
                    next = aniend && counter > 200;
                    break;
                }

                if (next) {
                    next_move();
                    update_movement();
                    counter = 0;
                }
            }
        } else {
            physics_object.normalize();
            physics.get_fht().update_fh(physics_object);
        }

        return physics_object.fh_layer;
    }

    void Mob::next_move() {
        if (canmove) {
            switch (stance) {
            case HIT:
            case STAND:
                set_stance(MOVE);
                flip = randomizer.next_bool();
                break;
            case MOVE:
            case JUMP:
                if (canjump && physics_object.is_on_ground && randomizer.below(0.25f)) {
                    set_stance(JUMP);
                } else {
                    switch (randomizer.next_int(3)) {
                    case 0:
                        set_stance(STAND);
                        break;
                    case 1:
                        set_stance(MOVE);
                        flip = false;
                        break;
                    case 2:
                        set_stance(MOVE);
                        flip = true;
                        break;
                    }
                }

                break;
            }

            if (stance == MOVE && canfly)
                flydirection = randomizer.next_enum(NUM_DIRECTIONS);
        } else {
            set_stance(STAND);
        }
    }

    void Mob::update_movement() {
        MoveMobPacket(
                object_id, 1, 0, 0, 0, 0, 0, 0,
                get_position(),
                Movement(physics_object, value_of(stance, flip))
        ).dispatch();
    }

    void Mob::draw(double viewx, double viewy, float alpha) const {
        Point<int16_t> absp = physics_object.get_absolute(viewx, viewy, alpha);
        Point<int16_t> headpos = get_head_position(absp);

        effects.draw_below(absp, alpha);

        if (!dead) {
            float interopc = opacity.get(alpha);

            animations.at(stance).draw(DrawArgument(absp, flip && !noflip, interopc), alpha);

            if (showhp) {
                namelabel.draw(absp);

                if (!dying && hppercent > 0)
                    hpbar.draw(headpos, hppercent);
            }
        }

        effects.drawabove(absp, alpha);
    }

    void Mob::set_control(int8_t mode) {
        control = mode > 0;
        aggro = mode == 2;
    }

    void Mob::send_movement(Point<int16_t> start, std::vector<Movement>&& in_movements) {
        if (control)
            return;

        set_position(start);

        movements = std::forward<decltype(in_movements)>(in_movements);

        if (movements.empty())
            return;

        const Movement& lastmove = movements.front();

        uint8_t laststance = lastmove.newstate;
        set_stance(laststance);

        physics_object.fh_id = lastmove.fh;
    }

    Point<int16_t> Mob::get_head_position(Point<int16_t> position) const {
        Point<int16_t> head = animations.at(stance).get_head();

        position.shift_x((flip && !noflip) ? -head.x() : head.x());
        position.shift_y(head.y());

        return position;
    }

    void Mob::kill(int8_t animation) {
        if (!active) return;

        switch (animation) {
        case 0:
            deactivate();
            break;
        case 1:
            dying = true;
            apply_death();
            break;
        case 2:
            fading = true;
            dying = true;
            break;
        }
    }

    void Mob::show_hp(int8_t percent, uint16_t playerlevel) {
        if (hppercent == 0) {
            int16_t delta = playerlevel - level;

            if (delta > 9)
                namelabel.change_color(Color::Name::YELLOW);
            else if (delta < -9)
                namelabel.change_color(Color::Name::RED);
        }

        if (percent > 100)
            percent = 100;
        else if (percent < 0)
            percent = 0;

        hppercent = percent;
        showhp.set_for(2000);
    }

    void Mob::show_effect(const Animation& animation, int8_t pos, int8_t z, bool f) {
        if (!active)
            return;

        Point<int16_t> shift;

        switch (pos) {
        case 0:
            shift = get_head_position(Point<int16_t>());
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        }

        effects.add(animation, DrawArgument(shift, f), z);
    }

    float Mob::calculate_hitchance(int16_t leveldelta, int32_t player_accuracy) const {
        float faccuracy = static_cast<float>(player_accuracy);
        float hitchance = faccuracy / (((1.84f + 0.07f * leveldelta) * avoid) + 1.0f);

        if (hitchance < 0.01f)
            hitchance = 0.01f;

        return hitchance;
    }

    double Mob::calculate_mindamage(int16_t leveldelta, double damage, bool magic) const {
        double mindamage =
            magic ? damage - (1 + 0.01 * leveldelta) * mdef * 0.6 : damage * (1 - 0.01 * leveldelta) - wdef * 0.6;

        return mindamage < 1.0 ? 1.0 : mindamage;
    }

    double Mob::calculate_maxdamage(int16_t leveldelta, double damage, bool magic) const {
        double maxdamage =
            magic ? damage - (1 + 0.01 * leveldelta) * mdef * 0.5 : damage * (1 - 0.01 * leveldelta) - wdef * 0.5;

        return maxdamage < 1.0 ? 1.0 : maxdamage;
    }

    std::vector<std::pair<int32_t, bool>> Mob::calculate_damage(const Attack& attack) {
        double mindamage;
        double maxdamage;
        float hitchance;
        float critical;
        int16_t leveldelta = level - attack.playerlevel;

        if (leveldelta < 0)
            leveldelta = 0;

        Attack::DamageType damagetype = attack.damagetype;

        switch (damagetype) {
        case Attack::DamageType::DMG_WEAPON:
        case Attack::DamageType::DMG_MAGIC:
            mindamage = calculate_mindamage(leveldelta, attack.mindamage, damagetype == Attack::DamageType::DMG_MAGIC);
            maxdamage = calculate_maxdamage(leveldelta, attack.maxdamage, damagetype == Attack::DamageType::DMG_MAGIC);
            hitchance = calculate_hitchance(leveldelta, attack.accuracy);
            critical = attack.critical;
            break;
        case Attack::DamageType::DMG_FIXED:
            mindamage = attack.fixdamage;
            maxdamage = attack.fixdamage;
            hitchance = 1.0f;
            critical = 0.0f;
            break;
        }

        std::vector<std::pair<int32_t, bool>> result(attack.hitcount);

        std::generate(
            result.begin(), result.end(),
            [&]() {
                return next_damage(mindamage, maxdamage, hitchance, critical);
            }
        );

        update_movement();

        return result;
    }

    std::pair<int32_t, bool>
    Mob::next_damage(double mindamage, double maxdamage, float hitchance, float critical) const {
        bool hit = randomizer.below(hitchance);

        if (!hit)
            return std::pair<int32_t, bool>(0, false);

        constexpr double DAMAGECAP = 999999.0;

        double damage = randomizer.next_real(mindamage, maxdamage);
        bool iscritical = randomizer.below(critical);

        if (iscritical)
            damage *= 1.5;

        if (damage < 1)
            damage = 1;
        else if (damage > DAMAGECAP)
            damage = DAMAGECAP;

        auto intdamage = static_cast<int32_t>(damage);

        return std::pair<int32_t, bool>(intdamage, iscritical);
    }

    void Mob::apply_damage(int32_t damage, bool toleft) {
        hitsound.play();

        if (dying && stance != DIE) {
            apply_death();
        } else if (control && is_alive() && damage >= knockback) {
            flip = toleft;
            counter = 170;
            set_stance(HIT);

            update_movement();
        }
    }

    MobAttack Mob::create_touch_attack() const {
        if (!touchdamage)
            return MobAttack();

        int32_t minattack = static_cast<int32_t>(watk * 0.8f);
        int32_t maxattack = watk;
        int32_t attack = randomizer.next_int(minattack, maxattack);

        return MobAttack(attack, get_position(), id, object_id);
    }

    void Mob::apply_death() {
        set_stance(DIE);
        diesound.play();
        dying = true;
    }

    bool Mob::is_alive() const {
        return active && !dying;
    }

    bool Mob::is_in_range(const Rectangle<int16_t>& range) const {
        if (!active)
            return false;

        Rectangle<int16_t> bounds = animations.at(stance).get_bounds();
        bounds.shift(get_position());

        return range.overlaps(bounds);
    }

    Point<int16_t> Mob::get_head_position() const {
        Point<int16_t> position = get_position();

        return get_head_position(position);
    }
}
