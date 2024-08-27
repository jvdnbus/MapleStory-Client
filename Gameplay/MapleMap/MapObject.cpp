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
#include "MapObject.h"

namespace ms {
    MapObject::MapObject(int32_t o, Point<int16_t> p) : object_id(o) {
        set_position(p);
        active = true;
    }

    int8_t MapObject::update(const Physics& physics) {
        physics.move_object(physics_object);

        return physics_object.fh_layer;
    }

    void MapObject::set_position(int16_t x, int16_t y) {
        physics_object.set_x(x);
        physics_object.set_y(y);
    }

    void MapObject::set_position(Point<int16_t> position) {
        int16_t x = position.x();
        int16_t y = position.y();
        set_position(x, y);
    }

    void MapObject::activate() {
        active = true;
    }

    void MapObject::deactivate() {
        active = false;
    }

    bool MapObject::is_active() const {
        return active;
    }

    int8_t MapObject::get_layer() const {
        return physics_object.fh_layer;
    }

    int32_t MapObject::get_object_id() const {
        return object_id;
    }

    Point<int16_t> MapObject::get_position() const {
        return physics_object.get_position();
    }
}
