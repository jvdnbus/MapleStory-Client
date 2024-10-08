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
#pragma once

#include "../OutPacket.h"

namespace ms {
    // Notifies the server of an attack
    // The opcode is determined by the attack type
    // Attack::CLOSE = CLOSE_ATTACK(44)
    // Attack::RANGED = RANGED_ATTACK(45)
    // Attack::MAGIC = MAGIC_ATTACK(46)
    class AttackPacket : public OutPacket {
    public:
        AttackPacket(const AttackResult& attack) : OutPacket(opcodefor(attack.type)) {
            skip(1);

            write_byte((attack.mobcount << 4) | attack.hitcount);
            write_int(attack.skill);

            if (attack.charge > 0)
                write_int(attack.charge);

            skip(8);

            write_byte(attack.display);
            write_byte(attack.toleft);
            write_byte(attack.stance);

            skip(1);

            write_byte(attack.speed);

            if (attack.type == Attack::Type::RANGED) {
                skip(1);
                write_byte(attack.toleft);
                skip(7);
                // TODO: skip(4); If hurricane, piercing arrow or rapidfire.
            } else {
                skip(4);
            }

            for (auto& damagetomob : attack.damagelines) {
                write_int(damagetomob.first);

                skip(14);

                for (auto& singledamage : damagetomob.second) {
                    write_int(singledamage.first);
                    // TODO: Add critical here
                }

                if (attack.skill != 5221004)
                    skip(4);
            }
        }

    private:
        static Opcode opcodefor(Attack::Type type) {
            switch (type) {
            case Attack::Type::CLOSE:
                return CLOSE_ATTACK;
            case Attack::Type::RANGED:
                return RANGED_ATTACK;
            default:
                return MAGIC_ATTACK;
            }
        }
    };

    // Tells the server that the player took damage
    // Opcode: TAKE_DAMAGE(48)
    class TakeDamagePacket : public OutPacket {
    public:
        enum From : int8_t {
            TOUCH = -1, // by monster
            MAP_OBJECT = -2, // by a map object
            ENVIRONMENT = -3, // e.g. fall damage or a general area mob attack
            MIST = -4 // e.g. poison mist
        };

        enum Element : int8_t {
            NONE = 0,
            ICE = 1,
            FIRE = 2,
            LIGHTNING = 3
        };

        TakeDamagePacket(int8_t from, Element element, int32_t damage, int32_t mobid, int32_t oid,
                         uint8_t direction) : OutPacket(TAKE_DAMAGE) {
            write_time();
            write_byte(from);
            write_byte(element);
            write_int(damage);
            if (from == -1 || from == -2) {
                write_int(mobid);
                write_int(oid);
                write_byte(direction);
            } else {
                write_byte(0);
            }
        }

        // From mob attack result
        TakeDamagePacket(const MobAttackResult& result, From from) : TakeDamagePacket(
            from, Element::NONE, result.damage, result.mobid, result.oid, result.direction) {
        }

        explicit TakeDamagePacket(int32_t fall_damage) : TakeDamagePacket(
            From::ENVIRONMENT, Element::NONE, fall_damage, 0, 0, 0) {
        }
    };

    // Packet which notifies the server of a skill usage
    // Opcode: USE_SKILL(91)
    class UseSkillPacket : public OutPacket {
    public:
        UseSkillPacket(int32_t skillid, int32_t level) : OutPacket(USE_SKILL) {
            write_time();
            write_int(skillid);
            write_byte(static_cast<uint8_t>(level));

            // If monster magnet : some more bytes

            if (skillid % 10000000 == 1004)
                skip(2); // TODO: No idea what this could be

            // TODO: A point (4 bytes) could be added at the end
        }
    };
}
