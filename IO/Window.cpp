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
#include "Window.h"

#include "UI.h"

#include "../Configuration.h"
#include "../Timer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Windows.h>

namespace ms {
    Window::Window() {
        context = nullptr;
        glwnd = nullptr;
        opacity = 1.0f;
        opcstep = 0.0f;
        width = Constants::Constants::get().get_view_width();
        height = Constants::Constants::get().get_view_height();
    }

    Window::~Window() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(glwnd);
        glfwDestroyWindow(context);
        glfwTerminate();
    }

    void error_callback(int no, const char* description) {
        LOG(LOG_ERROR, "GLFW error [" << no << "]: " << description);
    }

    void key_callback(GLFWwindow*, int key, int, int action, int) {
        if (ImGui::GetIO().WantCaptureKeyboard) {
            return;
        }

        UI::get().send_key(key, action != GLFW_RELEASE);
    }

    std::chrono::time_point<std::chrono::steady_clock> start = ContinuousTimer::get().start();

    void mousekey_callback(GLFWwindow*, int button, int action, int) {
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }

        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT: {
            switch (action) {
            case GLFW_PRESS: {
                UI::get().send_cursor(true);
                break;
            }
            case GLFW_RELEASE: {
                auto diff_ms = ContinuousTimer::get().stop(start) / 1000;

                start = ContinuousTimer::get().start();

                if (diff_ms > 10 && diff_ms < 200)
                    UI::get().doubleclick();

                UI::get().send_cursor(false);
                break;
            }
            }

            break;
        }
        case GLFW_MOUSE_BUTTON_RIGHT: {
            switch (action) {
            case GLFW_PRESS:
                UI::get().rightclick();
                break;
            }

            break;
        }
        }
    }

    void cursor_callback(GLFWwindow*, double xpos, double ypos) {
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }

        auto cursor_position = Point<int16_t>(
            static_cast<int16_t>(xpos),
            static_cast<int16_t>(ypos)
        );

        auto screen = Point<int16_t>(
            Constants::Constants::get().get_view_width(),
            Constants::Constants::get().get_view_height()
        );

        if (cursor_position.x() > 0 && cursor_position.y() > 0)
            if (cursor_position.x() < screen.x() && cursor_position.y() < screen.y())
                UI::get().send_cursor(cursor_position);
    }

    void focus_callback(GLFWwindow*, int focused) {
        UI::get().send_focus(focused);
    }

    void scroll_callback(GLFWwindow*, double xoffset, double yoffset) {
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }

        UI::get().send_scroll(yoffset);
    }

    void close_callback(GLFWwindow* window) {
        UI::get().send_close();

        glfwSetWindowShouldClose(window, GL_FALSE);
    }

    Error Window::init() {
        fullscreen = Setting<Fullscreen>::get().load();

        if (!glfwInit()) return Error::Code::GLFW;

        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        context = glfwCreateWindow(1, 1, "", nullptr, nullptr);
        glfwMakeContextCurrent(context);
        glfwSetErrorCallback(error_callback);
        glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        if (Error error = GraphicsGL::get().init()) return error;
        if (Error error = init_window()) return error;
//#ifdef NDEBUG
        if (Error error = init_imgui()) return error;
//#endif

        // Callbacks are installed after ImGui because ImGui::GetIO() will not be ready
        return init_window_callbacks();
    }

    Error Window::init_window() {
        if (glwnd) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            glfwDestroyWindow(glwnd);
        }

        glwnd = glfwCreateWindow(
            width,
            height,
            Configuration::get().get_title().c_str(),
            fullscreen ? glfwGetPrimaryMonitor() : nullptr,
            context
        );

        if (!glwnd) return Error::Code::WINDOW;

        glfwMakeContextCurrent(glwnd);

        bool vsync = Setting<VSync>::get().load();
        glfwSwapInterval(vsync ? 1 : 0);

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        char buf[256];
        GetCurrentDirectoryA(256, buf);
        strcat_s(buf, sizeof(buf), "\\Icon.png");

        GLFWimage images[1];

        auto stbi = stbi_load(buf, &images[0].width, &images[0].height, nullptr, 4);

        if (stbi == nullptr)
            return Error(Error::Code::MISSING_ICON, stbi_failure_reason());

        images[0].pixels = stbi;

        glfwSetWindowIcon(glwnd, 1, images);
        stbi_image_free(images[0].pixels);

        GraphicsGL::get().reinit();

        return Error::Code::NONE;
    }

    Error Window::init_imgui() {
        const char* glsl_version = "#version 150";

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
//        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.MouseDrawCursor = true;
        io.ConfigViewportsNoAutoMerge = true;

        ImGui::StyleColorsDark();

        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(glwnd, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        const std::string FONT_NORMAL = Setting<FontPathNormal>().get().load();
        const std::string FONT_BOLD = Setting<FontPathBold>().get().load();

        if (FONT_NORMAL.empty() || FONT_BOLD.empty())
            return Error::Code::FONT_PATH;

        const char* FONT_NORMAL_STR = FONT_NORMAL.c_str();
        const char* FONT_BOLD_STR = FONT_BOLD.c_str();

        io.Fonts->AddFontFromFileTTF(FONT_NORMAL_STR, 14.0f);
        io.Fonts->AddFontFromFileTTF(FONT_BOLD_STR, 14.0f);

        return Error::NONE;
    }

    Error Window::init_window_callbacks() {
        glfwSetInputMode(glwnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        double xpos, ypos;

        glfwGetCursorPos(glwnd, &xpos, &ypos);
        cursor_callback(glwnd, xpos, ypos);

        glfwSetInputMode(glwnd, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetKeyCallback(glwnd, key_callback);
        glfwSetMouseButtonCallback(glwnd, mousekey_callback);
        glfwSetCursorPosCallback(glwnd, cursor_callback);
        glfwSetWindowFocusCallback(glwnd, focus_callback);
        glfwSetScrollCallback(glwnd, scroll_callback);
        glfwSetWindowCloseCallback(glwnd, close_callback);

        return Error::Code::NONE;
    }

    bool Window::not_closed() const {
        return glfwWindowShouldClose(glwnd) == 0;
    }

    void Window::update() {
        updateopc();
    }

    void Window::updateopc() {
        if (opcstep != 0.0f) {
            opacity += opcstep;

            if (opacity >= 1.0f) {
                opacity = 1.0f;
                opcstep = 0.0f;
            } else if (opacity <= 0.0f) {
                opacity = 0.0f;
                opcstep = -opcstep;

                fadeprocedure();
            }
        }
    }

    void Window::reset_windows() {
        // ImGui has overriden some of these, so reset
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        init_window();
        init_imgui();
        init_window_callbacks();
    }

    void Window::check_events() {
        int16_t max_width = Configuration::get().get_max_width();
        int16_t max_height = Configuration::get().get_max_height();
        int16_t new_width = Constants::Constants::get().get_view_width();
        int16_t new_height = Constants::Constants::get().get_view_height();

        if (width != new_width || height != new_height) {
            width = new_width;
            height = new_height;

            if (new_width >= max_width || new_height >= max_height)
                fullscreen = true;

            reset_windows();
        }

        glfwPollEvents();
    }

    void Window::begin() const {
        GraphicsGL::get().clearscene();
    }

    void Window::end() const {
        GraphicsGL::get().flush(opacity);

        glfwSwapBuffers(glwnd);
    }

    void Window::fadeout(float step, std::function<void()> fadeproc) {
        opcstep = -step;
        fadeprocedure = fadeproc;
    }

    void Window::setclipboard(const std::string& text) const {
        glfwSetClipboardString(glwnd, text.c_str());
    }

    std::string Window::getclipboard() const {
        const char* text = glfwGetClipboardString(glwnd);

        return text ? text : "";
    }

    void Window::toggle_fullscreen() {
        int16_t max_width = Configuration::get().get_max_width();
        int16_t max_height = Configuration::get().get_max_height();

        if (width < max_width && height < max_height) {
            fullscreen = !fullscreen;
            Setting<Fullscreen>::get().save(fullscreen);

            reset_windows();
            glfwPollEvents();
        }
    }
}
