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

#include "Char.h"

#include "Look/CharLook.h"

#include "../Gameplay/MovementPath.h"

#include <queue>
#include <vector>
#include <memory>

namespace ms {
    // Other client players
    class OtherChar : public Char {
    public:
        OtherChar(int32_t char_id, const CharLook& look, uint16_t level, int16_t job, const std::string& name,
                  int8_t stance, Point<int16_t> position);

        // Update the character
        int8_t update(const Physics& physics) override;
        // Add the movements which this character will go through next
        void send_movement(std::unique_ptr<MovementPath> movements);

        // Update a skill level
        void update_skill(int32_t skill_id, uint8_t skill_level);
        // Update the attack speed
        void update_speed(uint8_t attackspeed);
        // Update the character new_look
        void update_look(const LookEntry& new_look);

        // Return the character's attacking speed
        int8_t get_integer_attackspeed() const override;
        // Return the character's level
        uint16_t get_level() const override;
        // Return the character's level of a skill
        int32_t get_skill_level(int32_t skill_id) const override;

    private:
        uint16_t level;
        int16_t job;
        std::queue<MovementSnapshot> movements;
        MovementSnapshot last_move;
        uint16_t timer;

        std::unordered_map<int32_t, uint8_t> skill_levels;
        uint8_t attackspeed;
    };
}
