#pragma once
#ifndef SDL2PP_SDL2PP_SWINDOW_HPP_
#define SDL2PP_SDL2PP_SWINDOW_HPP_

#include <atomic>
#include <chrono>

#include <SDL_ttf.h>
#include <SDL_image.h>

#include "sdl2pp/sdl/SDLWindow.hpp"
#include "common/Time.hpp"
#include "common/SDLLog.h"
#include "SRenderer.hpp"
#include "STexture.hpp"


namespace sdlpp {


class SWindow : public SDLWindow {
    static const PointI DEFAULT_SIZE;

public:
    SWindow() : SWindow(0) {}
    explicit SWindow(uint32_t flag) :
        SDLWindow(DEFAULT_SIZE, "", {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED}, flag) {
        this->SetPhysicPerS(60);
    }

    ~SWindow() override {}

    // renderer

    std::shared_ptr<SRenderer> CreateRenderer(uint32_t flags = 0, int index = -1) {
        renderer_ = SRenderer::Create(SDL_CreateRenderer(this->Get(), index, flags));
        return renderer_;
    }

    std::shared_ptr<SRenderer> GetRenderer() { return renderer_; }

    // set fps
    void SetFps(int fps) {
        if(fps == -1) {
            frame_delay_mics_ = 0;
        } else {
            frame_delay_mics_ = (sdlpp::GetPerformanceFrequency() / (uint64_t)fps);
        }
    }
    // set pps
    void SetPhysicPerS(uint32_t physic_per_s) {
        if(physic_per_s == 0) {
            physic_delay_micrs_ = 0;
            return;
        }

        physic_delay_micrs_ = ((double)sdlpp::GetPerformanceFrequency() / double(physic_per_s));
        LOG_DBG(log::LIB, "physic_delay_micrs_: {}", physic_delay_micrs_);
    }

    int Exec();

    void Close();

protected:
    // event overwrite
#pragma region event overwrite
    enum EventResult : int {
        Ignore,
        Continue,
    };

    void eventHandle(const SDL_Event &event);

    virtual int WindowEvent(const SDL_WindowEvent &event) { return Continue; }

    void DefaultWindowEvent(const SDL_WindowEvent &event);

    virtual int KeyEvent(const SDL_KeyboardEvent &event) { return Ignore; }

    virtual int MouseMoveEvent(const SDL_MouseMotionEvent &event) { return Ignore; }

    virtual int MouseButtonEvent(const SDL_MouseButtonEvent &event) { return Ignore; }

    virtual int MouseWheelEvent(const SDL_MouseWheelEvent &event) { return Ignore; }
#pragma endregion

    // render process
#pragma region render process
    void render() {
        this->RenderClear();
        this->RenderProcess();
        this->RenderFlush();
    }

    virtual void RenderClear() {}
    virtual void RenderProcess() {}
    virtual void RenderFlush() {}
#pragma endregion

    virtual void Tick(double tick_ms) {}

    void CheckPhysicFrame();

private:
    std::atomic_bool           active_;
    SDL_Event                  event_{};
    std::shared_ptr<SRenderer> renderer_{nullptr};

    uint64_t current_time_{0};
    uint32_t frame_delay_mics_{0};
    uint64_t current_physic_time_{0};
    double   physic_delay_micrs_{0};
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_SWINDOW_HPP_
