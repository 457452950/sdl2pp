#pragma once
#ifndef SDL2PP_SDL2PP_GLWINDOW_HPP_
#define SDL2PP_SDL2PP_GLWINDOW_HPP_

#include "SWindow.hpp"
#include "sdl2pp/gl/GL.hpp"

namespace sdlpp {

class GlWindow : public SWindow {
public:
    GlWindow();

    ~GlWindow() override = default;

    void SetSize(PointI size) {
        glViewport(0, 0, size.x, size.y);
        SWindow::SetSize(size);
    }


    int WindowEvent(const SDL_WindowEvent &event) override {
        switch(event.event) {
        case SDL_WINDOWEVENT_RESIZED: {
            glViewport(0, 0, event.data1, event.data2);
        } break;
        }
        return SWindow::WindowEvent(event);
    }

    void RenderFlush() override;

private:
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_GLWINDOW_HPP_
