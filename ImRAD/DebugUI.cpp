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
    ImGui::SetNextWindowSize({ 400, 540 }, ImGuiCond_FirstUseEver);
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
                    ImGui::TextUnformatted("Root:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("(2146, -153)");
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

                /// @begin CollapsingHeader
                ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
                if (ImGui::CollapsingHeader("PhysicsObject"))
                {
                    /// @separator

                    /// @begin Text
                    ImGui::TextUnformatted("Type:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("NORMAL");
                    /// @end Text

                    /// @begin Text
                    ImGui::TextUnformatted("Flags:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("0x0000");
                    /// @end Text

                    /// @begin CheckBox
                    ImGui::Checkbox("NO_GRAV", &value8);
                    /// @end CheckBox

                    /// @begin CheckBox
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::Checkbox("TURN_AT_EDGES", &value5);
                    /// @end CheckBox

                    /// @begin CheckBox
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::Checkbox("CHECK_BELOW", &value6);
                    /// @end CheckBox

                    /// @begin Table
                    ImRad::Spacing(2);
                    if (ImGui::BeginTable("table2", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV, { 0, 0 }))
                    {
                        ImGui::TableSetupColumn("A", ImGuiTableColumnFlags_None, 0);
                        ImGui::TableSetupColumn("B", ImGuiTableColumnFlags_None, 0);
                        ImGui::TableNextRow(0, 0);
                        ImGui::TableSetColumnIndex(0);
                        /// @separator

                        /// @begin Text
                        ImGui::TextUnformatted("Force:");
                        /// @end Text

                        /// @begin Text
                        ImGui::TextUnformatted("Acceleration:");
                        /// @end Text

                        /// @begin Text
                        ImGui::TextUnformatted("Velocity:");
                        /// @end Text

                        /// @begin Text
                        ImGui::TextUnformatted("Position:");
                        /// @end Text

                        /// @begin Text
                        ImRad::TableNextColumn(1);
                        ImGui::TextUnformatted("(0.00, 0.00)");
                        /// @end Text

                        /// @begin Text
                        ImGui::TextUnformatted("(0.00, 0.00)");
                        /// @end Text

                        /// @begin Text
                        ImGui::TextUnformatted("(0.00, 0.00)");
                        /// @end Text

                        /// @begin Text
                        ImGui::TextUnformatted("(0.00, 0.00)");
                        /// @end Text


                        /// @separator
                        ImGui::EndTable();
                    }
                    /// @end Table

                    /// @begin Text
                    ImRad::Spacing(2);
                    ImGui::TextUnformatted("Foothold (0)");
                    /// @end Text

                    /// @begin Text
                    ImGui::Indent(2 * ImGui::GetStyle().IndentSpacing / 2);
                    ImGui::TextUnformatted("Slope:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("0.00");
                    /// @end Text

                    /// @begin Text
                    ImGui::TextUnformatted("Layer:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("0");
                    /// @end Text

                    /// @begin Text
                    ImGui::TextUnformatted("Ground below Y:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("0.00");
                    /// @end Text

                    /// @begin Text
                    ImGui::TextUnformatted("Jumping down from FH (0)");
                    /// @end Text

                    /// @begin Text
                    ImGui::TextUnformatted("On ground:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("TRUE");
                    /// @end Text

                    /// @begin Text
                    ImGui::TextUnformatted("Can jump down:");
                    /// @end Text

                    /// @begin Text
                    ImGui::SameLine(0, 1 * ImGui::GetStyle().ItemSpacing.x);
                    ImGui::TextUnformatted("TRUE");
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

                /// @begin CollapsingHeader
                ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
                if (ImGui::CollapsingHeader("Physics"))
                {
                    /// @separator

                    /// @begin Text
                    ImGui::TextUnformatted("Timestep (default: 8)");
                    /// @end Text

                    /// @begin Slider
                    ImGui::SetNextItemWidth(385);
                    ImGui::SliderInt("##value3", &value3, 1, 64, nullptr);
                    /// @end Slider

                    /// @separator
                }
                /// @end CollapsingHeader

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
