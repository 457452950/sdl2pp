#pragma once
#ifndef SDL2PP_SDL2PP_COMMON_TIME_HPP_
#define SDL2PP_SDL2PP_COMMON_TIME_HPP_

#include <chrono>
#include <functional>
#include <utility>

#include <SDL_timer.h>
#include <SDL_assert.h>

#define TIME_USE_CHRONO
// #define TIME_USE_SDL


namespace sdlpp {

using namespace std::chrono;
using namespace std::chrono_literals;

inline uint64_t GetTick_ms() { return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); }

inline uint64_t GetPerformanceCounter() {
#ifdef TIME_USE_CHRONO
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
#elif defined(TIME_USE_SDL)
    return SDL_GetPerformanceCounter();
#endif
}
inline uint64_t GetPerformanceFrequency() {
#ifdef TIME_USE_CHRONO
    return 1000'000;
#elif defined(TIME_USE_SDL)
    return SDL_GetPerformanceFrequency();
#endif
}

using TimerId = SDL_TimerID;

class Timer {
public:
    Timer() = default;
    ~Timer() { Stop(); }

    Timer(const Timer &)            = delete;
    Timer &operator=(const Timer &) = delete;

    Timer(Timer &&other)            = default;
    Timer &operator=(Timer &&other) = default;

    using TimeoutCallback = std::function<void()>;

    bool Start(TimeoutCallback cb, milliseconds interval_ms, uint32_t times) {
        SDL_assert(id_ == 0);

        timeout_times_ = times;
        call_back_     = std::move(cb);

        id_ = SDL_AddTimer(interval_ms.count(), &Timer::timerCallback, this);

        return id_ != 0;
    }

    void Stop() {
        if(id_ != 0) {
            SDL_RemoveTimer(id_);
            id_ = 0;
        }
    }


private:
    static Uint32 timerCallback(uint32_t interval, void *param) {
        auto *timer = static_cast<Timer *>(param);
        if(timer->timeout_times_ == 0) {
            return 0;
        }

        if(timer->call_back_) {
            timer->call_back_();
        }

        --timer->timeout_times_;
        return (!!timer->timeout_times_) ? interval : 0;
    }

    TimeoutCallback call_back_;

    TimerId  id_{0};
    uint32_t timeout_times_{0};
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_COMMON_TIME_HPP_
