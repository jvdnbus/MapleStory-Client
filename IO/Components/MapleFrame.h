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

#include "../../Graphics/Texture.h"

namespace ms {
    class MapleFrame {
    public:
        MapleFrame() {
        }

        MapleFrame(nl::node src);

        void draw(Point<int16_t> position, int16_t width, int16_t height) const;

    private:
        Texture center;
        Texture east;
        Texture northeast;
        Texture north;
        Texture northwest;
        Texture west;
        Texture southwest;
        Texture south;
        Texture southeast;
        int16_t xtile;
        int16_t ytile;
    };
}
