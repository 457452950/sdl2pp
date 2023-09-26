#pragma once
#ifndef SDL2PP_SDL2PP_SWINDOW_H_
#define SDL2PP_SDL2PP_SWINDOW_H_

#include <atomic>

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
        //            renderer_ = SDL_CreateRenderer(this->Get(), -1, 0);
        //            if (!renderer_) {
        //                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed. err: %s \n",
        //                SDL_GetError()); SDL_assert(renderer_);
        //            }
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

    virtual void RenderProcess(SDL_FPoint view_pos, double view_angle) {}

    SDL_FPoint GetViewPos() const { return view_pos_; }

    void ViewMoveTo(const SDL_FPoint &pos) { view_pos_ = pos; }

    void ViewMove(const SDL_FPoint &delta) {
        view_pos_.x += delta.x;
        view_pos_.y += delta.y;
    }


    int Exec();

    void Close();

private:
    std::atomic_bool           active_;
    SDL_Event                  event_{};
    std::shared_ptr<SRenderer> renderer_{nullptr};
    SDL_FPoint                 view_pos_{0, 0};
};

} // namespace sdlpp

#endif // SDL2PP_SDL2PP_SWINDOW_H_
