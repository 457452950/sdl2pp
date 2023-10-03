#pragma once
#ifndef SDL2PP_GAME_SNAKE_TOOLS_RESOURCE_H_
#define SDL2PP_GAME_SNAKE_TOOLS_RESOURCE_H_

#include <string>

#include "sdl2pp/SDLpp.h"

namespace game {

static const std::string base_path = R"(H:\Resources\image\)";

class Resource {
public:
    static std::shared_ptr<sdlpp::SDLSurface> LoadFile(const std::string &file_name);
};

} // namespace game

#endif // SDL2PP_GAME_SNAKE_TOOLS_RESOURCE_H_
