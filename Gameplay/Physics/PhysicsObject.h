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

#include "../../Constants.h"

#include "../../Template/Interpolated.h"
#include "../../Template/Point.h"

namespace ms {
    // Structure that contains all properties for movement calculations
    struct MovingObject {
        Linear<double> x;
        Linear<double> y;
        double h_speed = 0.0;
        double v_speed = 0.0;

        void normalize() {
            x.normalize();
            y.normalize();
        }

        void move() {
            x += h_speed;
            y += v_speed;
        }

        void set_x(double d) {
            x.set(d);
        }

        void set_y(double d) {
            y.set(d);
        }

        void limit_x(double d) {
            x = d;
            h_speed = 0.0;
        }

        void limit_y(double d) {
            y = d;
            v_speed = 0.0;
        }

        void move_x_until(double d, uint16_t delay) {
            if (delay) {
                double hdelta = d - x.get();
                h_speed = Constants::TIMESTEP * hdelta / delay;
            }
        }

        void move_y_until(double d, uint16_t delay) {
            if (delay) {
                double vdelta = d - y.get();
                v_speed = Constants::TIMESTEP * vdelta / delay;
            }
        }

        bool is_moving_horizontally() const {
            return h_speed != 0.0;
        }

        bool is_moving_vertically() const {
            return v_speed != 0.0;
        }

        bool is_moving() const {
            return is_moving_horizontally() || is_moving_vertically();
        }

        double current_x() const {
            return x.get();
        }

        double current_y() const {
            return y.get();
        }

        double next_x() const {
            return x + h_speed;
        }

        double next_y() const {
            return y + v_speed;
        }

        int16_t get_x() const {
            double rounded = std::round(x.get());
            return static_cast<int16_t>(rounded);
        }

        int16_t get_y() const {
            double rounded = std::round(y.get());
            return static_cast<int16_t>(rounded);
        }

        int16_t get_last_x() const {
            double rounded = std::round(x.last());
            return static_cast<int16_t>(rounded);
        }

        int16_t get_last_y() const {
            double rounded = std::round(y.last());
            return static_cast<int16_t>(rounded);
        }

        Point<int16_t> get_position() const {
            return {get_x(), get_y()};
        }

        int16_t get_absolute_x(double viewx, float alpha) const {
            double interx = x.normalized() ? std::round(x.get()) : x.get(alpha);

            return static_cast<int16_t>(
                std::round(interx + viewx)
            );
        }

        int16_t get_absolute_y(double viewy, float alpha) const {
            double intery = y.normalized() ? std::round(y.get()) : y.get(alpha);

            return static_cast<int16_t>(
                std::round(intery + viewy)
            );
        }

        Point<int16_t> get_absolute(double viewx, double viewy, float alpha) const {
            return {get_absolute_x(viewx, alpha), get_absolute_y(viewy, alpha)};
        }
    };

    // Structure that contains all properties necessary for physics calculations
    struct PhysicsObject : MovingObject {
        // Determines which physics engine to use
        enum Type {
            NORMAL,
            FALLING,
            ICE,
            SWIMMING,
            FLYING,
            FIXATED
        };

        enum Flag {
            NO_GRAVITY = 0x0001,
            TURN_AT_EDGES = 0x0002,
            CHECK_BELOW = 0x0004
        };

        Type type = NORMAL;
        int32_t flags = 0;
        uint16_t fh_id = 0;
        double fh_slope = 0.0;
        int8_t fh_layer = 0;
        double ground_below_y = 0.0;
        bool is_on_ground = true;
        bool is_jump_down_enabled = false;

        double h_force = 0.0;
        double v_force = 0.0;
        double h_acceleration = 0.0;
        double v_acceleration = 0.0;

        bool is_flag_set(Flag f) {
            return (flags & f) != 0;
        }

        bool is_flag_not_set(Flag f) {
            return !is_flag_set(f);
        }

        void set_flag(Flag f) {
            flags |= f;
        }

        void clear_flag(Flag f) {
            flags &= ~f;
        }

        void clear_flags() {
            flags = 0;
        }
    };
}
