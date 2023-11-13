#include "sdl2pp/SDLpp.h"

#include "Window.h"

int main(int argc, char *argv[]) {
    // init system
    auto ok = sdlpp::SDL::Init(sdlpp::SDL::EVERY_THING);
    if(!ok) {
        LOG_ERR(log::APP, "Failed to init SDL {}", SDL_GetError());
        return 1;
    }
    ok = sdlpp::SDL::GetInstance()->InitImg(sdlpp::SDL::PNG | sdlpp::SDL::JPG);
    if(!ok) {
        LOG_ERR(log::APP, "Failed to init SDL_image {}", SDL_GetError());
        return 1;
    }
    ok = sdlpp::SDL::GetInstance()->InitTTF();
    if(!ok) {
        LOG_ERR(log::APP, "Failed to init SDL_ttf {}", SDL_GetError());
        return 1;
    }
    ok = sdlpp::SDL::GetInstance()->InitMixer(sdlpp::SDL::OGG);
    if(!ok) {
        LOG_ERR(log::APP, "Failed to init SDL_mixer {}", SDL_GetError());
    }

    // open audio
    sdlpp::SDLMixer::GetInstance()->OpenAudio();

    // set log level1
#ifdef _DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    game::Window window;

    return window.Exec();
}
