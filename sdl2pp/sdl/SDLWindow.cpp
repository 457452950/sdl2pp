#include "SDLWindow.hpp"

#include "SDLRenderer.hpp"
#include "sdl2pp/utils/Assert.h"

namespace sdlpp {

std::shared_ptr<SDLRenderer> SDLWindow::CreateRender(uint32_t flags, int index) {
    return SDLRenderer::Create(SDL_CreateRenderer(this->Get(), index, flags));
}

SDLWindow::SDLWindow(PointI size, std::string_view title, PointI pos, uint32_t flags) {
    window_ = SDL_CreateWindow(title.data(), pos.x, pos.y, size.x, size.y, flags);
    Assert(window_, "SDL_CreateWindow error: {}", SDL_GetError());
}

} // namespace sdlpp