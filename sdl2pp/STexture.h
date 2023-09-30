#pragma once
#ifndef SDL2PP_STEXTURE_H_
#define SDL2PP_STEXTURE_H_

#include <cmath>

#include <SDL_log.h>

#include "base/Math.h"
#include "base/SDLTexture.h"

#include "SRenderer.h"
#include "Renderable.h"

namespace sdlpp {


class STexture : public Renderable {
public:
    explicit STexture(std::shared_ptr<SDLTexture> texture, const SDL_Rect *texture_rect = nullptr) : texture_(texture) {
        if(texture_rect == nullptr) {
            texture_->Query(nullptr, nullptr, &texture_rect_.w, &texture_rect_.h);
        } else {
            texture_rect_ = *texture_rect;
        }
        center_.x = static_cast<float>(texture_rect_.w) / 2;
        center_.y = static_cast<float>(texture_rect_.h) / 2;
    }

    std::shared_ptr<SDLTexture> GetTexture() const { return texture_; }


    void Render(std::shared_ptr<SRenderer> renderer, PointF base_pos, double base_angle, Scale scale) override {
        if(!enable_) {
            return;
        }

        auto angle_a = std::atan2f(position_.y, position_.x);
        auto angle_b = base_angle / 180 * PI_H;

        auto r = std::sqrt(position_.x * position_.x + position_.y * position_.y);

        Scale real_scale = this->scale_ * scale;

        SDL_FRect dst_size{static_cast<float>(base_pos.x + std::cos(angle_a + angle_b) * r),
                           static_cast<float>(base_pos.y + std::sin(angle_a + angle_b) * r),
                           static_cast<float>(static_cast<double>(texture_rect_.w) * real_scale.w),
                           static_cast<float>(static_cast<double>(texture_rect_.h) * real_scale.h)};

        renderer->Update(texture_, &texture_rect_, &dst_size, base_angle + angle_, center_, flip_);
    }

private:
    std::shared_ptr<SDLTexture> texture_;

    SDL_Rect texture_rect_{};
};

} // namespace sdlpp

#endif // SDL2PP_STEXTURE_H_
