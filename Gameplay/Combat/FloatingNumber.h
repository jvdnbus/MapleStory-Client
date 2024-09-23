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

#include "../Physics/PhysicsObject.h"

#include "../../Template/BoolPair.h"

#include "../../IO/Components/Charset.h"

namespace ms {
    class FloatingNumber {
    public:
        static constexpr size_t NUM_TYPES = 4;

        enum Type {
            NORMAL,
            CRITICAL,
            TO_PLAYER,
            HEAL
        };

        FloatingNumber(Type type, int32_t damage, int16_t starty, int16_t x = 0);
        FloatingNumber();

        void draw(double viewx, double viewy, float alpha) const;
        void set_x(int16_t headx);
        bool update();

        static int16_t row_height(bool critical);
        static void init();

    private:
        int16_t get_advance(char c, bool first) const;

        static constexpr uint16_t FADE_TIME = 500;

        Type type;
        bool miss;
        bool multiple;
        int8_t first_num;
        std::string rest_num;
        int16_t shift;
        MovingObject moving_object;
        Linear<float> opacity;

        static BoolPair<Charset> charsets[NUM_TYPES];
    };
}
