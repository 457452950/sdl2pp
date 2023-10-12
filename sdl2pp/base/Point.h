#pragma once
#ifndef SDL2PP_SDL2PP_BASE_POINT_H_
#define SDL2PP_SDL2PP_BASE_POINT_H_

#include <SDL_rect.h>

#include "Math.h"

namespace sdlpp {

struct PointI : public SDL_Point {
    PointI() : SDL_Point{0, 0} {}
    PointI(int _x, int _y) : SDL_Point{_x, _y} {}

    PointI(const PointI &p) = default;
    PointI(PointI &&p)      = default;

    PointI &operator=(const PointI &p) = default;
    PointI &operator=(PointI &&p)      = default;

    bool operator==(const PointI &p) const { return this->x == p.x && this->y == p.y; }
    bool operator!=(const PointI &p) const { return this->x != p.x || this->y != p.y; }

    PointI operator+(const PointI &p) const { return {this->x + p.x, this->y + p.y}; };
    PointI operator-(const PointI &p) const { return {this->x - p.x, this->y - p.y}; }

    PointI &operator+=(const PointI &p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }
    PointI &operator-=(const PointI &p) {
        this->x -= p.x;
        this->y -= p.y;
        return *this;
    }

    PointI  operator*(int t) const { return {this->x * t, this->y * t}; };
    PointI &operator*=(int t) {
        this->x *= t;
        this->y *= t;
        return *this;
    }
};

struct PointF : public SDL_FPoint {
    PointF() : SDL_FPoint({0, 0}) {}
    PointF(float _x, float _y) : SDL_FPoint({_x, _y}) {}

    PointF(const PointF &p) = default;
    PointF(PointF &&p)      = default;

    PointF &operator=(const PointF &p) = default;
    PointF &operator=(PointF &&p)      = default;

    bool operator==(const PointF &p) const { return Compare(this->x, p.x) == 0 && Compare(this->y, p.y) == 0; }
    bool operator!=(const PointF &p) const { return Compare(this->x, p.x) != 0 || Compare(this->y, p.y) != 0; }

    PointF operator+(const PointF &p) const { return {this->x + p.x, this->y + p.y}; }
    PointF operator-(const PointF &p) const { return {this->x - p.x, this->y - p.y}; }

    PointF &operator+=(const PointF &p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }
    PointF &operator-=(const PointF &p) {
        this->x -= p.x;
        this->y -= p.y;
        return *this;
    }

    PointF  operator*(float t) const { return {this->x * t, this->y * t}; };
    PointF &operator*=(float t) {
        this->x *= t;
        this->y *= t;
        return *this;
    }
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_BASE_POINT_H_
