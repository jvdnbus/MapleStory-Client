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
#include "MovementParser.h"

namespace ms {
    std::unique_ptr<MovementPath> MovementParser::parse_movements(InPacket& recv) {
        std::unique_ptr<MovementPath> path = std::make_unique<MovementPath>();
        uint8_t length = recv.read_byte();

        for (uint8_t i = 0; i < length; ++i) {
            MovementSnapshot fragment;
            fragment.cmd = MovementSnapshot::by_value(recv.read_byte());
            fragment.type = MovementSnapshot::get_type(fragment.cmd);

            switch (fragment.type) {
                case MovementSnapshot::ABSOLUTE:
                case MovementSnapshot::JUMP_DOWN:
                    fragment.position_x = recv.read_short();
                    fragment.position_y = recv.read_short();
                    fragment.velocity_x = recv.read_short();
                    fragment.velocity_y = recv.read_short();
                    fragment.foothold = recv.read_short();
                    break;
                case MovementSnapshot::RELATIVE:
                    fragment.position_x = recv.read_short();
                    fragment.position_y = recv.read_short();
                    break;
                case MovementSnapshot::TELEPORT:
                    fragment.position_x = recv.read_short();
                    fragment.position_y = recv.read_short();
                    fragment.foothold = recv.read_short();
                    break;
                case MovementSnapshot::CHANGE_EQUIP:
                    // TODO equip slot?
                    recv.read_byte();
                    break;
                case MovementSnapshot::FALL_DOWN:
                    fragment.velocity_x = recv.read_short();
                    fragment.velocity_y = recv.read_short();
                    fragment.foothold_fall_start = recv.read_short();
                    break;
                case MovementSnapshot::ARAN:
                default:
                    break;
            }

            if (fragment.type != MovementSnapshot::CHANGE_EQUIP) {
                fragment.state = recv.read_byte();
                fragment.duration = recv.read_short();
            }

            path->add_snapshot(fragment);
        }

        return path;
    }
}
