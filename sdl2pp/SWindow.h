#pragma once
#ifndef SDL2PP_SDL2PP_SWINDOW_H_
#define SDL2PP_SDL2PP_SWINDOW_H_

#include <atomic>
#include <chrono>

#include <SDL_ttf.h>
#include <SDL_image.h>

#include "base/SDLWindow.h"
#include "SRenderer.h"
#include "STexture.h"


namespace sdlpp {


class SWindow : public SDLWindow {
    static const SDL_Point DEFAULT_SIZE;

public:
    SWindow() : SDLWindow(DEFAULT_SIZE) {
        physic_delay_micrs_ = 1000'000.0 / 60.0;
        SDL_Log("%f p delay", physic_delay_micrs_);
    }

    ~SWindow() override {}


    // renderer

    std::shared_ptr<SRenderer> CreateRenderer(uint32_t flags = 0, int index = -1) {
        renderer_ = SRenderer::Create(SDL_CreateRenderer(this->Get(), index, flags));
        return renderer_;
    }

    std::shared_ptr<SRenderer> GetRenderer() { return renderer_; }

    // event overwrite

    enum EventResult : int {
        Ignore,
        Continue,
    };

    virtual int WindowEvent(const SDL_WindowEvent &event) { return Continue; }

    void DefaultWindowEvent(const SDL_WindowEvent &event);

    virtual int KeyEvent(const SDL_KeyboardEvent &event) { return Ignore; }

    virtual int MouseMoveEvent(const SDL_MouseMotionEvent &event) { return Ignore; }

    virtual int MouseButtonEvent(const SDL_MouseButtonEvent &event) { return Ignore; }

    virtual int MouseWheelEvent(const SDL_MouseWheelEvent &event) { return Ignore; }

    virtual void RenderProcess(PointF view_pos, double view_angle) {}

    virtual void Tick(double tick_ms) {}

    PointF GetViewPos() const { return view_pos_; }

    void ViewMoveTo(const PointF &pos) { view_pos_ = pos; }

    void ViewMove(const PointF &delta) { this->view_pos_ += delta; }

    void SetFps(int fps) {
        if(fps == -1) {
            frame_delay_mics_ = 0;
        } else {
            frame_delay_mics_ = (1000'000 / fps);
        }
    }

    void SetPhysicPerS(uint32_t physic_per_s) { physic_delay_micrs_ = (1000'000.0 / double(physic_per_s)); }

    int Exec();

    void Close();

private:
    std::atomic_bool           active_;
    SDL_Event                  event_{};
    std::shared_ptr<SRenderer> renderer_{nullptr};
    PointF                     view_pos_{0, 0};

    std::chrono::microseconds current_time_{0};
    uint32_t                  frame_delay_mics_{0};
    std::chrono::microseconds current_physic_time_{0};
    double                    physic_delay_micrs_{0};
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_SWINDOW_H_
