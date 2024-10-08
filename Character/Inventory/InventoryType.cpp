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
#include "InventoryType.h"

#include "../../MapleStory.h"

#include <iostream>

namespace ms {
    InventoryType::Id InventoryType::by_item_id(int32_t item_id) {
        constexpr Id values_by_id[7] =
        {
            NONE,
            EQUIP,
            USE,
            SETUP,
            ETC,
            CASH,
            DEC
        };

        int32_t prefix = item_id / 1000000;

        return (prefix > NONE && prefix <= CASH) ? values_by_id[prefix] : NONE;
    }

    InventoryType::Id InventoryType::by_value(int8_t value) {
        switch (value) {
        case -1:
            return EQUIPPED;
        case 1:
            return EQUIP;
        case 2:
            return USE;
        case 3:
            return SETUP;
        case 4:
            return ETC;
        case 5:
            return CASH;
        case 6:
            return DEC;
        }

        LOG(LOG_DEBUG, "Unknown InventoryType::Id value: [" << value << "]");

        return NONE;
    }
}
