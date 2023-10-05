#pragma once
#ifndef SDL2PP_GAME_SNAKE_TOOLS_RESOURCE_H_
#define SDL2PP_GAME_SNAKE_TOOLS_RESOURCE_H_

#include <string>

#include "sdl2pp/SDLpp.h"

namespace snake {

static const std::string base_path  = R"(H:\Resources\)";
static const std::string image_path = R"(image\)";
static const std::string font_path  = R"(ttf\)";

class Resource {
public:
    // 单例
    static Resource *GetInstance() {
        static Resource instance;
        return &instance;
    }

    std::shared_ptr<sdlpp::SDLSurface> GetImage(const std::string &file_name);
    std::shared_ptr<sdlpp::Font>       GetFont(const std::string &file_name, int point_size);

private:
    std::unordered_map<std::string, std::shared_ptr<sdlpp::SDLSurface>> images_map_;
    std::unordered_map<std::string, std::shared_ptr<sdlpp::Font>>       fonts_map_;
};

} // namespace snake

#endif // SDL2PP_GAME_SNAKE_TOOLS_RESOURCE_H_
