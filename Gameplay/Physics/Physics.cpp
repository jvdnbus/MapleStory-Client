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
#include "Physics.h"

namespace ms {
    constexpr double GRAVFORCE = 0.14;
    constexpr double SWIMGRAVFORCE = 0.03;
    constexpr double FRICTION = 0.5;
    constexpr double SLOPEFACTOR = 0.1;
    constexpr double GROUNDSLIP = 3.0;
    constexpr double FLYFRICTION = 0.05;
    constexpr double SWIMFRICTION = 0.08;

    Physics::Physics(nl::node src) {
        fht = src;
    }

    Physics::Physics() {
    }

    void Physics::move_object(PhysicsObject& phobj) const {
        // Determine which platform the object is currently on
        fht.update_fh(phobj);

        // Use the appropriate physics for the terrain the object is on
        switch (phobj.type) {
        case PhysicsObject::Type::NORMAL:
            move_normal(phobj);
            fht.limit_movement(phobj);
            break;
        case PhysicsObject::Type::FLYING:
            move_flying(phobj);
            fht.limit_movement(phobj);
            break;
        case PhysicsObject::Type::SWIMMING:
            move_swimming(phobj);
            fht.limit_movement(phobj);
            break;
        case PhysicsObject::Type::FIXATED:
        default:
            break;
        }

        // Move the object forward
        phobj.move();
    }

    void Physics::move_normal(PhysicsObject& phobj) const {
        phobj.v_acceleration = 0.0;
        phobj.h_acceleration = 0.0;

        if (phobj.is_on_ground) {
            phobj.v_acceleration += phobj.v_force;
            phobj.h_acceleration += phobj.h_force;

            if (phobj.h_acceleration == 0.0 && phobj.h_speed < 0.1 && phobj.h_speed > -0.1) {
                phobj.h_speed = 0.0;
            } else {
                double inertia = phobj.h_speed / GROUNDSLIP;
                double slopef = phobj.fh_slope;

                if (slopef > 0.5)
                    slopef = 0.5;
                else if (slopef < -0.5)
                    slopef = -0.5;

                phobj.h_acceleration -= (FRICTION + SLOPEFACTOR * (1.0 + slopef * -inertia)) * inertia;
            }
        } else if (phobj.is_flag_not_set(PhysicsObject::Flag::NO_GRAVITY)) {
            phobj.v_acceleration += GRAVFORCE;
        }

        phobj.h_force = 0.0;
        phobj.v_force = 0.0;

        phobj.h_speed += phobj.h_acceleration;
        phobj.v_speed += phobj.v_acceleration;
    }

    void Physics::move_flying(PhysicsObject& phobj) const {
        phobj.h_acceleration = phobj.h_force;
        phobj.v_acceleration = phobj.v_force;
        phobj.h_force = 0.0;
        phobj.v_force = 0.0;

        phobj.h_acceleration -= FLYFRICTION * phobj.h_speed;
        phobj.v_acceleration -= FLYFRICTION * phobj.v_speed;

        phobj.h_speed += phobj.h_acceleration;
        phobj.v_speed += phobj.v_acceleration;

        if (phobj.h_acceleration == 0.0 && phobj.h_speed < 0.1 && phobj.h_speed > -0.1)
            phobj.h_speed = 0.0;

        if (phobj.v_acceleration == 0.0 && phobj.v_speed < 0.1 && phobj.v_speed > -0.1)
            phobj.v_speed = 0.0;
    }

    void Physics::move_swimming(PhysicsObject& phobj) const {
        phobj.h_acceleration = phobj.h_force;
        phobj.v_acceleration = phobj.v_force;
        phobj.h_force = 0.0;
        phobj.v_force = 0.0;

        phobj.h_acceleration -= SWIMFRICTION * phobj.h_speed;
        phobj.v_acceleration -= SWIMFRICTION * phobj.v_speed;

        if (phobj.is_flag_not_set(PhysicsObject::Flag::NO_GRAVITY))
            phobj.v_acceleration += SWIMGRAVFORCE;

        phobj.h_speed += phobj.h_acceleration;
        phobj.v_speed += phobj.v_acceleration;

        if (phobj.h_acceleration == 0.0 && phobj.h_speed < 0.1 && phobj.h_speed > -0.1)
            phobj.h_speed = 0.0;

        if (phobj.v_acceleration == 0.0 && phobj.v_speed < 0.1 && phobj.v_speed > -0.1)
            phobj.v_speed = 0.0f;
    }

    Point<int16_t> Physics::get_y_below(Point<int16_t> position) const {
        int16_t ground = fht.get_y_below(position);

        return Point<int16_t>(position.x(), ground - 1);
    }

    const FootholdTree& Physics::get_fht() const {
        return fht;
    }
}
