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
#include "EquipStat.h"

namespace ms {
    namespace EquipStat {
        Id by_id(size_t id) {
            return static_cast<Id>(id);
        }

        int32_t value_of(Id value) {
            return value;
        }

        const char* names[LENGTH] =
        {
            "STR",
            "DEX",
            "INT",
            "LUK",
            "MaxHP",
            "MaxMP",
            "Attack Power",
            "Magic Attack",
            "Defense",

            // TODO: Does current GMS use these anymore?
            "MAGIC DEFENSE",
            "ACCURACY",
            "AVOID",
            "HANDS",

            "Speed",
            "Jump"
        };
    }
}
