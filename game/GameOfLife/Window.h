#pragma once
#ifndef SDL2PP_GAME_WINDOW_H_
#define SDL2PP_GAME_WINDOW_H_

// #include <fmt/format.h>

#include "sdl2pp/SDLpp.h"
#include "sdl2pp/STexture.h"
#include "GOLMap.h"

namespace gol {

class Window : public sdlpp::SWindow {
public:
    Window();

    ~Window() override {}

    bool LoadFile(std::string file);

    bool LoadCellsFile(std::string file);


    int KeyEvent(const SDL_KeyboardEvent &event) override {

        if(event.type == SDL_KEYUP) {
            if(event.state == SDL_RELEASED) {

                switch(event.keysym.sym) {
                case SDLK_s:
                    //                    map_.Update();
                default:
                    break;
                }
            }
        }

        return SWindow::KeyEvent(event);
    }

    int WindowEvent(const SDL_WindowEvent &event) override { return SWindow::WindowEvent(event); }

    int MouseButtonEvent(const SDL_MouseButtonEvent &event) override { return SWindow::MouseButtonEvent(event); }

    int MouseMoveEvent(const SDL_MouseMotionEvent &event) override { return SWindow::MouseMoveEvent(event); }

    int MouseWheelEvent(const SDL_MouseWheelEvent &event) override { return SWindow::MouseWheelEvent(event); }

    void RenderProcess(sdlpp::PointF view_pos, double view_angle) override;

    void Tick(double tick_ms) override;

private:
    gol::GOLMap map_;
};

} // namespace gol

#endif // SDL2PP_GAME_WINDOW_H_
