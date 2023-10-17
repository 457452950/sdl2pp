#pragma once
#ifndef SDL2PP_BASE_MATH_H_
#define SDL2PP_BASE_MATH_H_

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <cfloat>

#include <SDL_stdinc.h>

namespace sdlpp {

/**
 * compare two floats
 * @param a
 * @param b
 * @return 1 for a > b, 0 for a == b, -1 for a < b
 */
inline int Compare(float a, float b) { return std::fabs(a - b) < FLT_EPSILON ? 0 : (a > b ? 1 : -1); }
inline int Compare(double a, double b) { return std::fabs(a - b) < DBL_EPSILON ? 0 : (a > b ? 1 : -1); }

inline double Atan(double y, double x) {
#ifdef WIN32
    return std::atan2f(y, x);
#elif defined(__LINUX__)
    if(position_.x != 0) {
        return std::atan(position_.y / position_.x);
    } else {
        return M_PI / 2;
    }
#endif
}

} // namespace sdlpp

#endif // SDL2PP_BASE_MATH_H_
