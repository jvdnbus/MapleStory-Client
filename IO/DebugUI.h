#pragma once

#include <unordered_map>

#include "../Template/Singleton.h"

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"

namespace ms {
    class variant {
        union {
            bool b;
            int i;
            std::string s;
            int64_t i64;
        };
        enum class type {
            boolean, integer, string, signed_int_64
        };
        type t_;

    public:
        // Constructor is only used for unordered_map
        variant() {}
        variant(bool b) : t_(type::boolean), b(b) {}
        variant(int i) : t_(type::integer), i(i) {}
        variant(std::string s) : t_(type::string), s(s) {}
        variant(int64_t i) : t_(type::signed_int_64), i64(i) {}

        ~variant() {
        }

        bool get_bool() const {
            return b;
        }

        int get_int() const {
            return i;
        }

        std::string get_string() const {
            return s;
        }

        int64_t get_int64() const {
            return i64;
        }

        variant(const variant& src) {
            t_ = src.t_;
            switch (t_) {
                case type::boolean:
                    this->b = src.b;
                    break;
                case type::integer:
                    this->i = src.i;
                    break;
                case type::string:
                    this->s = src.s;
                    break;
                case type::signed_int_64:
                    this->i64 = src.i64;
                    break;
            }
        }

        // move assignment
        variant& operator=(const variant& other) noexcept {
            if (this != &other) {
                t_ = other.t_;
                switch (t_) {
                    case type::boolean:
                        b = other.b;
                        break;
                    case type::integer:
                        i = other.i;
                        break;
                    case type::string:
                        s = other.s;
                        break;
                    case type::signed_int_64:
                        i64 = other.i64;
                        break;
                }
            }
            return *this;
        }
    };

    class DebugValue {
    public:
        static const DebugValue FPS;

    private:
        std::string str;

        DebugValue(std::string str) : str(str) {}

    public:
        std::string to_string() const {
            return str;
        }

        bool operator==(const DebugValue &other) const {
            return str == other.str;
        }
    };

    struct DebugValueHash {
        std::size_t operator()(const DebugValue &key) const {
            return std::hash<std::string>{}(key.to_string());
        }
    };

    class DebugUI : public Singleton<DebugUI> {
    public:
        DebugUI();

        void init();
        void update(const DebugValue& dv, const variant& val) {
            _state[dv] = val;
        }
        void draw();
        void show();
        void hide();

        bool is_shown() const;
    private:
        bool _is_shown;

        using state_mapping = std::unordered_map<
            DebugValue,
            variant,
            DebugValueHash
        >;
        state_mapping _state;

        variant& get_value(const DebugValue &dv) {
            return _state[dv];
        }

        void _draw();
    };
}
