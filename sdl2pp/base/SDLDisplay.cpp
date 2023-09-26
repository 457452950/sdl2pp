#include "SDLDisplay.h"

sdlpp::Display::Display() {
    auto count = SDL_GetNumVideoDisplays();
    for(int i = 0; i < count; ++i) {
        this->display_infos_.emplace_back(i);
    }
}
