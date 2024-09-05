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
#include "Camera.h"

#include "../Constants.h"

#define EASY_IN_OUT_CUBIC(x) std::fmin(1.0, std::fmax(0.1, x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2))

namespace ms {
    Camera::Camera() {
        x.set(0.0);
        y.set(0.0);

        v_width = Constants::Constants::get().get_view_width();
        v_height = Constants::Constants::get().get_view_height();
    }

    void Camera::update(Point<int16_t> position) {
        int32_t new_width = Constants::Constants::get().get_view_width();
        int32_t new_height = Constants::Constants::get().get_view_height();

        if (v_width != new_width || v_height != new_height) {
            v_width = new_width;
            v_height = new_height;
        }

        double next_x = x.get();
        double hdelta = v_width / 2.0 - position.x() - next_x;
        next_x += hdelta * H_LERP_MULT * EASY_IN_OUT_CUBIC(std::abs(hdelta - LAZY_ZONE) / H_LERP_MAX) / v_width;

        double next_y = y.get();
        double vdelta = (v_height * V_OFFSET) + (v_height / 2.0) - position.y() - next_y;
        next_y += vdelta * V_LERP_MULT * EASY_IN_OUT_CUBIC(std::abs(vdelta - LAZY_ZONE) / V_LERP_MAX) / v_height;

        if (next_x > h_bounds.first() || h_bounds.length() < v_width) {
            next_x = h_bounds.first();
        } else if (next_x < h_bounds.second() + v_width) {
            next_x = h_bounds.second() + v_width;
        }

        if (next_y > v_bounds.first() || v_bounds.length() < v_height) {
            next_y = v_bounds.first();
        } else if (next_y < v_bounds.second() + v_height) {
            next_y = v_bounds.second() + v_height;
        }

        x = next_x;
        y = next_y;
    }

    void Camera::set_position(Point<int16_t> position) {
        int32_t new_width = Constants::Constants::get().get_view_width();
        int32_t new_height = Constants::Constants::get().get_view_height();

        if (v_width != new_width || v_height != new_height) {
            v_width = new_width;
            v_height = new_height;
        }

        x.set(v_width / 2.0 - position.x());
        y.set((v_height * V_OFFSET) + (v_height / 2.0) - position.y());
    }

    void Camera::set_view(Range<int16_t> mapwalls, Range<int16_t> mapborders) {
        h_bounds = -mapwalls;
        v_bounds = -mapborders;
    }

    Point<int16_t> Camera::position() const {
        auto shortx = static_cast<int16_t>(std::round(x.get()));
        auto shorty = static_cast<int16_t>(std::round(y.get()));

        return {shortx, shorty};
    }

    Point<int16_t> Camera::position(float alpha) const {
        auto interx = static_cast<int16_t>(std::round(x.get(alpha)));
        auto intery = static_cast<int16_t>(std::round(y.get(alpha)));

        return {interx, intery};
    }

    Point<double> Camera::real_position(float alpha) const {
        return {x.get(alpha), y.get(alpha)};
    }
}
