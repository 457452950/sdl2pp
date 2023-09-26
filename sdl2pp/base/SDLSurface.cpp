#include "SDLSurface.h"

#include <SDL.h>

namespace sdlpp {
std::shared_ptr<SDL_Surface> MakeShared(SDL_Surface *&&surface) { return {surface, SDL_FreeSurface}; }

auto MakeUnique(SDL_Surface *&&surface) -> std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> {
    return std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>{surface, SDL_FreeSurface};
}

std::shared_ptr<SDLSurface> ConvertSurface(std::shared_ptr<SDLSurface> src, const SDL_PixelFormat &fmt) {
    return SDLSurface::Create(SDL_ConvertSurface(src->Get(), &fmt, 0));
}

std::shared_ptr<SDLSurface> SDLSurface::Copy() {
    if(!surface_) {
        return nullptr;
    }
    return Create(SDL_DuplicateSurface(surface_));
}

std::shared_ptr<SDLSurface> SDLSurface::Create(SDL_Surface *&&surface) {
    return std::shared_ptr<SDLSurface>(new SDLSurface(std::move(surface)));
}

bool SDLSurface::BlitCopy(std::shared_ptr<SDLSurface> dst, const SDL_Rect *src_rect, SDL_Point dst_point) {
    SDL_Rect dist_rect = {dst_point.x, dst_point.y, 0, 0};
    return SDL_BlitSurface(surface_, src_rect, dst->Get(), &dist_rect) == 0;
}

bool SDLSurface::BlitScaled(std::shared_ptr<SDLSurface> dst, const SDL_Rect *src_rect, SDL_Rect *dst_rect) {
    return SDL_BlitScaled(surface_, src_rect, dst->Get(), dst_rect) == 0;
}

} // namespace sdlpp