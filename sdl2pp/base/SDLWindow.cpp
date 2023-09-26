#include "SDLWindow.h"

#include "SDLRenderer.h"

namespace sdlpp {

std::shared_ptr<SDLRenderer> SDLWindow::CreateRender(uint32_t flags, int index) {
    return SDLRenderer::Create(SDL_CreateRenderer(this->Get(), index, flags));
}

SDLWindow::SDLWindow(SDL_Point size, std::string_view title, SDL_Point pos, uint32_t flags) {
    window_ = SDL_CreateWindow(title.data(), pos.x, pos.y, size.x, size.y, flags);

    if(!window_) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        SDL_assert(window_);
    }
}
} // namespace sdlpp