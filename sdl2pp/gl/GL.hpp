#pragma once
#ifndef SDL2PP_SDL2PP_GL_GL_HPP_
#define SDL2PP_SDL2PP_GL_GL_HPP_

#include "Toy/NonCopyAble.hpp"

namespace sdlpp::gl {


class GL : public NonCopyAble {
public:
    static bool Init(int major, int minor);

    static bool SetDoubleBuffer(bool v);

    static int GetSwapInterval();

    static bool SetSwapInterval(int v);

    static void GladInit();

    static void DepthTest();
};

} // namespace sdlpp::gl

#endif // SDL2PP_SDL2PP_GL_GL_HPP_
