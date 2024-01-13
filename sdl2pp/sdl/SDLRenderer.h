#pragma once
#ifndef SDL2PP_BASE_SDLRENDERER_H_
#define SDL2PP_BASE_SDLRENDERER_H_

#include <vector>
#include <memory>

#include <SDL_render.h>

#include "sdl2pp/common/Point.hpp"
#include "sdl2pp/common/shape/Rect.h"

namespace sdlpp {

class RenderDrivers {
public:
    static RenderDrivers *GetInstance() {
        static auto *instance = new RenderDrivers;
        return instance;
    }

    ~RenderDrivers() = default;

    int GetRendersCount() const { return renderers_info_.size(); }

    const std::vector<SDL_RendererInfo> &GetRenderersInfo() const { return renderers_info_; }

private:
    RenderDrivers();

    std::vector<SDL_RendererInfo> renderers_info_;
};

inline std::shared_ptr<SDL_Renderer> MakeShared(SDL_Renderer *&&ptr) { return {ptr, SDL_DestroyRenderer}; }

inline std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> MakeUnique(SDL_Renderer *&&ptr) {
    return {ptr, SDL_DestroyRenderer};
}

class SDLWindow;

class SDLTexture;

class SDLSurface;

// H 水平  V 垂直
using Flip = SDL_RendererFlip;

class SDLRenderer {
public:
    using Flags     = SDL_RendererFlags;
    using ScaleMode = SDL_ScaleMode;
    using BlendMode = SDL_BlendMode;

    static std::shared_ptr<SDLRenderer> Create(SDL_Renderer *&&renderer) {
        return std::shared_ptr<SDLRenderer>(new SDLRenderer(std::move(renderer)));
    }

    static std::shared_ptr<SDLRenderer> Create(std::shared_ptr<SDLWindow> window, Uint32 flags = 0, int index = -1);

    SDL_Renderer *Get() { return this->renderer_; }

    const SDL_Renderer *Get() const { return this->renderer_; }

    bool IsValid() const { return this->renderer_ != nullptr; }

    std::shared_ptr<SDLTexture> CreateTextureFromSurface(std::shared_ptr<SDLSurface> surface);

    bool SetRenderTarget(std::shared_ptr<SDLTexture> texture);
    bool SetRenderTarget(SDL_Texture *texture);

    SDL_Texture *GetRenderTarget() { return SDL_GetRenderTarget(this->renderer_); }

    //     SDL_CreateSoftwareRenderer

    SDL_Window *GetWindow() { return SDL_RenderGetWindow(this->renderer_); }

    bool SetViewport(const RectI *rect = nullptr) { return SDL_RenderSetViewport(this->renderer_, rect) == 0; }

    bool SetViewport(const RectI &rect) { return SetViewport(&rect); }

    void  GetViewport(RectI &rect) const { SDL_RenderGetViewport(this->renderer_, &rect); }
    RectI GetViewport() const {
        RectI res;
        SDL_RenderGetViewport(this->renderer_, &res);
        return res;
    }

    // SDL_RenderGetViewport
    // SDL_RenderSetClipRect
    // SDL_RenderGetClipRect
    // SDL_RenderIsClipEnabled
    // SDL_RenderWindowToLogical
    // SDL_RenderLogicalToWindow

    bool GetRendererInfo(SDL_RendererInfo *info) const { return SDL_GetRendererInfo(renderer_, info) == 0; }

    // w, h
    std::tuple<int, int> GetOutputSize() const {
        int w, h;
        if(SDL_GetRendererOutputSize(renderer_, &w, &h) == 0)
            return {w, h};
        return {0, 0};
    }

    bool IsRendererTargetSupported() { return SDL_RenderTargetSupported(renderer_) == SDL_TRUE; }

    // w, h
    std::tuple<int, int> GetLogicalSize() const {
        int w, h;
        SDL_RenderGetLogicalSize(renderer_, &w, &h);
        return {w, h};
    }

    bool SetLogicalSize(int w, int h) { return SDL_RenderSetLogicalSize(renderer_, w, h) == 0; }

    bool SetIntegerScale(bool enable) {
        return SDL_RenderSetIntegerScale(renderer_, enable ? SDL_TRUE : SDL_FALSE) == 0;
    }

    bool GetIntegerScale() const { return SDL_RenderGetIntegerScale(renderer_) == SDL_TRUE; }

    bool SetScale(float scaleX, float scaleY) { return SDL_RenderSetScale(renderer_, scaleX, scaleY) == 0; }

    std::tuple<float, float> GetScale() const {
        float scaleX, scaleY;
        SDL_RenderGetScale(renderer_, &scaleX, &scaleY);
        return {scaleX, scaleY};
    }

    bool SetDrawColor(const SDL_Color &color) {
        return SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a) == 0;
    }

    SDL_Color GetDrawColor() const {
        SDL_Color color;
        SDL_GetRenderDrawColor(renderer_, &color.r, &color.g, &color.b, &color.a);
        return color;
    }

    bool SetDrawBlendMode(BlendMode blendMode) {
        return SDL_SetRenderDrawBlendMode(renderer_, static_cast<SDL_BlendMode>(blendMode)) == 0;
    }

    bool GetDrawBlendMode(BlendMode *blendMode) const { return SDL_GetRenderDrawBlendMode(renderer_, blendMode) == 0; }

    bool Clear() { return SDL_RenderClear(renderer_) == 0; }

    bool DrawPoint(const PointI &point) { return SDL_RenderDrawPoint(renderer_, point.x, point.y) == 0; }

    bool DrawPoints(const std::vector<PointI> &points) {
        return SDL_RenderDrawPoints(renderer_, points.data(), points.size()) == 0;
    }

    bool DrawLine(const PointI &start, const PointI &end) {
        return SDL_RenderDrawLine(renderer_, start.x, start.y, end.x, end.y) == 0;
    }

    bool DrawLines(const std::vector<PointI> &points) {
        return SDL_RenderDrawLines(renderer_, points.data(), points.size()) == 0;
    }

    bool DrawRect(const RectI &rect) { return SDL_RenderDrawRect(renderer_, &rect) == 0; }

    bool DrawRects(const RectI *rects, int count) { return SDL_RenderDrawRects(renderer_, rects, count) == 0; }

    bool FillRect(const RectI &rect) { return SDL_RenderFillRect(renderer_, &rect) == 0; }

    bool FillRects(const RectI *rects, int count) { return SDL_RenderFillRects(renderer_, rects, count) == 0; }


    bool Update(std::shared_ptr<SDLTexture> texture, const RectI *src_rect = nullptr, const RectI *dst_rect = nullptr);

    bool Update(std::shared_ptr<SDLTexture> texture, const RectI *src_rect, const RectF *dst_rect);

    bool Update(std::shared_ptr<SDLTexture> texture,
                const RectI                *src_rect,
                const RectI                *dst_rect,
                double                      angle,
                const PointI               &center,
                Flip                        flip);

    bool Update(std::shared_ptr<SDLTexture> texture,
                const RectI                *src_rect,
                const RectF                *dst_rect,
                double                      angle,
                const PointF               &center,
                Flip                        flip);


    bool DrawPoint(const PointF &point) { return SDL_RenderDrawPointF(renderer_, point.x, point.y) == 0; }

    bool DrawPoints(const std::vector<PointF> &points) {
        return SDL_RenderDrawPointsF(renderer_, points.data(), points.size()) == 0;
    }

    bool DrawLine(const PointF &start, const PointF &end) {
        return SDL_RenderDrawLineF(renderer_, start.x, start.y, end.x, end.y) == 0;
    }

    bool DrawLines(const std::vector<PointF> &points) {
        return SDL_RenderDrawLinesF(renderer_, points.data(), points.size()) == 0;
    }

    bool DrawRect(const RectF &rect) { return SDL_RenderDrawRectF(renderer_, &rect) == 0; }

    bool DrawRects(const RectF *rects, int count) { return SDL_RenderDrawRectsF(renderer_, rects, count) == 0; }

    bool DrawFillRect(const RectF &rect) { return SDL_RenderFillRectF(renderer_, &rect) == 0; }

    bool DrawFillRects(const RectF *rects, int count) { return SDL_RenderFillRectsF(renderer_, rects, count) == 0; }

    // SDL_RenderGeometry
    // SDL_RenderGeometryRaw
    // SDL_RenderReadPixels

    void Flush() { SDL_RenderPresent(renderer_); }

    bool SetVSync(bool vsync) { return SDL_RenderSetVSync(renderer_, vsync ? 1 : 0) == 0; }

    virtual ~SDLRenderer() {
        if(renderer_) {
            SDL_DestroyRenderer(renderer_);
            renderer_ = nullptr;
        }
    }

    SDLRenderer(const SDLRenderer &) = delete;

    SDLRenderer &operator=(const SDLRenderer &) = delete;

    SDLRenderer(SDLRenderer &&other) = default;

    SDLRenderer &operator=(SDLRenderer &&other) = default;

protected:
    explicit SDLRenderer(SDL_Renderer *&&renderer) : renderer_(renderer) {}

private:
    SDL_Renderer *renderer_{nullptr};
};

} // namespace sdlpp

#endif // SDL2PP_BASE_SDLRENDERER_H_
