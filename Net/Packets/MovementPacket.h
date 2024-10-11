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
#include "../Gameplay/MovementSnapshot.h"

namespace ms {
    // Base class for packets which update object movements with the server
    class MovementPacket : public OutPacket {
    public:
        MovementPacket(Opcode opc) : OutPacket(opc) {
        }

    public:
        void write_movement(const MovementSnapshot& movement) {
            write_byte(movement.cmd);
            switch (movement.type) {
                case MovementSnapshot::ABSOLUTE:
                case MovementSnapshot::JUMP_DOWN:
                    write_short(movement.position_x);
                    write_short(movement.position_y);
                    write_short(movement.velocity_x);
                    write_short(movement.velocity_y);
                    write_short(movement.foothold);
                    if (movement.type == MovementSnapshot::JUMP_DOWN) {
                        write_short(movement.foothold_fall_start);
                    }
                    break;
                case MovementSnapshot::RELATIVE:
                    // offset position
                    write_short(movement.position_x);
                    write_short(movement.position_y);
                    break;
                case MovementSnapshot::TELEPORT:
                    write_short(movement.position_x);
                    write_short(movement.position_y);
                    write_short(movement.foothold);
                    break;
                case MovementSnapshot::CHANGE_EQUIP:
                    write_byte(0); // equip slot?
                    break;
                case MovementSnapshot::FALL_DOWN:
                    write_short(movement.velocity_x);
                    write_short(movement.velocity_y);
                    write_short(movement.foothold_fall_start);
                    break;
                case MovementSnapshot::ARAN:
                default:
                    break;
            }

            if (movement.type != MovementSnapshot::CHANGE_EQUIP) {
                write_byte(movement.state);
                write_short(movement.duration);
            }
        }

        void write_movement(const std::vector<MovementSnapshot>& path) {
            write_byte(path.size());
            for (const auto& movement : path) {
                write_movement(movement);
            }
        }
    };
}
