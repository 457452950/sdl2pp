#pragma once
#ifndef SDL2PP_SDL2PP_STEXTURE_HPP_
#define SDL2PP_SDL2PP_STEXTURE_HPP_

#include <SDL_log.h>

#include "common/Math.h"
#include "common/shape/Rect.h"
#include "sdl2pp/sdl/SDLTexture.hpp"

#include "SRenderer.hpp"
#include "Renderable.hpp"

namespace sdlpp {


class STexture : public Renderable {
public:
    explicit STexture(std::shared_ptr<SDLTexture> texture, const RectI *texture_rect = nullptr) : texture_(texture) {
        if(texture_rect == nullptr) {
            texture_->Query(nullptr, nullptr, &texture_rect_.w, &texture_rect_.h);
        } else {
            texture_rect_ = *texture_rect;
        }
        center_.x = static_cast<float>(texture_rect_.w) / 2;
        center_.y = static_cast<float>(texture_rect_.h) / 2;
    }

    std::shared_ptr<SDLTexture> GetTexture() const { return texture_; }

    bool is_debug{false};

    void Render(std::shared_ptr<SRenderer> renderer,
                PointF                     base_pos   = {0, 0},
                double                     base_angle = 0,
                Scale                      scale      = {1.0, 1.0}) override {
        if(!enable_ || !texture_) {
            return;
        }

        auto angle_a = Atan(position_.y, position_.x);
        auto angle_b = base_angle / 180 * M_PI;

        auto r = std::sqrt(position_.x * position_.x + position_.y * position_.y);

        Scale real_scale = this->scale_ * scale;

        RectF dst_size{static_cast<float>(base_pos.x + std::cos(angle_a + angle_b) * r),
                       static_cast<float>(base_pos.y + std::sin(angle_a + angle_b) * r),
                       static_cast<float>(static_cast<double>(texture_rect_.w) * real_scale.w),
                       static_cast<float>(static_cast<double>(texture_rect_.h) * real_scale.h)};

        renderer->Update(texture_, &texture_rect_, &dst_size, base_angle + angle_, center_, flip_);

        if(is_debug)
            renderer->DrawRect(dst_size);
    }

private:
    std::shared_ptr<SDLTexture> texture_;

    RectI texture_rect_{};
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_STEXTURE_HPP_
