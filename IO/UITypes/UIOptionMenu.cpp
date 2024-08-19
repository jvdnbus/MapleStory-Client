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
#include "UIOptionMenu.h"

#include "../KeyAction.h"

#include "../Components/MapleButton.h"
#include "../Components/TwoSpriteButton.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms {
    UIOptionMenu::UIOptionMenu() : UIDragElement<PosOPTIONMENU>(), selected_tab(0) {
        nl::node OptionMenu = nl::nx::UI["StatusBar3.img"]["OptionMenu"];
        nl::node backgrnd = OptionMenu["backgrnd"];

        sprites.emplace_back(backgrnd);
        sprites.emplace_back(OptionMenu["backgrnd2"]);

        nl::node graphic = OptionMenu["graphic"];

        tab_background[TAB0] = graphic["layer:backgrnd"];
        tab_background[TAB1] = OptionMenu["sound"]["layer:backgrnd"];
        tab_background[TAB2] = OptionMenu["game"]["layer:backgrnd"];
        tab_background[TAB3] = OptionMenu["invite"]["layer:backgrnd"];
        tab_background[TAB4] = OptionMenu["screenshot"]["layer:backgrnd"];

        buttons[CANCEL] = std::make_unique<MapleButton>(OptionMenu["button:Cancel"]);
        buttons[OK] = std::make_unique<MapleButton>(OptionMenu["button:OK"]);
        buttons[UIRESET] = std::make_unique<MapleButton>(OptionMenu["button:UIReset"]);

        nl::node tab = OptionMenu["tab"];
        nl::node tab_disabled = tab["disabled"];
        nl::node tab_enabled = tab["enabled"];

        for (size_t i = TAB0; i < CANCEL; i++)
            buttons[i] = std::make_unique<TwoSpriteButton>(tab_disabled[i], tab_enabled[i]);

        std::string sButtonUOL = graphic["combo:resolution"]["sButtonUOL"].get_string();
        auto ctype = std::string(1, sButtonUOL.back());
        auto type = static_cast<MapleComboBox::Type>(std::stoi(ctype));

        std::vector<std::string> resolutions =
        {
            "800 x 600 ( 4 : 3 )",
            "1024 x 768 ( 4 : 3 )",
            "1280 x 720 ( 16 : 9 )",
            "1366 x 768 ( 16 : 9 )",
            "1920 x 1080 ( 16 : 9 ) - Beta"
        };

        int16_t max_width = Configuration::get().get_max_width();
        int16_t max_height = Configuration::get().get_max_height();

        if (max_width >= 1920 && max_height >= 1200)
            resolutions.emplace_back("1920 x 1200 ( 16 : 10 ) - Beta");

        uint16_t default_option = 0;
        int16_t screen_width = Constants::Constants::get().get_viewwidth();
        int16_t screen_height = Constants::Constants::get().get_viewheight();

        switch (screen_width) {
        case 800:
            default_option = 0;
            break;
        case 1024:
            default_option = 1;
            break;
        case 1280:
            default_option = 2;
            break;
        case 1366:
            default_option = 3;
            break;
        case 1920:
            switch (screen_height) {
            case 1080:
                default_option = 4;
                break;
            case 1200:
                default_option = 5;
                break;
            }

            break;
        }

        int64_t combobox_width = graphic["combo:resolution"]["boxWidth"].get_integer();
        auto lt = Point<int16_t>(graphic["combo:resolution"]["lt"]);

        buttons[SELECT_RES] = std::make_unique<MapleComboBox>(type, resolutions, default_option, position, lt,
                                                              combobox_width);

        Point<int16_t> bg_dimensions = Texture(backgrnd).get_dimensions();

        dimension = bg_dimensions;
        dragarea = Point<int16_t>(bg_dimensions.x(), 20);

        change_tab(TAB2);
    }

    void UIOptionMenu::draw(float inter) const {
        draw_sprites(inter);

        tab_background[selected_tab].draw(position);

        draw_buttons(inter);
    }

    Button::State UIOptionMenu::button_pressed(uint16_t buttonid) {
        switch (buttonid) {
        case TAB0:
        case TAB1:
        case TAB2:
        case TAB3:
        case TAB4:
            change_tab(buttonid);
            return Button::State::IDENTITY;
        case CANCEL:
            deactivate();
            return Button::State::NORMAL;
        case OK:
            switch (selected_tab) {
            case TAB0: {
                uint16_t selected_value = buttons[SELECT_RES]->get_selected();

                int16_t width = Constants::Constants::get().get_viewwidth();
                int16_t height = Constants::Constants::get().get_viewheight();

                switch (selected_value) {
                case 0:
                    width = 800;
                    height = 600;
                    break;
                case 1:
                    width = 1024;
                    height = 768;
                    break;
                case 2:
                    width = 1280;
                    height = 720;
                    break;
                case 3:
                    width = 1366;
                    height = 768;
                    break;
                case 4:
                    width = 1920;
                    height = 1080;
                    break;
                case 5:
                    width = 1920;
                    height = 1200;
                    break;
                }

                Setting<Width>::get().save(width);
                Setting<Height>::get().save(height);

                Constants::Constants::get().set_viewwidth(width);
                Constants::Constants::get().set_viewheight(height);
            }
            break;
            case TAB1:
            case TAB2:
            case TAB3:
            case TAB4:
            default:
                break;
            }

            deactivate();
            return Button::State::NORMAL;
        case UIRESET:
            return Button::State::DISABLED;
        case SELECT_RES:
            buttons[SELECT_RES]->toggle_pressed();
            return Button::State::NORMAL;
        default:
            return Button::State::DISABLED;
        }
    }

    Cursor::State UIOptionMenu::send_cursor(bool clicked, Point<int16_t> cursorpos) {
        Cursor::State dstate = UIDragElement::send_cursor(clicked, cursorpos);

        if (dragged)
            return dstate;

        auto& button = buttons[SELECT_RES];

        if (button->is_pressed()) {
            if (button->in_combobox(cursorpos)) {
                if (Cursor::State new_state = button->send_cursor(clicked, cursorpos))
                    return new_state;
            } else {
                remove_cursor();
            }
        }

        return UIElement::send_cursor(clicked, cursorpos);
    }

    void UIOptionMenu::send_key(int32_t keycode, bool pressed, bool escape) {
        if (pressed) {
            if (escape)
                deactivate();
            else if (keycode == KeyAction::Id::RETURN)
                button_pressed(OK);
        }
    }

    UIElement::Type UIOptionMenu::get_type() const {
        return TYPE;
    }

    void UIOptionMenu::change_tab(uint16_t tabid) {
        buttons[selected_tab]->set_state(Button::State::NORMAL);
        buttons[tabid]->set_state(Button::State::PRESSED);

        selected_tab = tabid;

        switch (tabid) {
        case TAB0:
            buttons[SELECT_RES]->set_active(true);
            break;
        case TAB1:
        case TAB2:
        case TAB3:
        case TAB4:
            buttons[SELECT_RES]->set_active(false);
            break;
        default:
            break;
        }
    }
}
