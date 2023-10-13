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

struct PointD;

struct PointF : public SDL_FPoint {
    PointF() : SDL_FPoint({0, 0}) {}
    PointF(float _x, float _y) : SDL_FPoint({_x, _y}) {}

    PointF(const PointF &p) = default;
    PointF(PointF &&p)      = default;

    explicit PointF(const PointD &);

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


struct PointD {
    double x{0}, y{0};

    PointD() : PointD(0, 0) {}
    PointD(double _x, double _y) : x(_x), y(_y) {}

    PointD(const PointD &p) = default;
    PointD(PointD &&p)      = default;

    PointD &operator=(const PointD &p) = default;
    PointD &operator=(PointD &&p)      = default;

    bool operator==(const PointD &p) const { return Compare(this->x, p.x) == 0 && Compare(this->y, p.y) == 0; }
    bool operator!=(const PointD &p) const { return Compare(this->x, p.x) != 0 || Compare(this->y, p.y) != 0; }

    PointD operator+(const PointD &p) const { return {this->x + p.x, this->y + p.y}; }
    PointD operator-(const PointD &p) const { return {this->x - p.x, this->y - p.y}; }

    PointD &operator+=(const PointD &p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }
    PointD &operator-=(const PointD &p) {
        this->x -= p.x;
        this->y -= p.y;
        return *this;
    }

    PointD  operator*(float t) const { return {this->x * t, this->y * t}; };
    PointD &operator*=(float t) {
        this->x *= t;
        this->y *= t;
        return *this;
    }
};

template <class T, class V>
auto Distance(const T &point1, const V &point2) {
    auto x_d  = (point1.x - point2.x);
    x_d      *= x_d;

    auto y_d  = (point1.y - point2.y);
    y_d      *= y_d;

    return std::sqrt(x_d + y_d);
}

template <class T, class V>
T MidPoint(const T &point1, const V &point2) {
    return {(point1.x + point2.x) / 2, (point1.y + point2.y) / 2};
}

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_BASE_POINT_H_
