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

#include "../../Template/Optional.h"
#include "../../Template/Point.h"
#include "../../Template/Range.h"

#include <vector>

namespace ms {
    class Seat {
    public:
        Seat(nl::node source);

        bool in_range(Point<int16_t> position) const;
        Point<int16_t> getpos() const;

    private:
        Point<int16_t> pos;
    };

    class Ladder {
    public:
        Ladder(nl::node source);

        bool is_ladder() const;
        bool in_range(Point<int16_t> position, bool upwards) const;
        bool fell_off(int16_t y, bool downwards) const;
        int16_t get_x() const;

    private:
        int16_t x;
        int16_t y1;
        int16_t y2;
        bool ladder;
    };

    class MapInfo {
    public:
        MapInfo(nl::node src, Range<int16_t> walls, Range<int16_t> borders);
        MapInfo();

        double get_recovery() const;
        bool is_underwater() const;
        std::string get_bgm() const;
        Range<int16_t> get_walls() const;
        Range<int16_t> get_borders() const;

        // Find a seat the player's position
        Optional<const Seat> find_seat(Point<int16_t> position) const;
        // Find a ladder at the player's position
        // !upwards - implies downwards
        Optional<const Ladder> find_ladder(Point<int16_t> position, bool upwards) const;

    private:
        int32_t fieldlimit;
        double recovery;
        bool cloud;
        std::string bgm;
        std::string mapdesc;
        std::string mapname;
        std::string streetname;
        std::string mapmark;
        bool swim;
        bool town;
        bool hideminimap;
        Range<int16_t> mapwalls;
        Range<int16_t> mapborders;
        std::vector<Seat> seats;
        std::vector<Ladder> ladders;
    };
}
