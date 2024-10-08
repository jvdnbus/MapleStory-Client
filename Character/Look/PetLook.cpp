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
#include "PetLook.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms {
    PetLook::PetLook(int32_t iid, std::string nm, int32_t uqid, Point<int16_t> pos, uint8_t st, int32_t) {
        itemid = iid;
        name = nm;
        uniqueid = uqid;

        set_position(pos.x(), pos.y());
        set_stance(st);

        namelabel = Text(Text::Font::A13M, Text::Alignment::CENTER, Color::Name::WHITE, Text::Background::NAMETAG,
                         name);

        std::string strid = std::to_string(iid);
        nl::node src = nl::nx::Item["Pet"][strid + ".img"];

        animations[MOVE] = src["move"];
        animations[STAND] = src["stand0"];
        animations[JUMP] = src["jump"];
        animations[ALERT] = src["alert"];
        animations[PRONE] = src["prone"];
        animations[FLY] = src["fly"];
        animations[HANG] = src["hang"];

        nl::node effsrc = nl::nx::Effect["PetEff.img"][strid];

        animations[WARP] = effsrc["warp"];
    }

    PetLook::PetLook() {
        itemid = 0;
        name = "";
        uniqueid = 0;
        stance = STAND;
    }

    void PetLook::draw(double viewx, double viewy, float alpha) const {
        Point<int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);

        animations[stance].draw(DrawArgument(absp, flip), alpha);
        namelabel.draw(absp);
    }

    void PetLook::update(const Physics& physics, Point<int16_t> charpos) {
        static constexpr double PETWALKFORCE = 0.35;
        static constexpr double PETFLYFORCE = 0.2;

        Point<int16_t> curpos = phobj.get_position();

        switch (stance) {
        case STAND:
        case MOVE:
            if (curpos.distance(charpos) > 150) {
                set_position(charpos.x(), charpos.y());
            } else {
                if (charpos.x() - curpos.x() > 50) {
                    phobj.h_force = PETWALKFORCE;
                    flip = true;

                    set_stance(MOVE);
                } else if (charpos.x() - curpos.x() < -50) {
                    phobj.h_force = -PETWALKFORCE;
                    flip = false;

                    set_stance(MOVE);
                } else {
                    phobj.h_force = 0.0;

                    set_stance(STAND);
                }
            }

            phobj.type = PhysicsObject::Type::NORMAL;
            phobj.clear_flag(PhysicsObject::Flag::NO_GRAVITY);
            break;
        case HANG:
            set_position(charpos.x(), charpos.y());
            phobj.set_flag(PhysicsObject::Flag::NO_GRAVITY);
            break;
        case FLY:
            if ((charpos - curpos).length() > 250) {
                set_position(charpos.x(), charpos.y());
            } else {
                if (charpos.x() - curpos.x() > 50) {
                    phobj.h_force = PETFLYFORCE;
                    flip = true;
                } else if (charpos.x() - curpos.x() < -50) {
                    phobj.h_force = -PETFLYFORCE;
                    flip = false;
                } else {
                    phobj.h_force = 0.0f;
                }

                if (charpos.y() - curpos.y() > 50.0f)
                    phobj.v_force = PETFLYFORCE;
                else if (charpos.y() - curpos.y() < -50.0f)
                    phobj.v_force = -PETFLYFORCE;
                else
                    phobj.v_force = 0.0f;
            }

            phobj.type = PhysicsObject::Type::FLYING;
            phobj.clear_flag(PhysicsObject::Flag::NO_GRAVITY);
            break;
        }

        physics.move_object(phobj);

        animations[stance].update();
    }

    void PetLook::set_position(int16_t x, int16_t y) {
        phobj.set_x(x);
        phobj.set_y(y);
    }

    void PetLook::set_stance(Stance st) {
        if (stance != st) {
            stance = st;
            animations[stance].reset();
        }
    }

    void PetLook::set_stance(uint8_t stancebyte) {
        flip = stancebyte % 2 == 1;
        stance = stancebyvalue(stancebyte);
    }

    int32_t PetLook::get_itemid() const {
        return itemid;
    }

    PetLook::Stance PetLook::get_stance() const {
        return stance;
    }
}
