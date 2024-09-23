#pragma once

#include "../Constants.h"
#include <cstdint>
#include <vector>
#include <memory>
#include <chrono>

namespace ms {
    class Timer {
    public:
        /**
         * Creates a timer, initially with a delay of 0 (inactive).
         */
        Timer();

        /**
         * Creates a timer which will return true after millis.
         */
        explicit Timer(int64_t millis);

        /**
         * Change the delay of the timer.
         * @param millis The amount of milliseconds after which the timer will return true
         */
        void set_duration(int64_t millis);

        Timer& start();
        void stop();
        void reset();
//        void update();
        bool is_active() const;
        bool is_ready() const;

    private:
        using clock = std::chrono::steady_clock;

        clock::time_point m_start;
        clock::duration m_duration{};
//        bool m_ready;
        bool m_active;
    };

    class TimerManager : public Singleton<TimerManager> {
    public:
        Timer& create_timer(int64_t millis);
//        void update();

    private:
        std::vector<std::unique_ptr<Timer>> timers;
    };
}
