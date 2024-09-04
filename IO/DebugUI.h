#pragma once

#include "../Template/Singleton.h"

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"

namespace ms {
    class DebugUI : public Singleton<DebugUI> {
    public:
        DebugUI();

        void init();
        void update();
        void draw();

        void show();
        void hide();
    private:
        bool is_shown;

        void _draw();
    };
}
