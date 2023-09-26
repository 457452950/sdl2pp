#pragma once
#ifndef SDL2PP_STEXTURE_H_
#define SDL2PP_STEXTURE_H_

#include <cmath>

#include <SDL_log.h>

#include "base/Math.h"
#include "base/SDLTexture.h"
#include "base/SDLRenderer.h"

#include "SRenderer.h"

namespace sdlpp {

struct Scale {
    double scale_w = 1.0;
    double scale_h = 1.0;
};

class STexture {
public:
    explicit STexture(std::shared_ptr<SDLTexture> texture, SDL_Rect *texture_rect = nullptr) : texture_(texture) {
        if(texture_rect == nullptr) {
            texture_->Query(nullptr, nullptr, &texture_rect_.w, &texture_rect_.h);
        } else {
            texture_rect_ = *texture_rect;
        }
        center_.x = static_cast<float>(texture_rect_.w) / 2;
        center_.y = static_cast<float>(texture_rect_.h) / 2;
    }

    std::shared_ptr<SDLTexture> GetTexture() const { return texture_; }

    void MoveTo(SDL_FPoint position) { position_ = position; }

    void Move(SDL_FPoint offset) {
        position_.x += offset.x;
        position_.y += offset.y;
    }

    // 旋转
    void Rotate(double angle) { angle_ += angle; }

    void SetRotation(double angle) { angle_ = angle; }

    double GetRotation() const { return angle_; }

    // 缩放
    void SetScale(Scale scale) { scale_ = scale; }

    Scale GetScale() const { return scale_; }

    // 设置旋转中点
    void SetRotationCenter(SDL_FPoint center) { center_ = center; }

    SDL_FPoint GetRotationCenter() const { return center_; }

    // 翻转
    void Flip(SDLRenderer::Flip flip) { flip_ = flip; }

    void Render(std::shared_ptr<SRenderer> renderer, SDL_FPoint base_pos, double base_angle, Scale scale) {
        auto angle_a = std::atan2f(position_.y, position_.x);
        auto angle_b = base_angle / 180 * PI_H;

        auto r = std::sqrt(position_.x * position_.x + position_.y * position_.y);

        Scale real_scale = {scale.scale_w * scale_.scale_w, scale.scale_h * scale_.scale_h};

        SDL_FRect dst_size{static_cast<float>(base_pos.x + std::cos(angle_a + angle_b) * r),
                           static_cast<float>(base_pos.y + std::sin(angle_a + angle_b) * r),
                           static_cast<float>(static_cast<double>(texture_rect_.w) * real_scale.scale_w),
                           static_cast<float>(static_cast<double>(texture_rect_.h) * real_scale.scale_h)};

        renderer->Update(texture_, &texture_rect_, &dst_size, base_angle + angle_, center_, flip_);
    }

private:
    std::shared_ptr<SDLTexture> texture_;
    SDL_Rect                    texture_rect_{};
    SDL_FPoint                  position_{0, 0};
    double                      angle_{0}; // 角度制
    Scale                       scale_;
    SDL_FPoint                  center_{0, 0};
    SDLRenderer::Flip           flip_{SDLRenderer::Flip::SDL_FLIP_NONE};
};

} // namespace sdlpp

#endif // SDL2PP_STEXTURE_H_
