#pragma once
#ifndef SDL2_BASE_SDLENVIRONMENT_H
#define SDL2_BASE_SDLENVIRONMENT_H

#include <mutex>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "SDLDisplay.h"

namespace sdlpp {

class SDL {
public:
    enum INIT_SUB_SYSTEM : uint32_t {
        TIMER           = SDL_INIT_TIMER,
        AUDIO           = SDL_INIT_AUDIO,
        VIDEO           = SDL_INIT_VIDEO,    /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
        JOYSTICK        = SDL_INIT_JOYSTICK, /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
        HAPTIC          = SDL_INIT_HAPTIC,
        GAME_CONTROLLER = SDL_INIT_GAMECONTROLLER, /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
        EVENTS          = SDL_INIT_EVENTS,
        SENSOR          = SDL_INIT_SENSOR,
        EVERY_THING     = SDL_INIT_EVERYTHING,
    };

    enum INIT_IMG_FLAGS : int32_t {
        JPG  = IMG_INIT_JPG,
        PNG  = IMG_INIT_PNG,
        TIF  = IMG_INIT_TIF,
        WEBP = IMG_INIT_WEBP,
        JXL  = IMG_INIT_JXL,
        AVIF = IMG_INIT_AVIF,
    };

    enum INIT_MIX_FLAGS : int32_t {
        FLAC = MIX_INIT_FLAC,
        MOD  = MIX_INIT_MOD,
        MP3  = MIX_INIT_MP3,
        OGG  = MIX_INIT_OGG,
        MID  = MIX_INIT_MID,
        OPUS = MIX_INIT_OPUS,
    };

    // threat safety
    static bool Init(uint32_t sdl_flags);

    static SDL *GetInstance() { return instance_; }

    // not thread safe
    bool InitImg(int32_t img_flags);

    bool InitTTF() {
        ttf_initialized = true;
        return TTF_Init() == 0;
    }

    bool InitMixer(int flags);

    ~SDL();

private:
    explicit SDL() = default;

    static std::mutex mutex_;
    static SDL       *instance_;

    static bool sdlInit(uint32_t sdl_flags) { return SDL_Init(sdl_flags) == 0; }


    bool img_initialized = false;
    bool ttf_initialized = false;
    bool mix_initialized = false;
};
} // namespace sdlpp

#endif // SDL2_BASE_SDLENVIRONMENT_H
