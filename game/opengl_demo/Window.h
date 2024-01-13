#pragma once
#ifndef SDL2PP_GAME_WINDOW_H_
#define SDL2PP_GAME_WINDOW_H_

#include "glad/glad.h"

#include "sdl2pp/SDLpp.h"
#include "sdl2pp/STexture.hpp"

namespace game {

class Window : public sdlpp::SWindow {
public:
    Window();

    ~Window() override;

    int KeyEvent(const SDL_KeyboardEvent &event) override { return SWindow::KeyEvent(event); }

    int WindowEvent(const SDL_WindowEvent &event) override { return SWindow::WindowEvent(event); }

    int MouseButtonEvent(const SDL_MouseButtonEvent &event) override { return SWindow::MouseButtonEvent(event); }

    int MouseMoveEvent(const SDL_MouseMotionEvent &event) override { return SWindow::MouseMoveEvent(event); }

    int MouseWheelEvent(const SDL_MouseWheelEvent &event) override { return SWindow::MouseWheelEvent(event); }

    void RenderClear() override;
    void RenderProcess() override;
    void RenderFlush() override;

    void Tick(double tick_ms) override;

private:
    SDL_GLContext gl_context_{nullptr};
};

} // namespace game

#endif // SDL2PP_GAME_WINDOW_H_
