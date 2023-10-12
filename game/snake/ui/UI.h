#pragma once
#ifndef SDL2PP_GAME_SNAKE_UI_UI_H_
#define SDL2PP_GAME_SNAKE_UI_UI_H_

#include <memory>

#include "sdl2pp/SDLpp.h"

namespace ui {

const std::string_view TITLE        = "snake";
const std::string_view HELP_INFO[3] = {
        " w              up      ",
        "a     d    left    right",
        "   s           down     ",
};

class UI {
public:
    static UI *GetInstance() {
        static UI instance;
        return &instance;
    }

    ~UI()                     = default;
    UI(const UI &)            = delete;
    UI &operator=(const UI &) = delete;

    bool Init(sdlpp::SWindow *window);
    void Clear();

    void Render(std::shared_ptr<sdlpp::SRenderer> renderer);
    void Resize(int new_w, int new_h);

private:
    bool InitTitle(std::shared_ptr<sdlpp::SRenderer> renderer);
    bool InitHelpInfo(std::shared_ptr<sdlpp::SRenderer> renderer);

    void render(std::shared_ptr<sdlpp::SRenderer> renderer);

private:
    UI() = default;

    sdlpp::RectI render_view_{};
    int          distance_ = 12;

    std::shared_ptr<sdlpp::STexture>              title_;
    std::vector<std::shared_ptr<sdlpp::STexture>> help_info_;
};

} // namespace ui

#endif // SDL2PP_GAME_SNAKE_UI_UI_H_
