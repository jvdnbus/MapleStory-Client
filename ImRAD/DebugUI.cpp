// Generated with ImRAD 0.7
// visit https://github.com/tpecholt/imrad

#include "DebugUI.h"

DebugUI debugUI;


void DebugUI::Open()
{
    isOpen = true;
}

void DebugUI::Close()
{
    isOpen = false;
}

void DebugUI::Draw()
{
    /// @style Dark
    /// @unit px
    /// @begin TopWindow
    auto* ioUserData = (ImRad::IOUserData*)ImGui::GetIO().UserData;
    ImGui::SetNextWindowSize({ 400, 480 }, ImGuiCond_FirstUseEver);
    if (isOpen && ImGui::Begin("DebugUI###DebugUI", &isOpen, ImGuiWindowFlags_NoCollapse))
    {
        /// @separator

        // TODO: Add Draw calls of dependent popup windows here

        /// @begin TabBar
        if (ImGui::BeginTabBar("tabBar1", ImGuiTabBarFlags_None))
        {
            /// @separator

            /// @begin TabItem
            if (ImGui::BeginTabItem("Player", nullptr, ImGuiTabItemFlags_None))
            {
                /// @separator

                /// @begin CollapsingHeader
                ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
                if (ImGui::CollapsingHeader("Position"))
                {
                    /// @separator

                    /// @begin Text
                    ImGui::TextUnformatted("X:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("2146");
                    /// @end Text

                    /// @begin Text
                    ImRad::Spacing(1);
                    ImGui::TextUnformatted("Y:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("-156");
                    /// @end Text

                    /// @separator
                }
                /// @end CollapsingHeader

                /// @begin CollapsingHeader
                ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
                if (ImGui::CollapsingHeader("State"))
                {
                    /// @separator

                    /// @begin Text
                    ImGui::TextUnformatted("Facing:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("LEFT");
                    /// @end Text

                    /// @begin Text
                    ImGui::TextUnformatted("State:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("2");
                    /// @end Text

                    /// @separator
                }
                /// @end CollapsingHeader

                /// @separator
                ImGui::EndTabItem();
            }
            /// @end TabItem

            /// @begin TabItem
            if (ImGui::BeginTabItem("Map", nullptr, ImGuiTabItemFlags_None))
            {
                /// @separator

                /// @separator
                ImGui::EndTabItem();
            }
            /// @end TabItem

            /// @begin TabItem
            if (ImGui::BeginTabItem("UI", nullptr, ImGuiTabItemFlags_None))
            {
                /// @separator

                /// @separator
                ImGui::EndTabItem();
            }
            /// @end TabItem

            /// @begin TabItem
            if (ImGui::BeginTabItem("System", nullptr, ImGuiTabItemFlags_None))
            {
                /// @separator

                /// @begin Text
                ImGui::TextUnformatted("FPS:");
                /// @end Text

                /// @begin Text
                ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                ImGui::TextUnformatted("120");
                /// @end Text

                /// @separator
                ImGui::EndTabItem();
            }
            /// @end TabItem

            /// @separator
            ImGui::EndTabBar();
        }
        /// @end TabBar

        /// @separator
        ImGui::End();
    }
    /// @end TopWindow
}
