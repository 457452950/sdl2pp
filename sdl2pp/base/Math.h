#pragma once
#ifndef SDL2PP_BASE_MATH_H_
#define SDL2PP_BASE_MATH_H_

#include <cmath>

#include <SDL_stdinc.h>

namespace sdlpp {

#define PI_L (3.1415926f)
#define PI_H (3.1415926535897932384626f)

#define MIN_COMPARE_DOUBLE (0.0001f)

// compare two floats
inline int Compare(float a, float b) { return std::fabs(a - b) < MIN_COMPARE_DOUBLE ? 0 : (a - b > 0.0f ? 1 : -1); }


} // namespace sdlpp

#endif // SDL2PP_BASE_MATH_H_
