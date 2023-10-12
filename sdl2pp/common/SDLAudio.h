#pragma once
#ifndef SDL2PP_BASE_BASE_SDLAUDIO_H_
#define SDL2PP_BASE_BASE_SDLAUDIO_H_

#include <vector>
#include <string>

#include <SDL_log.h>
#include <SDL_audio.h>

#include "SDLLog.h"

namespace sdlpp {

class AudioDrivers {
public:
    static AudioDrivers *GetInstance() {
        static AudioDrivers instance_;
        return &instance_;
    }

    ~AudioDrivers() = default;

    static bool Init(std::string_view driver_name) { return SDL_AudioInit(driver_name.data()) == 0; }
    static void Quit() { SDL_AudioQuit(); }

    static std::string GetCurrentAudioDriver() {
        auto name = SDL_GetCurrentAudioDriver();
        return (name == nullptr) ? "" : name;
    }

    const std::vector<std::string> &GetDrivers() const { return drivers_; }

private:
    AudioDrivers() {
        auto count = SDL_GetNumAudioDrivers();
        drivers_.resize(count);

        for(int index = 0; index < count; ++index) {
            drivers_[index] = SDL_GetAudioDriver(index);
        }

        auto c = SDL_GetNumAudioDevices(1);
        for(int i = 0; i < c; ++i) {
            LOG_INF(log::LIB, "Audio device {}: {}", i, SDL_GetAudioDeviceName(i, 1));
        }
    }

    std::vector<std::string> drivers_;
};


} // namespace sdlpp

#endif // SDL2PP_BASE_BASE_SDLAUDIO_H_
