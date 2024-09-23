#include "Timer.h"

namespace ms {
    /* Timer */
    Timer::Timer() : Timer(0) {
    }

    Timer::Timer(int64_t millis) {
        m_start = clock::now();
//        m_ready = false;
        m_duration = std::chrono::milliseconds(millis);
        m_active = false;
    }

    void Timer::set_duration(int64_t millis) {
        m_duration = std::chrono::milliseconds(millis);
    }

    Timer& Timer::start() {
        m_active = true;
        return *this;
    }

    void Timer::stop() {
        m_active = false;
    }

    void Timer::reset() {
        m_start = clock::now();
    }

//    void Timer::update() {
//        if (m_active) {
//            auto now = clock::now();
//            auto d = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start);
//            m_ready = d > m_duration;
//        }
//    }

    bool Timer::is_active() const {
        return m_active;
    }

    bool Timer::is_ready() const {
        if (m_active) {
            auto now = clock::now();
            auto d = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start);
            return d > m_duration;
        }
        return false;
    }

    /* TimerManager */
    Timer& TimerManager::create_timer(int64_t millis) {
        auto timer = std::make_unique<Timer>(millis);
        timers.emplace_back(std::move(timer));
        return *timers.back();
    }

//    void TimerManager::update() {
//        for (auto &timer : timers) {
//            timer->update();
//        }
//    }
}
