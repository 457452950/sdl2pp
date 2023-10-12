#pragma once
#ifndef SDL2PP_RENDERABLE_H_
#define SDL2PP_RENDERABLE_H_

#include "common/Point.h"
#include "common/shape/Rect.h"
#include "common/Scale.h"

#include "SRenderer.h"

namespace sdlpp {

class Renderable {
public:
    bool Enable() const { return enable_; }

    void Enable(bool enable) { enable_ = enable; }

    void MoveTo(PointF position) { position_ = position; }

    void Move(PointF offset) { position_ += offset; }

    // 旋转
    void Rotate(double angle) { angle_ += angle; }

    void SetRotation(double angle) { angle_ = angle; }

    double GetRotation() const { return angle_; }

    // 缩放
    void SetScale(Scale scale) { scale_ = scale; }

    Scale GetScale() const { return scale_; }

    // 设置旋转中点
    void SetRotationCenter(PointF center) { center_ = center; }

    PointF GetRotationCenter() const { return center_; }

    // 翻转
    void Flip(SDLRenderer::Flip flip) { flip_ = flip; }

    virtual void Render(std::shared_ptr<SRenderer> renderer, PointF base_pos, double base_angle, Scale scale) = 0;


protected:
    bool enable_{true};

    PointF            position_{0, 0};
    double            angle_{0}; // 角度制
    Scale             scale_;
    PointF            center_{0, 0};
    SDLRenderer::Flip flip_{SDLRenderer::Flip::SDL_FLIP_NONE};
};

} // namespace sdlpp

#endif // SDL2PP_RENDERABLE_H_
