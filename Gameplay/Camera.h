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

#include "../Template/Interpolated.h"
#include "../Template/Point.h"
#include "../Template/Range.h"

#include <cstdint>

namespace ms {
    // View on stage which follows the player object.
    class Camera {
    public:
        // Initialize everything to 0, we need the player's spawnpoint first to properly set the position.
        Camera();

        // Update the view with the current player position. (Or any other target)
        void update(Point<int16_t> position);
        // Set the position, changing the view immediately.
        void set_position(Point<int16_t> position);
        // Updates the view's boundaries. Determined by map_info or footholds.
        void set_view(Range<int16_t> hborders, Range<int16_t> vborders);
        // Return the current position.
        Point<int16_t> position() const;
        // Return the interpolated position.
        Point<int16_t> position(float alpha) const;
        // Return the interpolated position.
        Point<double> real_position(float alpha) const;

    private:
        // Movement variables.
        Linear<double> x;
        Linear<double> y;

        // View limits.
        Range<int16_t> h_bounds;
        Range<int16_t> v_bounds;

        int16_t v_width;
        int16_t v_height;

        // Vertical shift offset % of window height
        const double V_OFFSET = 1.0 / 8;

        // Easing camera config
        const double H_LERP_MULT = 45;
        const double V_LERP_MULT = 30;
        const double LAZY_ZONE = 25;
        const double H_LERP_MAX = 200;
        const double V_LERP_MAX = 150;
    };
}
