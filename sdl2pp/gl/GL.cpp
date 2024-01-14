#include "GL.hpp"

#include <SDL_video.h>

#include "sdl2pp/utils/Assert.h"
#include "sdl2pp/common/SDLLog.h"

namespace sdlpp::gl {

bool Init(int major, int minor) {
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major) != 0) {
        LOG_ERR(log::LIB, "SDL GL Set Context major version error {}", SDL_GetError());
        return false;
    }

    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor) != 0) {
        LOG_ERR(log::LIB, "SDL GL Set Context minor version error {}", SDL_GetError());
        return false;
    }

    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
        LOG_ERR(log::LIB, "SDL GL Set Context profile mask error {}", SDL_GetError());
        return false;
    }

    return true;
}

bool SetDoubleBuffer(bool v) { return SDL_GL_SetAttribute(SDL_GLattr::SDL_GL_DOUBLEBUFFER, v ? 1 : 0) == 0; }

int GetSwapInterval() { return SDL_GL_GetSwapInterval(); }

bool SetSwapInterval(int v) { return SDL_GL_SetSwapInterval(v) == 0; }

static SDL_GLContext context{nullptr};

bool CreateContext(SDL_Window *window) {
    Assert(context == nullptr, "context has already created.");

    context = SDL_GL_CreateContext(window);
    return context != nullptr;
}

bool MakeCurrent(SDL_Window *window) {
    Assert(context != nullptr, "context has not initialed.");
    return SDL_GL_MakeCurrent(window, context) == 0;
}

SDL_Window *GetCurrentWindow() { return SDL_GL_GetCurrentWindow(); }

void DestroyContext() {
    if(context)
        SDL_GL_DeleteContext(context);
}

static std::once_flag glad_init_flag;

namespace ext {

void Init() {
    std::call_once(glad_init_flag, []() -> void {
        // 初始化GLAD
        Assert(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) != 0, "gladLoadGLLoader false.");
    });
}

void DepthTest() { glEnable(GL_DEPTH_TEST); }

} // namespace ext

} // namespace sdlpp::gl