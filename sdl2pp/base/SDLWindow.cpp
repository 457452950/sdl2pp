#include "SDLWindow.h"

#include <memory>

namespace sdlpp {

    std::shared_ptr<SDLWindow>
    SDLWindow::Create(const std::string &title, SDL_Point pos, SDL_Point size, Uint32 flags = 0) {
        return std::make_shared<SDLWindow>(
                SDL_CreateWindow(title.c_str(), pos.x, pos.y, size.x, size.y, flags)
        );
    }

    std::shared_ptr<SDLWindow> SDLWindow::Create(const std::string &title, SDL_Point size, Uint32 flags = 0) {
        return std::make_shared<SDLWindow>(

                SDL_CreateWindow(title.c_str(),
                                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 size.x, size.y,
                                 flags)

        );
    }

}