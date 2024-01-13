#pragma once
#ifndef SDL2PP_SDL2PP_COMMON_SCALE_HPP_
#define SDL2PP_SDL2PP_COMMON_SCALE_HPP_

namespace sdlpp {

struct Scale {
    double w{1.0f}, h{1.0f};


    Scale() = default;
    Scale(double _w, double _h) : w(_w), h(_h) {}

    Scale(const Scale &p) = default;
    Scale(Scale &&p)      = default;

    Scale &operator=(const Scale &p) = default;
    Scale &operator=(Scale &&p)      = default;

    bool operator==(const Scale &p) const { return this->w == p.w && this->h == p.h; }
    bool operator!=(const Scale &p) const { return this->w != p.w || this->h != p.h; }

    Scale  operator*(double t) const { return {this->w * t, this->h * t}; };
    Scale &operator*=(double t) {
        this->w *= t;
        this->h *= t;
        return *this;
    }
    Scale  operator/(double t) const { return {this->w / t, this->h / t}; };
    Scale &operator/=(double t) {
        this->w /= t;
        this->h /= t;
        return *this;
    }

    Scale  operator*(const Scale &p) const { return {this->w * p.w, this->h * p.h}; }
    Scale &operator*=(const Scale &p) {
        this->w *= p.w;
        this->h *= p.h;
        return *this;
    }

    Scale  operator/(const Scale &p) const { return {this->w / p.w, this->h / p.h}; }
    Scale &operator/=(const Scale &p) {
        this->w /= p.w;
        this->h /= p.h;
        return *this;
    }
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_COMMON_SCALE_HPP_
