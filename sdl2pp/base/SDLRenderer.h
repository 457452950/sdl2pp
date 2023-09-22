#pragma once
#ifndef SDL2PP_SDL2PP_BASE_SDLRENDERER_H_
#define SDL2PP_SDL2PP_BASE_SDLRENDERER_H_

#include <vector>
#include <memory>

#include <SDL_render.h>


namespace sdlpp {

    class RenderDrivers {
    public:
        static RenderDrivers *GetInstance() {
            static auto *instance = new RenderDrivers;
            return instance;
        }

        ~RenderDrivers() = default;

        int GetRendersCount() const {
            return renderers_info_.size();
        }

        const std::vector<SDL_RendererInfo> &GetRenderersInfo() const {
            return renderers_info_;
        }

    private:
        RenderDrivers();

        std::vector<SDL_RendererInfo> renderers_info_;
    };

    inline std::shared_ptr<SDL_Renderer> MakeShared(SDL_Renderer *&&ptr) {
        return {ptr, SDL_DestroyRenderer};
    }

    inline std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> MakeUnique(SDL_Renderer *&&ptr) {
        return {ptr, SDL_DestroyRenderer};
    }

    class SDLWindow;

    class SDLTexture;

    class SDLSurface;

    class SDLRenderer {
    public:
        using Flags = SDL_RendererFlags;
        using ScaleMode = SDL_ScaleMode;
        using Flip = SDL_RendererFlip;
        using BlendMode = SDL_BlendMode;

        static std::shared_ptr<SDLRenderer>
        Create(SDL_Renderer *&&renderer) {
            return std::shared_ptr<SDLRenderer>(new SDLRenderer(std::move(renderer)));
        }

        static std::shared_ptr<SDLRenderer>
        Create(std::shared_ptr<SDLWindow> window, Uint32 flags = 0, int index = -1);

        SDL_Renderer *Get() {
            return this->renderer_;
        }

        const SDL_Renderer *Get() const {
            return this->renderer_;
        }

        bool IsValid() const {
            return this->renderer_ != nullptr;
        }

        std::shared_ptr<SDLTexture> CreateTextureFromSurface(std::shared_ptr<SDLSurface> surface);

        bool SetRenderTarget(std::shared_ptr<SDLTexture> texture);
        // SDL_GetRenderTarget


        // SDL_CreateSoftwareRenderer
        // SDL_RenderGetWindow
        // SDL_SetRenderTarget
        // SDL_GetRenderTarget

        bool SetViewport(const SDL_Rect *rect = nullptr) {
            return SDL_RenderSetViewport(this->renderer_, rect) == 0;
        }

        bool SetViewport(const SDL_Rect &rect) {
            return SetViewport(&rect);
        }

        void GetViewport(SDL_Rect &rect) const {
            SDL_RenderGetViewport(this->renderer_, &rect);
        }

        // SDL_RenderGetViewport
        // SDL_RenderSetClipRect
        // SDL_RenderGetClipRect
        // SDL_RenderIsClipEnabled
        // SDL_RenderWindowToLogical
        // SDL_RenderLogicalToWindow

        bool GetRendererInfo(SDL_RendererInfo *info) const {
            return SDL_GetRendererInfo(renderer_, info) == 0;
        }

        std::tuple<int, int>
        GetOutputSize() const {
            int w, h;
            SDL_GetRendererOutputSize(renderer_, &w, &h);
            return {w, h};
        }

        bool IsRendererTargetSupported() {
            return SDL_RenderTargetSupported(renderer_) == SDL_TRUE;
        }

        std::tuple<int, int>
        GetLogicalSize() const {
            int w, h;
            SDL_RenderGetLogicalSize(renderer_, &w, &h);
            return {w, h};
        }

        bool SetLogicalSize(int w, int h) {
            return SDL_RenderSetLogicalSize(renderer_, w, h) == 0;
        }

        bool SetIntegerScale(bool enable) {
            return SDL_RenderSetIntegerScale(renderer_, enable ? SDL_TRUE : SDL_FALSE) == 0;
        }

        bool GetIntegerScale() const {
            return SDL_RenderGetIntegerScale(renderer_) == SDL_TRUE;
        }

        bool
        SetScale(float scaleX, float scaleY) {
            return SDL_RenderSetScale(renderer_, scaleX, scaleY) == 0;
        }

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

        bool GetDrawBlendMode(BlendMode *blendMode) const {
            return SDL_GetRenderDrawBlendMode(renderer_, blendMode) == 0;
        }

        bool
        Clear() {
            return SDL_RenderClear(renderer_) == 0;
        }

        bool DrawPoint(const SDL_Point &point) {
            return SDL_RenderDrawPoint(renderer_, point.x, point.y) == 0;
        }

        bool DrawPoints(const SDL_Point *points, int count) {
            return SDL_RenderDrawPoints(renderer_, points, count) == 0;
        }

        bool DrawLine(const SDL_Point &start, const SDL_Point &end) {
            return SDL_RenderDrawLine(renderer_, start.x, start.y, end.x, end.y) == 0;
        }

        bool DrawLines(const SDL_Point *points, int count) {
            return SDL_RenderDrawLines(renderer_, points, count) == 0;
        }

        bool DrawRect(const SDL_Rect &rect) {
            return SDL_RenderDrawRect(renderer_, &rect) == 0;
        }

        bool DrawRects(const SDL_Rect *rects, int count) {
            return SDL_RenderDrawRects(renderer_, rects, count) == 0;
        }

        bool FillRect(const SDL_Rect &rect) {
            return SDL_RenderFillRect(renderer_, &rect) == 0;
        }

        bool FillRects(const SDL_Rect *rects, int count) {
            return SDL_RenderFillRects(renderer_, rects, count) == 0;
        }


        bool Update(std::shared_ptr<SDLTexture> texture, const SDL_Rect *src_rect = nullptr,
                    const SDL_Rect *dst_rect = nullptr);

        bool Update(std::shared_ptr<SDLTexture> texture, const SDL_Rect *src_rect,
                    const SDL_FRect *dst_rect);

        bool
        UpdateEx(std::shared_ptr<SDLTexture> texture, const SDL_Rect *src_rect, const SDL_Rect *dst_rect, double angle,
                 SDL_Point center,
                 Flip flip);

        bool
        UpdateEx(std::shared_ptr<SDLTexture> texture, const SDL_Rect *src_rect, const SDL_FRect *dst_rect, double angle,
                 SDL_FPoint center,
                 Flip flip);


        bool DrawPoint(const SDL_FPoint &point) {
            return SDL_RenderDrawPointF(renderer_, point.x, point.y) == 0;
        }

        bool DrawPoints(const SDL_FPoint *points, int count) {
            return SDL_RenderDrawPointsF(renderer_, points, count) == 0;
        }

        bool DrawLine(const SDL_FPoint &start, const SDL_FPoint &end) {
            return SDL_RenderDrawLineF(renderer_, start.x, start.y, end.x, end.y) == 0;
        }

        bool DrawLines(const SDL_FPoint *points, int count) {
            return SDL_RenderDrawLinesF(renderer_, points, count) == 0;
        }

        bool DrawRect(const SDL_FRect &rect) {
            return SDL_RenderDrawRectF(renderer_, &rect) == 0;
        }

        bool DrawRects(const SDL_FRect *rects, int count) {
            return SDL_RenderDrawRectsF(renderer_, rects, count) == 0;
        }

        bool DrawFillRect(const SDL_FRect &rect) {
            return SDL_RenderFillRectF(renderer_, &rect) == 0;
        }

        bool DrawFillRects(const SDL_FRect *rects, int count) {
            return SDL_RenderFillRectsF(renderer_, rects, count) == 0;
        }

        // SDL_RenderGeometry
        // SDL_RenderGeometryRaw
        // SDL_RenderReadPixels

        void Flush() {
            SDL_RenderPresent(renderer_);
        }

        bool SetVSync(bool vsync) {
            return SDL_RenderSetVSync(renderer_, vsync ? 1 : 0) == 0;
        }

        ~SDLRenderer() {
            if (renderer_) {
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

} // sdlpp

#endif //SDL2PP_SDL2PP_BASE_SDLRENDERER_H_
