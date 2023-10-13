#pragma once
#ifndef SDL2PP_SDL2PP_BASE_RECT_H_
#define SDL2PP_SDL2PP_BASE_RECT_H_

#include <vector>

#include "sdl2pp/common/Math.h"
#include "sdl2pp/common/Point.h"

namespace sdlpp {

struct RectI : public SDL_Rect {
public:
    explicit RectI() : SDL_Rect{} {}
    RectI(int _x, int _y, int _w, int _h) : SDL_Rect{_x, _y, _w, _h} {}
    ~RectI() = default;

    RectI(const RectI &)            = default;
    RectI &operator=(const RectI &) = default;
    RectI(RectI &&)                 = default;
    RectI &operator=(RectI &&)      = default;

    // a point is in the rect or on the edge of the rectangle
    bool InOrOn(const PointI &point) const {
        return point.x >= x && point.x <= (x + w) && point.y >= y && point.y <= (y + h);
    }

    // a point is in the rect but not on the edge of the rectangle
    bool In(const PointI &point) const { return point.x > x && point.x < (x + w) && point.y > y && point.y < (y + h); }

    // a point is on the edge of the rectangle
    bool OnTheEdge(const PointI &point) const { return InOrOn(point) && !In(point); }

    bool Empty() const { return w <= 0 || h <= 0; }

    bool operator==(const RectI &other) const { return x == other.x && y == other.y && w == other.w && h == other.h; }

    bool Intersects(const RectI &other) const { return SDL_HasIntersection(&*this, &other) == SDL_TRUE; }

    RectI GetIntersection(const RectI &other) const {
        if(this == &other) {
            return *this;
        }

        RectI result;
        auto  res = SDL_IntersectRect(this, &other, &result);
        if(res == SDL_FALSE) {
            return RectI();
        }

        return result;
    }

    RectI GetUnion(const RectI &other) const {
        if(this == &other) {
            return *this;
        }

        RectI result;
        SDL_UnionRect(this, &other, &result);
        return result;
    }

    PointI Center() const { return {this->x + this->w / 2, this->y + this->h / 2}; }

    static RectI EnclosePoints(std::vector<PointI> points, const RectI *range = nullptr) {
        if(points.empty()) {
            return RectI();
        }

        RectI result;
        auto  res = SDL_EnclosePoints(points.data(), points.size(), range, &result);
        if(res == SDL_FALSE) {
            return RectI();
        }

        return result;
    }
};

// SDL_IntersectRectAndLine

struct RectF : public SDL_FRect {
    explicit RectF(float _x = 0, float _y = 0, float _w = 0, float _h = 0) : SDL_FRect{_x, _y, _w, _h} {}
    ~RectF() = default;

    RectF(const RectF &)            = default;
    RectF &operator=(const RectF &) = default;
    RectF(RectF &&)                 = default;
    RectF &operator=(RectF &&)      = default;

    // a point is in the rect or on the edge of the rectangle
    bool InOrOn(const PointF &point) {
        return Compare(point.x, x) != -1 && Compare(point.x, x + w) != 1 && Compare(point.y, y) != -1 &&
                Compare(point.y, y + h) != 1;
    }

    // a point is in the rect but not on the edge of the rectangle
    bool In(const PointF &point) {
        return Compare(point.x, x) == 1 && Compare(point.x, x + w) == -1 && Compare(point.y, y) == 1 &&
                Compare(point.y, (y + h)) == -1;
    }

    // a point is on the edge of the rectangle
    bool OnTheEdge(const PointF &point) { return InOrOn(point) && !In(point); }

    bool Empty() const { return SDL_FRectEmpty(this) == SDL_TRUE; }

    bool operator==(const RectF &other) const { return SDL_FRectEquals(this, &other) == SDL_TRUE; }

    bool Intersects(const RectF &other) const { return SDL_HasIntersectionF(&*this, &other) == SDL_TRUE; }

    RectF GetIntersection(const RectF &other) const {
        if(this == &other) {
            return *this;
        }

        RectF result;
        auto  res = SDL_IntersectFRect(this, &other, &result);
        if(res == SDL_FALSE) {
            return RectF();
        }

        return result;
    }

    RectF GetUnion(const RectF &other) const {
        if(this == &other) {
            return *this;
        }

        RectF result;
        SDL_UnionFRect(this, &other, &result);
        return result;
    }

    static RectF EnclosePoints(std::vector<PointF> points, const RectF *range = nullptr) {
        if(points.empty()) {
            return RectF();
        }

        RectF result;
        auto  res = SDL_EncloseFPoints(points.data(), points.size(), range, &result);
        if(res == SDL_FALSE) {
            return RectF();
        }

        return result;
    }
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_BASE_RECT_H_
