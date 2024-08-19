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
#include "Body.h"

#include "../../Util/Misc.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms {
    Body::Body(int32_t skin, const BodyDrawInfo& drawinfo) {
        std::string strid = string_format::extend_id(skin, 2);
        nl::node bodynode = nl::nx::Character["000020" + strid + ".img"];
        nl::node headnode = nl::nx::Character["000120" + strid + ".img"];

        for (auto iter : Stance::names) {
            Stance::Id stance = iter.first;
            const std::string& stancename = iter.second;

            nl::node stancenode = bodynode[stancename];

            if (!stancenode)
                continue;

            for (uint8_t frame = 0; nl::node framenode = stancenode[frame]; ++frame) {
                for (nl::node partnode : framenode) {
                    std::string part = partnode.name();

                    if (part != "delay" && part != "face") {
                        std::string z = partnode["z"];
                        Layer layer = layer_by_name(z);

                        if (layer == NONE)
                            continue;

                        Point<int16_t> shift;

                        switch (layer) {
                        case HAND_BELOW_WEAPON:
                            shift = drawinfo.get_hand_position(stance, frame);
                            shift -= partnode["map"]["handMove"];
                            break;
                        default:
                            shift = drawinfo.get_body_position(stance, frame);
                            shift -= partnode["map"]["navel"];
                            break;
                        }

                        stances[stance][layer]
                            .emplace(frame, partnode)
                            .first->second.shift(shift);
                    }
                }

                for (nl::node partnode : headnode[stancename][frame]) {
                    std::string part = partnode.name();
                    Layer layer = layer_by_name(part);

                    if (layer == NONE)
                        continue;

                    Point<int16_t> shift = drawinfo.get_head_position(stance, frame);

                    stances[stance][layer]
                        .emplace(frame, partnode)
                        .first->second.shift(shift);
                }
            }
        }

        constexpr size_t NUM_SKINTYPES = 13;

        constexpr const char* skintypes[NUM_SKINTYPES] =
        {
            "Light",
            "Tan",
            "Dark",
            "Pale",
            "Ashen",
            "Green",
            "",
            "",
            "",
            "Ghostly",
            "Pale Pink",
            "Clay",
            "Alabaster"
        };

        if (skin < NUM_SKINTYPES)
            name = skintypes[skin];

        if (name == "") {
            LOG(LOG_DEBUG, "Skin [" << skin << "] is using the default value.");

            name = nl::nx::String["Eqp.img"]["Eqp"]["Skin"][skin]["name"];
        }
    }

    void Body::draw(Layer layer, Stance::Id stance, uint8_t frame, const DrawArgument& args) const {
        auto frameit = stances[stance][layer].find(frame);

        if (frameit == stances[stance][layer].end())
            return;

        frameit->second.draw(args);
    }

    const std::string& Body::get_name() const {
        return name;
    }

    Body::Layer Body::layer_by_name(const std::string& name) {
        auto layer_iter = layers_by_name.find(name);

        if (layer_iter == layers_by_name.end()) {
            if (name != "")
                single_console::log_message("[Body::layer_by_name] Unknown Layer name: [" + name + "]");

            return NONE;
        }

        return layer_iter->second;
    }

    const std::unordered_map<std::string, Body::Layer> Body::layers_by_name =
    {
        {"body", BODY},
        {"backBody", BODY},
        {"arm", ARM},
        {"armBelowHead", ARM_BELOW_HEAD},
        {"armBelowHeadOverMailChest", ARM_BELOW_HEAD_OVER_MAIL},
        {"armOverHair", ARM_OVER_HAIR},
        {"armOverHairBelowWeapon", ARM_OVER_HAIR_BELOW_WEAPON},
        {"handBelowWeapon", HAND_BELOW_WEAPON},
        {"handOverHair", HAND_OVER_HAIR},
        {"handOverWeapon", HAND_OVER_WEAPON},
        {"ear", EAR},
        {"head", HEAD},
        {"highlefEar", HIGH_LEF_EAR},
        {"humanEar", HUMAN_EAR},
        {"lefEar", LEF_EAR}
    };
}
