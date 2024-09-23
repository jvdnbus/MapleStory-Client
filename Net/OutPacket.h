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

#include "../Template/Point.h"

#include <vector>

namespace ms {
    // A packet to be sent to the server
    // Used as a base class to create specific packets
    class OutPacket {
    public:
        // Construct a packet by writing its opcode
        OutPacket(int16_t opcode);

        void dispatch();

        // Opcodes for OutPackets associated with version 83 of the game
        enum Opcode : uint16_t {
            /// Login
            LOGIN = 0x0001,
            LOGIN_EMAIL = 0x0003,
            CHARLIST_REQUEST = 0x0005,
            SERVERSTATUS_REQUEST = 0x0006,
            ACCEPT_TOS = 0x0007,
            SET_GENDER = 0x0008,
            SERVERLIST_REQUEST = 0x000B,
            SELECT_CHAR = 0x0013,
            PLAYER_LOGIN = 0x0014,
            NAME_CHAR = 0x0015,
            CREATE_CHAR = 0x0016,
            DELETE_CHAR = 0x0017,
            PONG = 0x0018,
            REGISTER_PIC = 0x001D,
            SELECT_CHAR_PIC = 0x001E,

            /// Gameplay 1
            CHANGEMAP = 0x0026,
            ENTER_CASHSHOP = 0x0028,
            MOVE_PLAYER = 0x0029,
            CLOSE_ATTACK = 0x002C,
            RANGED_ATTACK = 0x002D,
            MAGIC_ATTACK = 0x002E,
            TAKE_DAMAGE = 0x0030,

            /// Messaging
            GENERAL_CHAT = 0x0031,
            MULTI_CHAT = 0x0077,
            SPOUSE_CHAT = 0x0079,

            /// NPC Interaction
            TALK_TO_NPC = 0x003A,
            NPC_TALK_MORE = 0x003C,
            NPC_SHOP_ACTION = 0x003D,

            /// Player Interaction
            CHAR_INFO_REQUEST = 0x0061,

            /// Inventory
            GATHER_ITEMS = 0x0045,
            SORT_ITEMS = 0x0046,
            MOVE_ITEM = 0x0047,
            USE_ITEM = 0x0048,
            SCROLL_EQUIP = 0x0056,

            /// Player
            SPEND_AP = 0x0057,
            HP_MP_CHANGE_REQUEST = 0x0059,
            SPEND_SP = 0x005A,
            CHANGE_KEYMAP = 0x0087,

            /// Skill
            USE_SKILL = 0x005B,

            /// Gameplay 2
            PARTY_OPERATION = 0x007C,
            ADMIN_COMMAND = 0x0080,
            MOVE_MONSTER = 0x00BC,
            PICKUP_ITEM = 0x00CA,
            DAMAGE_REACTOR = 0x00CD,
            PLAYER_MAP_TRANSFER = 0x00CF,
            PLAYER_UPDATE = 0x00DF,

            /// Custom
            CUSTOM_PACKET = 0x3713
        };

    protected:
        // Skip a number of bytes (filled with zeros)
        void skip(size_t count);
        // Write a byte
        void write_byte(int8_t ch);
        // Write a short
        void write_short(int16_t sh);
        // Write an int
        void write_int(int32_t in);
        // Write a long
        void write_long(int64_t lg);

        // Write a point
        // One short for x and one for y
        void write_point(Point<int16_t> point);
        // Write a timestamp as an integer
        void write_time();
        // Write a string
        // Writes the length as a short and then each individual character as a byte
        void write_string(const std::string& str);
        // Write a random int
        void write_random();

        // Write the MACS and then write the HWID of the computer
        void write_hardware_info();
        // Function to convert hexadecimal to decimal
        int32_t hex_to_dec(std::string hexVal);

    private:
        std::vector<int8_t> bytes;
        int16_t opcode;
    };
}
