#include "SDLPixels.h"

namespace sdlpp {
std::shared_ptr<SDL_PixelFormat> MakeShared(SDL_PixelFormat *&&pf) {
    return std::shared_ptr<SDL_PixelFormat>(pf, SDL_FreeFormat);
}

std::unique_ptr<SDL_PixelFormat, decltype(&SDL_FreeFormat)> MakeUnique(SDL_PixelFormat *&&pf) {
    return std::unique_ptr<SDL_PixelFormat, decltype(&SDL_FreeFormat)>(pf, SDL_FreeFormat);
}
} // namespace sdlpp