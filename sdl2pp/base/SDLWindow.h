#pragma once
#ifndef SDL2_BASE_SDLWINDOW_H
#define SDL2_BASE_SDLWINDOW_H

#include <memory>
#include <string>


#include <SDL.h>
#include <SDL_video.h>
#include "SDLSurface.h"

namespace sdlpp {

inline auto MakeShared(SDL_Window *&&ptr) -> std::shared_ptr<SDL_Window> { return {ptr, SDL_DestroyWindow}; }

inline auto MakeUnique(SDL_Window *&&ptr) -> std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> {
    return {ptr, SDL_DestroyWindow};
}

class SDLRenderer;

class SDLWindow {
public:
    using Flags   = SDL_WindowFlags;
    using EventID = SDL_WindowEventID;

    static std::shared_ptr<SDLWindow> Create(SDL_Point        size,
                                             std::string_view title = "",
                                             SDL_Point        pos = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED},
                                             uint32_t         flags = 0) {
        return std::shared_ptr<SDLWindow>(new SDLWindow(size, title, pos, flags));
    };

    static std::shared_ptr<SDLWindow> Create(SDL_Window *&&ptr) {
        return std::shared_ptr<SDLWindow>(new SDLWindow(std::move(ptr)));
    }

    std::shared_ptr<SDLRenderer> CreateRender(uint32_t flags = 0, int index = -1);

    int GetDisplayIndex() { return SDL_GetWindowDisplayIndex(window_); };

    uint32_t GetID() { return SDL_GetWindowID(window_); }

    uint32_t GetFlags() { return SDL_GetWindowFlags(window_); }

    SDL_Window *Get() { return window_; }

    const SDL_Window *Get() const { return window_; }

    std::string GetTitle() { return SDL_GetWindowTitle(window_); }

    void SetTitle(std::string_view title) { SDL_SetWindowTitle(window_, title.data()); }

    SDL_Point GetPosition() {
        SDL_Point pos;
        SDL_GetWindowPosition(window_, &pos.x, &pos.y);
        return pos;
    }

    void SetPosition(SDL_Point pos) { SDL_SetWindowPosition(window_, pos.x, pos.y); }

    int GetWidth() const {
        int w;
        SDL_GetWindowSize(window_, &w, nullptr);
        return w;
    }

    int GetHeight() const {
        int h;
        SDL_GetWindowSize(window_, nullptr, &h);
        return h;
    }

    std::tuple<int, int> GetSize() {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        return {w, h};
    }

    void SetSize(SDL_Point size) { SDL_SetWindowSize(window_, size.x, size.y); }

    // future
    // SDL_SetWindowDisplayMode
    // SDL_GetWindowDisplayMode
    // SDL_GetWindowFromID
    // SDL_SetWindowIcon
    void SetIcon(std::shared_ptr<SDLSurface> surface) { SDL_SetWindowIcon(this->window_, surface->Get()); }

    // SDL_GetWindowBordersSize
    // SDL_GetWindowSizeInPixels
    // SDL_SetWindowMinimumSize
    // SDL_GetWindowMinimumSize
    // SDL_SetWindowMaximumSize
    // SDL_GetWindowMaximumSize
    // SDL_GetWindowSurface

    bool UpdateSurface() { return SDL_UpdateWindowSurface(window_) == 0; }

    // SDL_UpdateWindowSurfaceRects
    // SDL_SetWindowGrab
    // SDL_SetWindowKeyboardGrab
    // SDL_SetWindowMouseGrab
    // SDL_GetWindowGrab
    // SDL_GetWindowKeyboardGrab
    // SDL_GetWindowMouseGrab
    // SDL_GetGrabbedWindow
    // SDL_SetWindowMouseRect
    // SDL_GetWindowMouseRect
    // SDL_SetWindowModalFor
    // SDL_SetWindowGammaRamp
    // SDL_GetWindowGammaRamp
    // SDL_FlashWindow

    // opengl
    // SDL_GL_MakeCurrent
    // SDL_GL_GetCurrentWindow
    // SDL_GL_GetCurrentContext
    // SDL_GL_GetDrawableSize
    // SDL_GL_SwapWindow

    const SDL_PixelFormat *GetPixelFormat() { return SDL_GetWindowSurface(window_)->format; }

    void SetBordered(bool bordered) { SDL_SetWindowBordered(window_, bordered ? SDL_TRUE : SDL_FALSE); }

    void SetResizeable(bool resizable) { SDL_SetWindowResizable(window_, resizable ? SDL_TRUE : SDL_FALSE); }

    void SetAlwaysOnTop(bool alwaysOnTop) { SDL_SetWindowAlwaysOnTop(window_, alwaysOnTop ? SDL_TRUE : SDL_FALSE); }

    void Show() { SDL_ShowWindow(window_); }

    void Hide() { SDL_HideWindow(window_); }

    void Raise() { SDL_RaiseWindow(window_); }

    void Minimize() { SDL_MinimizeWindow(window_); }

    void Restore() { SDL_RestoreWindow(window_); }

    bool SetFullscreen(bool fullscreen) {
        return SDL_SetWindowFullscreen(window_, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) == 0;
    }

    bool SetBrightness(float brightness) { return SDL_SetWindowBrightness(window_, brightness) == 0; }

    float GetBrightness() { return SDL_GetWindowBrightness(window_); }

    bool SetOpacity(float opacity) { return SDL_SetWindowOpacity(window_, opacity) == 0; }

    bool GetOpacity(float *opacity) { return SDL_GetWindowOpacity(window_, opacity) == 0; }

    bool SetFocus() { return SDL_SetWindowInputFocus(window_) == 0; }

    static bool IsScreenSaverEnabled() { return SDL_IsScreenSaverEnabled(); };

    static void SetScreenSaverEnabled(bool enabled) {
        if(enabled) {
            SDL_EnableScreenSaver();
        } else {
            SDL_DisableScreenSaver();
        }
    }

    virtual ~SDLWindow() {
        if(window_) {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }
    };

    SDLWindow(const SDLWindow &) = delete;

    SDLWindow &operator=(const SDLWindow &) = delete;

    SDLWindow(SDLWindow &&) = default;

    SDLWindow &operator=(SDLWindow &&) = default;

protected:
    explicit SDLWindow(SDL_Point        size,
                       std::string_view title = "",
                       SDL_Point        pos   = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED},
                       uint32_t         flags = 0);

    explicit SDLWindow(SDL_Window *&&window) : window_(window){};

private:
    SDL_Window *window_{nullptr};
};
} // namespace sdlpp

#endif // SDL2_BASE_SDLWINDOW_H
