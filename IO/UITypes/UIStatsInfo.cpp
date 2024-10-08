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
#include "UIStatsInfo.h"

#include "UINotice.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../../Gameplay/Stage.h"

#include "../../Net/Packets/PlayerPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms {
    UIStatsInfo::UIStatsInfo(const CharStats& st) : UIDragElement<PosSTATS>(Point<int16_t>(212, 20)), stats(st) {
        nl::node close = nl::nx::UI["Basic.img"]["BtClose3"];
        nl::node Stat = nl::nx::UI["UIWindow4.img"]["Stat"];
        nl::node main = Stat["main"];
        nl::node detail = Stat["detail"];
        nl::node abilityTitle = detail["abilityTitle"];
        nl::node metierLine = detail["metierLine"];

        sprites.emplace_back(main["backgrnd"]);
        sprites.emplace_back(main["backgrnd2"]);
        sprites.emplace_back(main["backgrnd3"]);

        cover0 = Sprite(main["cover0"]);
        cover1 = Sprite(main["cover1"]);

        textures_detail.emplace_back(detail["backgrnd"]);
        textures_detail.emplace_back(detail["backgrnd2"]);
        textures_detail.emplace_back(detail["backgrnd3"]);
        textures_detail.emplace_back(detail["backgrnd4"]);

        abilities[RARE] = abilityTitle["rare"]["0"];
        abilities[EPIC] = abilityTitle["epic"]["0"];
        abilities[UNIQUE] = abilityTitle["unique"]["0"];
        abilities[LEGENDARY] = abilityTitle["legendary"]["0"];
        abilities[NONE] = abilityTitle["normal"]["0"];

        inner_ability[true] = metierLine["activated"]["0"];
        inner_ability[false] = metierLine["disabled"]["0"];

        buttons[BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(190, 6));
        buttons[BT_HP] = std::make_unique<MapleButton>(main["BtHpUp"]);
        buttons[BT_MP] = std::make_unique<MapleButton>(main["BtMpUp"]);
        buttons[BT_STR] = std::make_unique<MapleButton>(main["BtStrUp"]);
        buttons[BT_DEX] = std::make_unique<MapleButton>(main["BtDexUp"]);
        buttons[BT_INT] = std::make_unique<MapleButton>(main["BtIntUp"]);
        buttons[BT_LUK] = std::make_unique<MapleButton>(main["BtLukUp"]);
        buttons[BT_AUTO] = std::make_unique<MapleButton>(main["BtAuto"]);
        buttons[BT_HYPERSTATOPEN] = std::make_unique<MapleButton>(main["BtHyperStatOpen"]);
        buttons[BT_HYPERSTATCLOSE] = std::make_unique<MapleButton>(main["BtHyperStatClose"]);
        buttons[BT_DETAILOPEN] = std::make_unique<MapleButton>(main["BtDetailOpen"]);
        buttons[BT_DETAILCLOSE] = std::make_unique<MapleButton>(main["BtDetailClose"]);
        buttons[BT_ABILITY] = std::make_unique<MapleButton>(detail["BtAbility"], Point<int16_t>(212, 0));
        buttons[BT_DETAIL_DETAILCLOSE] = std::make_unique<MapleButton>(
            detail["BtHpUp"], Point<int16_t>(212, 0));

        buttons[BT_HYPERSTATOPEN]->set_active(false);
        buttons[BT_DETAILCLOSE]->set_active(false);
        buttons[BT_ABILITY]->set_active(false);
        buttons[BT_ABILITY]->set_state(Button::State::DISABLED);
        buttons[BT_DETAIL_DETAILCLOSE]->set_active(false);

        jobId = stats.get_stat(MapleStat::Id::JOB);

        if (jobId == Job::Level::BEGINNER) {
            buttons[BT_HP]->set_active(false);
            buttons[BT_MP]->set_active(false);
            buttons[BT_STR]->set_active(false);
            buttons[BT_DEX]->set_active(false);
            buttons[BT_INT]->set_active(false);
            buttons[BT_LUK]->set_active(false);
            buttons[BT_AUTO]->set_active(false);
        }

        update_ap();

        // Normal
        for (size_t i = NAME; i <= LUK; i++)
            statlabels[i] = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);

        statlabels[AP] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);

        auto statoffset = Point<int16_t>(73, 29);
        int16_t statoffset_y = 18;

        statoffsets[NAME] = statoffset;
        statoffsets[JOB] = statoffset + Point<int16_t>(1, statoffset_y * 1);
        statoffsets[GUILD] = statoffset + Point<int16_t>(1, statoffset_y * 2);
        statoffsets[FAME] = statoffset + Point<int16_t>(1, statoffset_y * 3);
        statoffsets[MIN_DAMAGE] = statoffset + Point<int16_t>(15, statoffset_y * 4);
        statoffsets[MAX_DAMAGE] = statoffset + Point<int16_t>(19, statoffset_y * 5);
        statoffsets[HP] = statoffset + Point<int16_t>(1, statoffset_y * 6);
        statoffsets[MP] = statoffset + Point<int16_t>(1, statoffset_y * 7);
        statoffsets[AP] = statoffset + Point<int16_t>(19, 167);
        statoffsets[STR] = statoffset + Point<int16_t>(1, 196);
        statoffsets[DEX] = statoffset + Point<int16_t>(1, 214);
        statoffsets[INT] = statoffset + Point<int16_t>(1, 232);
        statoffsets[LUK] = statoffset + Point<int16_t>(1, 250);

        // Detailed
        statlabels[MIN_DAMAGE_DETAILED] = Text(Text::Font::A11M, Text::Alignment::RIGHT,
                                               Color::Name::EMPEROR);
        statlabels[MAX_DAMAGE_DETAILED] = Text(Text::Font::A11M, Text::Alignment::RIGHT,
                                               Color::Name::EMPEROR);
        statlabels[DAMAGE_BONUS] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[BOSS_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[FINAL_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[BUFF_DURATION] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[IGNORE_DEFENSE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[ITEM_DROP_RATE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[CRITICAL_RATE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[MESOS_OBTAINED] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[CRITICAL_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[STATUS_RESISTANCE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[KNOCKBACK_RESISTANCE] = Text(Text::Font::A11M, Text::Alignment::RIGHT,
                                                Color::Name::EMPEROR);
        statlabels[DEFENSE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[SPEED] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[JUMP] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
        statlabels[HONOR] = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);

        auto statoffset_detailed = Point<int16_t>(0, 77);
        int16_t statoffset_detailed_lx = 116;
        int16_t statoffset_detailed_rx = 227;

        statoffsets[MIN_DAMAGE_DETAILED] = Point<int16_t>(94, 41);
        statoffsets[MAX_DAMAGE_DETAILED] = Point<int16_t>(105, 59);

        statoffsets[DAMAGE_BONUS] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_lx, statoffset_y * 0);
        statoffsets[BOSS_DAMAGE] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_rx, statoffset_y * 0);

        statoffsets[FINAL_DAMAGE] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_lx, statoffset_y * 1);
        statoffsets[BUFF_DURATION] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_rx, statoffset_y * 1);

        statoffsets[IGNORE_DEFENSE] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_lx, statoffset_y * 2);
        statoffsets[ITEM_DROP_RATE] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_rx, statoffset_y * 2);

        statoffsets[CRITICAL_RATE] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_lx, statoffset_y * 3);
        statoffsets[MESOS_OBTAINED] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_rx, statoffset_y * 3);

        statoffsets[CRITICAL_DAMAGE] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_lx + 3, statoffset_y * 4);

        statoffsets[STATUS_RESISTANCE] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_lx, statoffset_y * 5);
        statoffsets[KNOCKBACK_RESISTANCE] = statoffset_detailed + Point<int16_t>(
            statoffset_detailed_rx, statoffset_y * 5);

        statoffsets[DEFENSE] = statoffset_detailed + Point<
            int16_t>(statoffset_detailed_lx, statoffset_y * 6);

        statoffsets[SPEED] = statoffset_detailed + Point<int16_t>(statoffset_detailed_lx, statoffset_y * 7);
        statoffsets[JUMP] = statoffset_detailed + Point<int16_t>(statoffset_detailed_lx, statoffset_y * 8);

        statoffsets[HONOR] = statoffset_detailed + Point<int16_t>(73, 263);

        update_all_stats();
        update_stat(MapleStat::Id::JOB);
        update_stat(MapleStat::Id::FAME);

        dimension = Point<int16_t>(212, 318);
        showdetail = false;
    }

    void UIStatsInfo::draw(float alpha) const {
        draw_sprites(alpha);

        if (jobId == Job::Level::BEGINNER) {
            cover0.draw(position, alpha);
            cover1.draw(position, alpha);
        }

        if (showdetail) {
            Point<int16_t> detail_pos(position + Point<int16_t>(212, 0));

            textures_detail[0].draw(detail_pos + Point<int16_t>(0, -1));
            textures_detail[1].draw(detail_pos);
            textures_detail[2].draw(detail_pos);
            textures_detail[3].draw(detail_pos);

            abilities[NONE].draw(DrawArgument(detail_pos));

            inner_ability[false].draw(detail_pos);
            inner_ability[false].draw(detail_pos + Point<int16_t>(0, 19));
            inner_ability[false].draw(detail_pos + Point<int16_t>(0, 38));
        }

        size_t last = showdetail ? NUM_LABELS : NUM_NORMAL;

        for (size_t i = 0; i < last; i++) {
            Point<int16_t> labelpos = position + statoffsets[i];

            if (i >= NUM_NORMAL)
                labelpos.shift_x(213);

            if (jobId == Job::Level::BEGINNER) {
                if (i < AP || i > NUM_NORMAL)
                    statlabels[i].draw(labelpos);
            } else {
                statlabels[i].draw(labelpos);
            }
        }

        draw_buttons(alpha);
    }

    void UIStatsInfo::send_key(int32_t keycode, bool pressed, bool escape) {
        if (pressed && escape)
            deactivate();
    }

    bool UIStatsInfo::is_in_range(Point<int16_t> cursorpos) const {
        Point<int16_t> pos_adj;

        if (showdetail)
            pos_adj = Point<int16_t>(211, 25);
        else
            pos_adj = Point<int16_t>(0, 0);

        auto bounds = Rectangle<int16_t>(position, position + dimension + pos_adj);
        return bounds.contains(cursorpos);
    }

    UIElement::Type UIStatsInfo::get_type() const {
        return TYPE;
    }

    void UIStatsInfo::update_all_stats() {
        update_simple(AP, MapleStat::Id::AP);

        if (hasap ^ (stats.get_stat(MapleStat::Id::AP) > 0))
            update_ap();

        statlabels[NAME].change_text(stats.get_name());
        statlabels[GUILD].change_text("-");
        statlabels[HP].change_text(
            std::to_string(stats.get_stat(MapleStat::Id::HP)) + " / " + std::to_string(
                stats.get_total(EquipStat::Id::HP)));
        statlabels[MP].change_text(
            std::to_string(stats.get_stat(MapleStat::Id::MP)) + " / " + std::to_string(
                stats.get_total(EquipStat::Id::MP)));

        update_basevstotal(STR, MapleStat::Id::STR, EquipStat::Id::STR);
        update_basevstotal(DEX, MapleStat::Id::DEX, EquipStat::Id::DEX);
        update_basevstotal(INT, MapleStat::Id::INT, EquipStat::Id::INT);
        update_basevstotal(LUK, MapleStat::Id::LUK, EquipStat::Id::LUK);

        statlabels[MIN_DAMAGE].change_text(std::to_string(stats.get_mindamage()));
        statlabels[MAX_DAMAGE].change_text(" ~ " + std::to_string(stats.get_maxdamage()));

        if (stats.is_damage_buffed()) {
            statlabels[MIN_DAMAGE].change_color(Color::Name::RED);
            statlabels[MIN_DAMAGE].change_color(Color::Name::RED);
        } else {
            statlabels[MAX_DAMAGE].change_color(Color::Name::EMPEROR);
            statlabels[MAX_DAMAGE].change_color(Color::Name::EMPEROR);
        }

        statlabels[MIN_DAMAGE_DETAILED].change_text(std::to_string(stats.get_mindamage()));
        statlabels[MAX_DAMAGE_DETAILED].change_text(" ~ " + std::to_string(stats.get_maxdamage()));
        statlabels[DAMAGE_BONUS].change_text("0%");
        statlabels[BOSS_DAMAGE].change_text(
            std::to_string(static_cast<int32_t>(stats.get_bossdmg() * 100)) + "%");
        statlabels[FINAL_DAMAGE].change_text("0.00%");
        statlabels[BUFF_DURATION].change_text("0%");
        statlabels[IGNORE_DEFENSE].change_text(
            std::to_string(static_cast<int32_t>(stats.get_ignoredef())) + "%");
        statlabels[ITEM_DROP_RATE].change_text("0%");
        statlabels[CRITICAL_RATE].change_text(
            std::to_string(static_cast<int32_t>(stats.get_critical() * 100)) + "%");
        statlabels[MESOS_OBTAINED].change_text("0%");
        statlabels[CRITICAL_DAMAGE].change_text("0.00%");
        statlabels[STATUS_RESISTANCE].change_text(
            std::to_string(static_cast<int32_t>(stats.get_resistance())));
        statlabels[KNOCKBACK_RESISTANCE].change_text("0%");

        update_buffed(DEFENSE, EquipStat::Id::WDEF);

        statlabels[SPEED].change_text(std::to_string(stats.get_total(EquipStat::Id::SPEED)) + "%");
        statlabels[JUMP].change_text(std::to_string(stats.get_total(EquipStat::Id::JUMP)) + "%");
        statlabels[HONOR].change_text(std::to_string(stats.get_honor()));
    }

    void UIStatsInfo::update_stat(MapleStat::Id stat) {
        switch (stat) {
        case MapleStat::Id::JOB: {
            jobId = stats.get_stat(MapleStat::Id::JOB);

            statlabels[JOB].change_text(stats.get_jobname());

            buttons[BT_HP]->set_active(jobId != Job::Level::BEGINNER);
            buttons[BT_MP]->set_active(jobId != Job::Level::BEGINNER);
            buttons[BT_STR]->set_active(jobId != Job::Level::BEGINNER);
            buttons[BT_DEX]->set_active(jobId != Job::Level::BEGINNER);
            buttons[BT_INT]->set_active(jobId != Job::Level::BEGINNER);
            buttons[BT_LUK]->set_active(jobId != Job::Level::BEGINNER);
            buttons[BT_AUTO]->set_active(jobId != Job::Level::BEGINNER);

            break;
        }
        case MapleStat::Id::FAME: {
            update_simple(FAME, MapleStat::Id::FAME);
            break;
        }
        }
    }

    Button::State UIStatsInfo::button_pressed(uint16_t id) {
        const Player& player = Stage::get().get_player();

        switch (id) {
        case BT_CLOSE: {
            deactivate();
            break;
        }
        case BT_HP: {
            send_apup(MapleStat::Id::HP);
            break;
        }
        case BT_MP: {
            send_apup(MapleStat::Id::MP);
            break;
        }
        case BT_STR: {
            send_apup(MapleStat::Id::STR);
            break;
        }
        case BT_DEX: {
            send_apup(MapleStat::Id::DEX);
            break;
        }
        case BT_INT: {
            send_apup(MapleStat::Id::INT);
            break;
        }
        case BT_LUK: {
            send_apup(MapleStat::Id::LUK);
            break;
        }
        case BT_AUTO: {
            uint16_t autostr = 0;
            uint16_t autodex = 0;
            uint16_t autoint = 0;
            uint16_t autoluk = 0;
            uint16_t nowap = stats.get_stat(MapleStat::Id::AP);
            EquipStat::Id id = player.get_stats().get_job().get_primary(player.get_weapon_type());

            switch (id) {
            case EquipStat::Id::STR:
                autostr = nowap;
                break;
            case EquipStat::Id::DEX:
                autodex = nowap;
                break;
            case EquipStat::Id::INT:
                autoint = nowap;
                break;
            case EquipStat::Id::LUK:
                autoluk = nowap;
                break;
            }

            std::string message =
                "Your AP will be distributed as follows:\\r"
                "\\nSTR : +" + std::to_string(autostr) +
                "\\nDEX : +" + std::to_string(autodex) +
                "\\nINT : +" + std::to_string(autoint) +
                "\\nLUK : +" + std::to_string(autoluk) +
                "\\r\\n";

            std::function<void(bool)> yesnohandler = [&, autostr, autodex, autoint, autoluk](bool yes) {
                if (yes) {
                    if (autostr > 0)
                        for (size_t i = 0; i < autostr; i++)
                            send_apup(MapleStat::Id::STR);

                    if (autodex > 0)
                        for (size_t i = 0; i < autodex; i++)
                            send_apup(MapleStat::Id::DEX);

                    if (autoint > 0)
                        for (size_t i = 0; i < autoint; i++)
                            send_apup(MapleStat::Id::INT);

                    if (autoluk > 0)
                        for (size_t i = 0; i < autoluk; i++)
                            send_apup(MapleStat::Id::LUK);
                }
            };

            UI::get().emplace<UIYesNo>(message, yesnohandler, Text::Alignment::LEFT);
            break;
        }
        case BT_HYPERSTATOPEN: {
            break;
        }
        case BT_HYPERSTATCLOSE: {
            if (player.get_level() < 140)
                UI::get().emplace<UIOk>("You can use the Hyper Stat at Lv. 140 and above.", [](bool) {
                });

            break;
        }
        case BT_DETAILOPEN: {
            set_detail(true);
            break;
        }
        case BT_DETAILCLOSE:
        case BT_DETAIL_DETAILCLOSE: {
            set_detail(false);
            break;
        }
        case BT_ABILITY:
        default: {
            break;
        }
        }

        return Button::State::NORMAL;
    }

    void UIStatsInfo::send_apup(MapleStat::Id stat) const {
        SpendApPacket(stat).dispatch();
        UI::get().disable();
    }

    void UIStatsInfo::set_detail(bool enabled) {
        showdetail = enabled;

        buttons[BT_DETAILOPEN]->set_active(!enabled);
        buttons[BT_DETAILCLOSE]->set_active(enabled);
        buttons[BT_ABILITY]->set_active(enabled);
        buttons[BT_DETAIL_DETAILCLOSE]->set_active(enabled);
    }

    void UIStatsInfo::update_ap() {
        bool nowap = stats.get_stat(MapleStat::Id::AP) > 0;
        Button::State newstate = nowap ? Button::State::NORMAL : Button::State::DISABLED;

        for (int i = BT_HP; i <= BT_AUTO; i++)
            buttons[i]->set_state(newstate);

        hasap = nowap;
    }

    void UIStatsInfo::update_simple(StatLabel label, MapleStat::Id stat) {
        statlabels[label].change_text(std::to_string(stats.get_stat(stat)));
    }

    void UIStatsInfo::update_basevstotal(StatLabel label, MapleStat::Id bstat, EquipStat::Id tstat) {
        int32_t base = stats.get_stat(bstat);
        int32_t total = stats.get_total(tstat);
        int32_t delta = total - base;

        std::string stattext = std::to_string(total);

        if (delta) {
            stattext += " (" + std::to_string(base);

            if (delta > 0)
                stattext += "+" + std::to_string(delta);
            else if (delta < 0)
                stattext += "-" + std::to_string(-delta);

            stattext += ")";
        }

        statlabels[label].change_text(stattext);
    }

    void UIStatsInfo::update_buffed(StatLabel label, EquipStat::Id stat) {
        int32_t total = stats.get_total(stat);
        int32_t delta = stats.get_buffdelta(stat);

        std::string stattext = std::to_string(total);

        if (delta) {
            stattext += " (" + std::to_string(total - delta);

            if (delta > 0) {
                stattext += "+" + std::to_string(delta);

                statlabels[label].change_color(Color::Name::RED);
            } else if (delta < 0) {
                stattext += "-" + std::to_string(-delta);

                statlabels[label].change_color(Color::Name::BLUE);
            }

            stattext += ")";
        }

        statlabels[label].change_text(stattext);
    }
}
