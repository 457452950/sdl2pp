#pragma once
#ifndef SDL2PP_SDL2PP_GLWINDOW_HPP_
#define SDL2PP_SDL2PP_GLWINDOW_HPP_

#include "SWindow.hpp"
#include "sdl2pp/gl/GL.hpp"

namespace sdlpp {

class GlWindow : public sdlpp::SWindow {
public:
    GlWindow();

    ~GlWindow() override = default;

    void RenderFlush() override;

private:
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_GLWINDOW_HPP_
