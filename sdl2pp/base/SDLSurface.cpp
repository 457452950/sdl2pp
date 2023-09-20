//
// Created by wang on 2023/9/18.
//

#include "SDLSurface.h"

namespace sdlpp {
    std::shared_ptr<SDL_Surface> MakeShared(SDL_Surface *&&surface) {
        return {surface, SDL_FreeSurface};
    }

    auto MakeUnique(SDL_Surface *&&surface) -> std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> {
        return std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>{surface, SDL_FreeSurface};
    }

    std::shared_ptr<SDLSurface> SDLSurface::Copy() {
        if (!surface_) {
            return nullptr;
        }
        return Create(SDL_DuplicateSurface(surface_));
    }

    std::shared_ptr<SDLSurface> SDLSurface::Create(SDL_Surface *&&surface) {
        return std::shared_ptr<SDLSurface>(new SDLSurface(std::move(surface)));
    }
} // sdlpp