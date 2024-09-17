#include "DebugUI.h"

#include "../Gameplay/Stage.h"

#include <GLFW/glfw3.h>
#include <sstream>

#define DEBUG_VALUE(key, str) const DebugValue DebugValue::key = DebugValue(str);

namespace ms {
    DEBUG_VALUE(FPS, "FPS");

    DebugUI::DebugUI() {
        _is_shown = true;
    }

    Player& get_player() {
        Stage& stage = Stage::get();
        return stage.get_player();
    }

    void DebugUI::init() {
    }

    bool DebugUI::is_shown() const {
        return _is_shown;
    }

    void DebugUI::draw() {
        if (_is_shown) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            _draw();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            ImGuiIO &io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }
    }

    std::string get_phobj_type(PhysicsObject::Type t) {
        std::string ret;

        switch (t) {
            case PhysicsObject::NORMAL:
                ret = "NORMAL";
                break;
            case PhysicsObject::FALLING:
                ret = "FALLING";
                break;
            case PhysicsObject::ICE:
                ret = "ICE";
                break;
            case PhysicsObject::SWIMMING:
                ret = "SWIMMING";
                break;
            case PhysicsObject::FLYING:
                ret = "FLYING";
                break;
            case PhysicsObject::FIXATED:
                ret = "FIXATED";
                break;
        }

        return ret;
    }

    void add_physics_object(PhysicsObject& po) {
        static uint16_t table_index = 0;

        ImGui::TextUnformatted("Type:");
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::TextUnformatted(get_phobj_type(po.type).c_str());

        ImGui::TextUnformatted("Flags:");
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text("0x%x", po.flags);

        bool no_grav = po.is_flag_set(PhysicsObject::NO_GRAVITY);
        ImGui::Checkbox("NO_GRAV", &no_grav);
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        bool turn_at_edges = po.is_flag_set(PhysicsObject::TURN_AT_EDGES);
        ImGui::Checkbox("TURN_AT_EDGES", &turn_at_edges);
        bool check_below = po.is_flag_set(PhysicsObject::CHECK_BELOW);
        ImGui::Checkbox("CHECK_BELOW", &check_below);
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        bool hugging_wall = po.is_flag_set(PhysicsObject::HUGGING_WALL);
        ImGui::Checkbox("HUGGING_WALL", &hugging_wall);

        ImGui::Dummy(ImVec2(0.0f, 4.0f));

        std::ostringstream table_id_stream;
        table_id_stream << "table" << (table_index++);
        if (ImGui::BeginTable(table_id_stream.str().c_str(), 3, 0, { 0, 0 }))
        {
            ImGui::TableSetupColumn("A", ImGuiTableColumnFlags_None, 0);
            ImGui::TableSetupColumn("B", ImGuiTableColumnFlags_None, 0);
            ImGui::TableNextRow(0, 0);
            ImGui::TableSetColumnIndex(0);

            ImGui::TextUnformatted("Force:");
            ImGui::TextUnformatted("Acceleration:");
            ImGui::TextUnformatted("Velocity:");
            ImGui::TextUnformatted("Position:");

            ImGui::TableNextColumn();
            ImGui::Text("(%.3f, %.3f)", po.h_force, po.v_force);
            ImGui::Text("(%.3f, %.3f)", po.h_acceleration, po.v_acceleration);
            ImGui::Text("(%.3f, %.3f)", po.h_speed, po.v_speed);
            ImGui::Text("(%.3f, %.3f)", po.current_x(), po.current_y());

            ImGui::EndTable();
        }

        ImGui::Dummy(ImVec2(0.0f, 4.0f));
        ImGui::Text("Foothold (%d)", po.fh_id);

        ImGui::Indent(2 * ImGui::GetStyle().IndentSpacing / 2);

        ImGui::TextUnformatted("Slope:");
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text("%.3f", po.fh_slope);

        ImGui::TextUnformatted("Layer:");
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text("%d", po.fh_layer);

        ImGui::TextUnformatted("Ground below Y:");
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text("%.3f", po.ground_below_y);

        ImGui::Text("Jumping down from FH (%d)", po.jumping_down_from_fh_id);

        ImGui::TextUnformatted("On ground:");
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text("%s", po.is_on_ground ? "TRUE" : "FALSE");

        ImGui::TextUnformatted("Can jump down:");
        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text("%s", po.is_jump_down_enabled ? "TRUE" : "FALSE");
    }

    void DebugUI::_draw() {
//        ImGui::ShowDemoWindow(&_is_shown);

        ImGui::SetNextWindowSize({ 400, 480 }, ImGuiCond_FirstUseEver);
        if (_is_shown && ImGui::Begin("DebugUI###DebugUI", &_is_shown, ImGuiWindowFlags_NoCollapse)) {
            if (ImGui::BeginTabBar("tabBar1", ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("Player", nullptr, ImGuiTabItemFlags_None)) {
                    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);

                    Player& player = get_player();

                    if (ImGui::CollapsingHeader("Position")) {
                        Point<int16_t> pos = player.get_position();
                        ImGui::TextUnformatted("Root:");
                        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                        ImGui::Text("(%d, %d)", pos.x(), pos.y());
                    }

                    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
                    if (ImGui::CollapsingHeader("State")) {
                        ImGui::TextUnformatted("Facing:");
                        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                        ImGui::Text("%s", player.is_facing_right() ? "RIGHT" : "LEFT");

                        ImGui::TextUnformatted("State:");
                        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                        ImGui::Text("%d", static_cast<int>(player.get_state()));
                    }

                    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
                    if (ImGui::CollapsingHeader("PhysicsObject")) {
                        add_physics_object(player.get_physics_object());
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Map", nullptr, ImGuiTabItemFlags_None)) {
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("UI", nullptr, ImGuiTabItemFlags_None)) {
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("System", nullptr, ImGuiTabItemFlags_None)) {
                    auto fps = DebugValue::FPS;
                    ImGui::Text("%s:", fps.to_string().c_str());
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::Text("%lld", get_value(fps).get_int64());

                    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
                    if (ImGui::CollapsingHeader("Physics")) {
                        ImGui::TextUnformatted("Timestep (default: 8)");
                        ImGui::SetNextItemWidth(385);

                        ImGui::SliderInt("##timestep", Constants::Constants::get().get_timestep_ref(),
                        1, 64, "%d", ImGuiSliderFlags_AlwaysClamp & ImGuiSliderFlags_Logarithmic);
                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }

    void DebugUI::show() {
        _is_shown = true;
    }

    void DebugUI::hide() {
        _is_shown = false;
    }
}