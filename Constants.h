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

#include "Template/Singleton.h"

#include <cstdint>

namespace ms {
    namespace Constants {
        constexpr uint16_t TIMESTEP_DEFAULT = 8;
#define TIMESTEP Constants::get().get_timestep()

        class Constants : public Singleton<Constants> {
        public:
            Constants() {
                view_width = 1024;
                view_height = 768;
                timestep = TIMESTEP_DEFAULT;
            };

            ~Constants() override = default;

            // Get the window and screen width
            int16_t get_view_width() const {
                return view_width;
            }

            // Set the window and screen width
            void set_view_width(int16_t width) {
                view_width = width;
            }

            // Get the window and screen height
            int16_t get_view_height() const {
                return view_height;
            }

            // Set the window and screen height
            void set_view_height(int16_t height) {
                view_height = height;
            }

            uint16_t get_timestep() const {
                return static_cast<uint16_t>(timestep);
            }

            int* get_timestep_ref() {
                return &timestep;
            }

            void set_paused(bool p) {
                paused = p;
            }

            bool is_paused() const {
                return paused;
            }

        private:
            // Window and screen width
            int16_t view_width;
            // Window and screen height
            int16_t view_height;
            // Timestep, i.e., the granularity in which the game advances.
            int timestep;
            // Whether the game physics are paused
            bool paused;
        };
    }
}
