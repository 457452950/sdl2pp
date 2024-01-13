#include "SDLEnvironment.hpp"

namespace sdlpp {
inline SDL       *SDL::instance_ = nullptr;
inline std::mutex SDL::mutex_;

SDL::~SDL() {
    SDL_Quit();

    if(img_initialized) {
        IMG_Quit();
    }
    if(ttf_initialized) {
        TTF_Quit();
    }

    if(mix_initialized) {
        Mix_Quit();
    }
};


bool SDL::Init(uint32_t sdl_flags) {
    if(instance_ == nullptr) {
        std::unique_lock<std::mutex> uniqueLock(mutex_);

        if(instance_ == nullptr) {
            instance_ = new SDL;
        }
    }

    return sdlpp::SDL::sdlInit(sdl_flags);
}

bool SDL::InitImg(int32_t img_flags) {
    assert(img_flags != 0);

    img_initialized = true;
    return IMG_Init(img_flags) == img_flags;
}
bool SDL::InitMixer(int flags) {
    assert(flags);

    mix_initialized = true;

    return Mix_Init(flags) == flags;
}

} // namespace sdlpp
