#include "SDLPixels.h"

namespace sdlpp {
std::shared_ptr<SDL_PixelFormat> MakeShared(SDL_PixelFormat *&&pf) {
    return std::shared_ptr<SDL_PixelFormat>(pf, SDL_FreeFormat);
}

std::unique_ptr<SDL_PixelFormat, decltype(&SDL_FreeFormat)> MakeUnique(SDL_PixelFormat *&&pf) {
    return std::unique_ptr<SDL_PixelFormat, decltype(&SDL_FreeFormat)>(pf, SDL_FreeFormat);
}

ColorData MapRGB(const SDL_PixelFormat *pf, SDL_Color color) { return SDL_MapRGB(pf, color.r, color.g, color.b); }

ColorData MapRGBA(const SDL_PixelFormat *pf, SDL_Color color) {
    return SDL_MapRGBA(pf, color.r, color.g, color.b, color.a);
}

SDL_Color GetRGB(ColorData pixel, const SDL_PixelFormat *pf) {
    SDL_Color res{};
    SDL_GetRGB(pixel, pf, &res.r, &res.g, &res.b);
    return res;
}

SDL_Color GetRGBA(ColorData pixel, const SDL_PixelFormat *pf) {
    SDL_Color res{};
    SDL_GetRGBA(pixel, pf, &res.r, &res.g, &res.b, &res.a);
    return res;
}


} // namespace sdlpp