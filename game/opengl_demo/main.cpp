#include "sdl2pp/SDLpp.h"

#include "Window.h"

int main(int argc, char *argv[]) {
    // init system
    auto ok = sdlpp::SDL::Init(sdlpp::SDL::EVERY_THING);
    Assert(ok, "Failed to init SDL {}", SDL_GetError());

    ok = sdlpp::SDL::GetInstance()->InitImg(sdlpp::SDL::PNG | sdlpp::SDL::JPG);
    Assert(ok, "Failed to init SDL_image {}", SDL_GetError());

    ok = sdlpp::SDL::GetInstance()->InitTTF();
    Assert(ok, "Failed to init SDL_ttf {}", SDL_GetError());

    ok = sdlpp::SDL::GetInstance()->InitMixer(sdlpp::SDL::OGG);
    Assert(ok, "Failed to init SDL_mixer {}", SDL_GetError());

    // open audio
    sdlpp::SDLMixer::GetInstance()->OpenAudio();

    // set log level1
#ifdef _DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
#endif

    ok = sdlpp::gl::Init(3, 3);
    Assert(ok, "Failed to init opengl {}", SDL_GetError());

    game::Window window;

    ok = sdlpp::gl::SetDoubleBuffer(true);
    Assert(ok, "Failed to set double buffer {}", SDL_GetError());

    switch(sdlpp::gl::GetSwapInterval()) {
    case 1: {
        LOG_INF(log::APP, "1 {}", SDL_GetError());
        break;
    }
    case 0: {
        LOG_INF(log::APP, "0 {}", SDL_GetError());
        break;
    }
    case -1: {
        LOG_INF(log::APP, "-1 {}", SDL_GetError());
        break;
    }
    }

    ok = sdlpp::gl::SetSwapInterval(0);
    Assert(ok, "failed to set vsync {}", SDL_GetError());

    return window.Exec();
}
