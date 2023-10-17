#pragma once
#ifndef SDL2PP_GAME_PONG_BALL_H_
#define SDL2PP_GAME_PONG_BALL_H_

#include "sdl2pp/SDLpp.h"

namespace pong {

using namespace sdlpp;

class Ball {
public:
private:
    PointI position_;
    Circle collision_;
};

} // namespace pong

#endif // SDL2PP_GAME_PONG_BALL_H_
