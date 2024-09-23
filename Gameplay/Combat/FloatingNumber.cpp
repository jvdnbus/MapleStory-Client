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
#include "FloatingNumber.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms {
    FloatingNumber::FloatingNumber(Type t, int32_t damage, int16_t starty, int16_t x) {
        type = t;

        if (damage > 0) {
            miss = false;

            std::string number = std::to_string(damage);
            first_num = number[0];

            if (number.size() > 1) {
                rest_num = number.substr(1);
                multiple = true;
            } else {
                rest_num = "";
                multiple = false;
            }

            int16_t total = get_advance(first_num, true);

            for (size_t i = 0; i < rest_num.length(); i++) {
                char c = rest_num[i];
                int16_t advance;

                if (i < rest_num.length() - 1) {
                    char n = rest_num[i + 1];
                    advance = (get_advance(c, false) + get_advance(n, false)) / 2;
                } else {
                    advance = get_advance(c, false);
                }

                total += advance;
            }

            shift = total / 2;
        } else {
            shift = charsets[type][true].getw('M') / 2;
            miss = true;
        }

        moving_object.set_x(x);
        moving_object.set_y(starty);
        moving_object.v_speed = -0.25;
        opacity.set(1.5f);
    }

    FloatingNumber::FloatingNumber() {
    }

    void FloatingNumber::draw(double viewx, double viewy, float alpha) const {
        Point<int16_t> absolute = moving_object.get_absolute(viewx, viewy, alpha);
        Point<int16_t> position = absolute - Point<int16_t>(0, shift);
        float interopc = opacity.get(alpha);

        if (miss) {
            charsets[type][true].draw('M', {position, interopc});
        } else {
            charsets[type][false].draw(first_num, {position, interopc});

            if (multiple) {
                int16_t first_advance = get_advance(first_num, true);
                position.shift_x(first_advance);

                for (size_t i = 0; i < rest_num.length(); i++) {
                    char c = rest_num[i];
                    Point<int16_t> yshift = {0, (i % 2) ? -2 : 2};
                    charsets[type][true].draw(c, {position + yshift, interopc});

                    int16_t advance;

                    if (i < rest_num.length() - 1) {
                        char n = rest_num[i + 1];
                        int16_t c_advance = get_advance(c, false);
                        int16_t n_advance = get_advance(n, false);
                        advance = (c_advance + n_advance) / 2;
                    } else {
                        advance = get_advance(c, false);
                    }

                    position.shift_x(advance);
                }
            }
        }
    }

    int16_t FloatingNumber::get_advance(char c, bool first) const {
        constexpr size_t LENGTH = 10;

        constexpr int16_t advances[LENGTH] =
        {
            24, 20, 22, 22, 24, 23, 24, 22, 24, 24
        };

        size_t index = c - 48;

        if (index < LENGTH) {
            int16_t advance = advances[index];

            switch (type) {
            case CRITICAL: {
                if (first)
                    advance += 8;
                else
                    advance += 4;

                break;
            }
            default: {
                if (first)
                    advance += 2;

                break;
            }
            }

            return advance;
        }
        return 0;
    }

    void FloatingNumber::set_x(int16_t headx) {
        moving_object.set_x(headx);
    }

    bool FloatingNumber::update() {
        moving_object.move();

        float FADE_STEP = Constants::TIMESTEP * 1.0f / FADE_TIME;
        opacity -= FADE_STEP;

        return opacity.last() <= 0.0f;
    }

    int16_t FloatingNumber::row_height(bool critical) {
        return critical ? 36 : 30;
    }

    void FloatingNumber::init() {
        nl::node BasicEff = nl::nx::Effect["BasicEff.img"];

        charsets[NORMAL].set(false, BasicEff["NoRed1"], Charset::Alignment::LEFT);
        charsets[NORMAL].set(true, BasicEff["NoRed0"], Charset::Alignment::LEFT);
        charsets[CRITICAL].set(false, BasicEff["NoCri1"], Charset::Alignment::LEFT);
        charsets[CRITICAL].set(true, BasicEff["NoCri0"], Charset::Alignment::LEFT);
        charsets[TO_PLAYER].set(false, BasicEff["NoViolet1"], Charset::Alignment::LEFT);
        charsets[TO_PLAYER].set(true, BasicEff["NoViolet0"], Charset::Alignment::LEFT);
        charsets[HEAL].set(false, BasicEff["NoBlue1"], Charset::Alignment::LEFT);
        charsets[HEAL].set(true, BasicEff["NoBlue0"], Charset::Alignment::LEFT);
    }

    BoolPair<Charset> FloatingNumber::charsets[NUM_TYPES];
}
