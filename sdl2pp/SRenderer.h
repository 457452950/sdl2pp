#pragma once
#ifndef SDL2PP_SRENDERER_H_
#define SDL2PP_SRENDERER_H_

#include "sdl2pp/sdl/SDLRenderer.h"

namespace sdlpp {

class SRenderer : public SDLRenderer {
public:
    static std::shared_ptr<SRenderer> Create(SDL_Renderer *&&renderer) {
        return std::shared_ptr<SRenderer>(new SRenderer(std::move(renderer)));
    }


protected:
    explicit SRenderer(SDL_Renderer *&&renderer) : SDLRenderer(std::move(renderer)) {}


private:
};

} // namespace sdlpp

#endif // SDL2PP_SRENDERER_H_
