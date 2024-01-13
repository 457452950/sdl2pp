#include "SDLWindow.h"

#include "SDLRenderer.hpp"
#include "sdl2pp/common/SDLLog.h"

namespace sdlpp {

std::shared_ptr<SDLRenderer> SDLWindow::CreateRender(uint32_t flags, int index) {
    return SDLRenderer::Create(SDL_CreateRenderer(this->Get(), index, flags));
}

SDLWindow::SDLWindow(PointI size, std::string_view title, PointI pos, uint32_t flags) {
    window_ = SDL_CreateWindow(title.data(), pos.x, pos.y, size.x, size.y, flags);

    if(!window_) {
        LOG_ERR(log::LIB, "SDL_CreateWindow error: {}", SDL_GetError());
        SDL_assert(window_);
    }
}
} // namespace sdlpp