#include "DebugUI.h"

#include "../Gameplay/Stage.h"

#include <GLFW/glfw3.h>

namespace ms {
    DebugUI::DebugUI() {
        is_shown = true;
    }

    Player& get_player() {
        Stage& stage = Stage::get();
        return stage.get_player();
    }

    void DebugUI::init() {
    }

    void DebugUI::update() {
        //
    }

    void DebugUI::draw() {
        if (is_shown) {
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

    void DebugUI::_draw() {
//        ImGui::ShowDemoWindow(&is_shown);

        ImGui::SetNextWindowSize({ 400, 480 }, ImGuiCond_FirstUseEver);
        if (is_shown && ImGui::Begin("DebugUI###DebugUI", &is_shown, ImGuiWindowFlags_NoCollapse)) {
            if (ImGui::BeginTabBar("tabBar1", ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("Player", nullptr, ImGuiTabItemFlags_None)) {
                    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);

                    Player& player = get_player();

                    if (ImGui::CollapsingHeader("Position")) {
                        Point<int16_t> pos = player.get_position();
                        ImGui::TextUnformatted("X:");

                        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                        ImGui::Text("%d", pos.x());

                        ImGui::TextUnformatted("Y:");

                        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                        ImGui::Text("%d", pos.y());
                    }

                    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
                    if (ImGui::CollapsingHeader("State")) {
                        ImGui::TextUnformatted("Facing:");

                        ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                        ImGui::Text("%s", player.is_facing_right() ? "RIGHT" : "LEFT");

                        ImGui::Text("%d", static_cast<int>(player.get_state()));
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
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }

    void DebugUI::show() {
        is_shown = true;
    }

    void DebugUI::hide() {
        is_shown = false;
    }
}