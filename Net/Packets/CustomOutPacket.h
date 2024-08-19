//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton, Jorn Van denbussche //
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

#include "../../Configuration.h"

namespace ms {
    // Opcode: CUSTOM(0x3713)
    class CustomOutPacket : public OutPacket {
    protected:
        // A custom outgoing packet
        CustomOutPacket() : OutPacket(CUSTOM_PACKET) {
        }
    };

    // Subopcode: 1
    class CustomClientPacket : public CustomOutPacket {
    public:
        // Notify the server this is a custom client.
        // This will make it so the server will:
        // - send the Player Lvl as a `short` instead of `byte`
        // - change the default keybinds
        CustomClientPacket() : CustomOutPacket() {
            write_short(1);
        }
    };
}
