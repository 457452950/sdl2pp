#pragma once
#ifndef SDL2PP_SDL2PP_COMMON_SHAPE_CIRCLE_H_
#define SDL2PP_SDL2PP_COMMON_SHAPE_CIRCLE_H_

#include <array>
#include <vector>

#include "sdl2pp/common/Point.hpp"

namespace sdlpp {

struct Circle {
    PointD center;
    double r;

    Circle() : center(0, 0), r(0) {}
    Circle(const PointD &_center, double _r) : center(_center), r(_r) {}
    ~Circle() = default;

    Circle(const Circle &)            = default;
    Circle(Circle &&)                 = default;
    Circle &operator=(const Circle &) = default;
    Circle &operator=(Circle &&)      = default;

    // a point is in the rect or on the edge of the Circle
    bool InOrOn(const PointD &point) const {
        auto dis = Distance(center, point);
        return Compare(dis, r) <= 0;
    }

    // a point is in the rect but not on the edge of the rectangle
    bool In(const PointD &point) const {
        auto dis = Distance(center, point);
        return Compare(dis, r) < 0;
    }

    // a point is on the edge of the rectangle
    bool OnTheEdge(const PointD &point) const {
        auto dis = Distance(center, point);
        return Compare(dis, r) == 0;
    }

    bool Empty() const { return Compare(r, 0) <= 0; }

    bool operator==(const Circle &other) const { return center == other.center && Compare(this->r, other.r) == 0; }

    bool Intersects(const Circle &other) const {
        auto dis = Distance(this->center, other.center);
        return Compare(dis, this->r + other.r) <= 0;
    }

    // 三点定圆
    static Circle EnclosePoints(std::array<PointD, 3> points) {
        double x_1 = points[0].x;
        double y_1 = points[0].y;
        double x_2 = points[1].x;
        double y_2 = points[1].y;
        double x_3 = points[2].x;
        double y_3 = points[2].y;


        double a = x_1 - x_2;
        double b = y_1 - y_2;
        double c = x_1 - x_3;
        double d = y_1 - y_3;

        double e = ((x_1 * x_1 - x_2 * x_2) - (y_2 * y_2 - y_1 * y_1)) / 2;
        double f = ((x_1 * x_1 - x_3 * x_3) - (y_3 * y_3 - y_1 * y_1)) / 2;

        double z = a * d - b * c;
        PointD point{(e * d - b * f) / z, (a * f - e * c) / z};

        return Circle(point, Distance(point, points[0]));
    }

    // 最小包络圆
    //    static Circle EnclosePoints(std::vector<PointI> points) {}
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_COMMON_SHAPE_CIRCLE_H_
