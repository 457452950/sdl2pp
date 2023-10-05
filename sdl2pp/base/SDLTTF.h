#pragma once
#ifndef SDL2PP_BASE_SDLTTF_H_
#define SDL2PP_BASE_SDLTTF_H_

#include <string_view>

#include <SDL_ttf.h>

#include "SDLSurface.h"
#include "SDLTexture.h"

namespace sdlpp {


class Font {
public:
    static std::shared_ptr<Font> Create(TTF_Font *&&font, int point_size) {
        return std::shared_ptr<Font>(new Font(std::move(font), point_size));
    }

    static std::shared_ptr<Font> Open(std::string_view path, int point_size);

    static std::shared_ptr<Font> Open(std::string_view path, int point_size, long index);

    static std::shared_ptr<Font> Open(std::string_view path, int point_size, unsigned int hdpi, unsigned int vdpi);

    static std::shared_ptr<Font>
    Open(std::string_view path, int point_size, long index, unsigned int hdpi, unsigned int vdpi);

    int GetFontSize() const { return point_size_; }

    bool SetFontSize(int point_size) { return TTF_SetFontSize(font_, point_size) == 0; }

    bool SetFontSizeDPI(int point_size, unsigned int hdpi, unsigned int vdpi) {
        return TTF_SetFontSizeDPI(font_, point_size, hdpi, vdpi) == 0;
    }

    enum FontStyle {
        STYLE_NORMAL         = TTF_STYLE_NORMAL,
        STYLE_BOLD           = TTF_STYLE_BOLD,
        STYLE_ITALIC         = TTF_STYLE_ITALIC,
        STYLE_UNDERLINE      = TTF_STYLE_UNDERLINE,
        STYLE_STRIKE_THROUGH = TTF_STYLE_STRIKETHROUGH,
    };

    int GetSytle() { return TTF_GetFontStyle(font_); }

    void SetSytle(int style) { TTF_SetFontStyle(font_, style); }

    int GetOutline() { return TTF_GetFontOutline(font_); }

    void SetOutline(int outline) { TTF_SetFontOutline(font_, outline); }

    enum Hinting {
        HINTING_NORMAL         = TTF_HINTING_NORMAL,
        HINTING_LIGHT          = TTF_HINTING_LIGHT,
        HINTING_MONO           = TTF_HINTING_MONO,
        HINTING_NONE           = TTF_HINTING_NONE,
        HINTING_LIGHT_SUBPIXEL = TTF_HINTING_LIGHT_SUBPIXEL,
    };

    Hinting GetHinting() { return static_cast<Hinting>(TTF_GetFontHinting(font_)); }

    void SetHinting(Hinting hinting) { TTF_SetFontHinting(font_, hinting); }

    enum WrappedAlign {
        WRAPPED_ALIGN_LEFT   = TTF_WRAPPED_ALIGN_LEFT,
        WRAPPED_ALIGN_CENTER = TTF_WRAPPED_ALIGN_CENTER,
        WRAPPED_ALIGN_RIGHT  = TTF_WRAPPED_ALIGN_RIGHT,
    };

    WrappedAlign GetWrapped() { return static_cast<WrappedAlign>(TTF_GetFontWrappedAlign(font_)); }

    void SetWrapped(WrappedAlign wrapped) { TTF_SetFontWrappedAlign(font_, wrapped); }

    int GetHeight() { return TTF_FontHeight(font_); }

    int GetAscent() { return TTF_FontAscent(font_); }

    int GetDescent() { return TTF_FontDescent(font_); }

    int GetLineSkip() { return TTF_FontLineSkip(font_); }

    int GetKerning() { return TTF_GetFontKerning(font_); }

    void SetKerning(int kerning) { TTF_SetFontKerning(font_, kerning); }

    // TTF_GetFontKerningSize
    // TTF_GetFontKerningSizeGlyphs
    // TTF_GetFontKerningSizeGlyphs32
    // TTF_SetFontSDF
    // TTF_GetFontSDF


    long GetFaces() { return TTF_FontFaces(font_); }

    bool FaceIsFixedWidth() { return TTF_FontFaceIsFixedWidth(font_) != 0; }

    std::string_view FaceFamilyName() { return {TTF_FontFaceFamilyName(font_)}; }

    std::string_view FaceStyleName() { return {TTF_FontFaceStyleName(font_)}; }

    bool IsGlyphProvided(uint16_t ch) { return TTF_GlyphIsProvided(font_, ch) != 0; }

    bool IsGlyphProvided(uint32_t ch) { return TTF_GlyphIsProvided32(font_, ch) != 0; }

    // TTF_GlyphMetrics
    // TTF_GlyphMetrics32

    bool GetSizeText(std::string_view text, int &width, int &height) {
        return TTF_SizeText(font_, text.data(), &width, &height) != 0;
    }

    bool GetSizeUTF8(std::string_view text, int &width, int &height) {
        return TTF_SizeUTF8(font_, text.data(), &width, &height);
    }

    bool GetSizeUNICODE(std::u16string_view text, int &width, int &height) {
        return TTF_SizeUNICODE(font_, (const uint16_t *)text.data(), &width, &height);
    }

    bool MeasureText(std::string_view text, int measure_w, int &extent, int &count) {
        return TTF_MeasureText(font_, text.data(), measure_w, &extent, &count) != 0;
    }

    bool MeasureUTF8(std::string_view text, int measure_w, int &extent, int &count) {
        return TTF_MeasureUTF8(font_, text.data(), measure_w, &extent, &count);
    }

    bool MeasureUNICODE(std::u16string_view text, int measure_w, int &extent, int &count) {
        return TTF_MeasureUNICODE(font_, (const uint16_t *)text.data(), measure_w, &extent, &count);
    }

    std::shared_ptr<SDLSurface> RenderText_Solid(std::string_view text, const SDL_Color &color) {
        return SDLSurface::Create(TTF_RenderText_Solid(font_, text.data(), color));
    }

    std::shared_ptr<SDLSurface> RenderUTF8_Solid(std::string_view text, const SDL_Color &color) {
        return SDLSurface::Create(TTF_RenderUTF8_Solid(font_, text.data(), color));
    }

    std::shared_ptr<SDLSurface> RenderUNICODE_Solid(std::u16string_view text, const SDL_Color &color) {
        return SDLSurface::Create(TTF_RenderUNICODE_Solid(font_, (const uint16_t *)text.data(), color));
    }

    std::shared_ptr<SDLSurface>
    RenderText_Solid_Wrapped(std::string_view text, const SDL_Color &color, Uint32 wrapLength) {
        return SDLSurface::Create(TTF_RenderText_Solid_Wrapped(font_, text.data(), color, wrapLength));
    }

    std::shared_ptr<SDLSurface>
    RenderUTF8_Solid_Wrapped(std::string_view text, const SDL_Color &color, Uint32 wrapLength) {
        return SDLSurface::Create(TTF_RenderUTF8_Solid_Wrapped(font_, text.data(), color, wrapLength));
    }

    std::shared_ptr<SDLSurface>
    RenderUNICODE_Solid_Wrapped(std::u16string_view text, const SDL_Color &color, Uint32 wrapLength) {
        return SDLSurface::Create(
                TTF_RenderUNICODE_Solid_Wrapped(font_, (const uint16_t *)text.data(), color, wrapLength));
    }

    std::shared_ptr<SDLSurface> RenderGlyph_Solid(uint16_t ch, const SDL_Color &color) {
        return SDLSurface::Create(TTF_RenderGlyph_Solid(font_, ch, color));
    }

    std::shared_ptr<SDLSurface> RenderGlyph_Solid(Uint32 ch, const SDL_Color &color) {
        return SDLSurface::Create(TTF_RenderGlyph32_Solid(font_, ch, color));
    }

    std::shared_ptr<SDLSurface> RenderText_Shaded(std::string_view text, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderText_Shaded(font_, text.data(), fg, bg));
    }

    std::shared_ptr<SDLSurface> RenderUTF8_Shaded(std::string_view text, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderUTF8_Shaded(font_, text.data(), fg, bg));
    }

    std::shared_ptr<SDLSurface> RenderUNICODE_Shaded(std::string_view text, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderUNICODE_Shaded(font_, (const uint16_t *)text.data(), fg, bg));
    }

    std::shared_ptr<SDLSurface>
    RenderText_Shaded_Wrapped(std::string_view text, const SDL_Color &fg, const SDL_Color &bg, Uint32 wrapLength) {
        return SDLSurface::Create(TTF_RenderText_Shaded_Wrapped(font_, text.data(), fg, bg, wrapLength));
    }

    std::shared_ptr<SDLSurface>
    RenderUTF8_Shaded_Wrapped(std::string_view text, const SDL_Color &fg, const SDL_Color &bg, Uint32 wrapLength) {
        return SDLSurface::Create(TTF_RenderUTF8_Shaded_Wrapped(font_, text.data(), fg, bg, wrapLength));
    }

    std::shared_ptr<SDLSurface>
    RenderUNICODE_Shaded_Wrapped(std::string_view text, const SDL_Color &fg, const SDL_Color &bg, Uint32 wrapLength) {
        return SDLSurface::Create(
                TTF_RenderUNICODE_Shaded_Wrapped(font_, (const uint16_t *)text.data(), fg, bg, wrapLength));
    }

    std::shared_ptr<SDLSurface> RenderGlyph_Shaded(uint32_t glyph, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderGlyph32_Shaded(font_, glyph, fg, bg));
    }

    std::shared_ptr<SDLSurface> RenderGlyph_Shaded(uint16_t glyph, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderGlyph_Shaded(font_, glyph, fg, bg));
    }

    std::shared_ptr<SDLSurface> RenderText_Blended(std::string_view text, const SDL_Color &fg) {
        return SDLSurface::Create(TTF_RenderText_Blended(font_, text.data(), fg));
    }

    std::shared_ptr<SDLSurface> RenderUNICODE_Blended(std::string_view text, const SDL_Color &fg) {
        return SDLSurface::Create(TTF_RenderUNICODE_Blended(font_, (const uint16_t *)text.data(), fg));
    }

    std::shared_ptr<SDLSurface> RenderUTF8_Blended(std::string_view text, const SDL_Color &fg) {
        return SDLSurface::Create(TTF_RenderUTF8_Blended(font_, text.data(), fg));
    }

    std::shared_ptr<SDLSurface>
    RenderText_Blended_Wrapped(std::string_view text, const SDL_Color &fg, Uint32 wrapLength) {
        return SDLSurface::Create(TTF_RenderText_Blended_Wrapped(font_, text.data(), fg, wrapLength));
    }

    std::shared_ptr<SDLSurface>
    RenderUNICODE_Blended_Wrapped(std::string_view text, const SDL_Color &fg, Uint32 wrapLength) {
        return SDLSurface::Create(
                TTF_RenderUNICODE_Blended_Wrapped(font_, (const uint16_t *)text.data(), fg, wrapLength));
    }

    std::shared_ptr<SDLSurface>
    RenderUTF8_Blended_Wrapped(std::string_view text, const SDL_Color &fg, Uint32 wrapLength) {
        return SDLSurface::Create(TTF_RenderUTF8_Blended_Wrapped(font_, text.data(), fg, wrapLength));
    }

    std::shared_ptr<SDLSurface> RenderGlyph_Blended(uint16_t ch, const SDL_Color &fg) {
        return SDLSurface::Create(TTF_RenderGlyph_Blended(font_, ch, fg));
    }

    std::shared_ptr<SDLSurface> RenderGlyph_Blended(uint32_t ch, const SDL_Color &fg) {
        return SDLSurface::Create(TTF_RenderGlyph32_Blended(font_, ch, fg));
    }


    std::shared_ptr<SDLSurface> RenderText_LCD(std::string_view text, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderText_LCD(font_, text.data(), fg, bg));
    }

    std::shared_ptr<SDLSurface> RenderUTF8_LCD(std::string_view text, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderUTF8_LCD(font_, text.data(), fg, bg));
    }

    std::shared_ptr<SDLSurface> RenderUNICODE_LCD(std::string_view text, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderUNICODE_LCD(font_, (const uint16_t *)text.data(), fg, bg));
    }

    std::shared_ptr<SDLSurface>
    RenderText_LCD_Wrapped(std::string_view text, const SDL_Color &fg, const SDL_Color &bg, Uint32 wrapLength) {
        return SDLSurface::Create(TTF_RenderText_LCD_Wrapped(font_, text.data(), fg, bg, wrapLength));
    }

    std::shared_ptr<SDLSurface>
    RenderUTF8_LCD_Wrapped(std::string_view text, const SDL_Color &fg, const SDL_Color &bg, Uint32 wrapLength) {
        return SDLSurface::Create(TTF_RenderUTF8_LCD_Wrapped(font_, text.data(), fg, bg, wrapLength));
    }

    std::shared_ptr<SDLSurface>
    RenderUNICODE_LCD_Wrapped(std::string_view text, const SDL_Color &fg, const SDL_Color &bg, Uint32 wrapLength) {
        return SDLSurface::Create(
                TTF_RenderUNICODE_LCD_Wrapped(font_, (const uint16_t *)text.data(), fg, bg, wrapLength));
    }

    std::shared_ptr<SDLSurface> RenderGlyph_LCD(uint32_t glyph, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderGlyph32_LCD(font_, glyph, fg, bg));
    }

    std::shared_ptr<SDLSurface> RenderGlyph_LCD(uint16_t glyph, const SDL_Color &fg, const SDL_Color &bg) {
        return SDLSurface::Create(TTF_RenderGlyph_LCD(font_, glyph, fg, bg));
    }

    using Direction = TTF_Direction;

    bool SetDirection(Direction direction) { return TTF_SetFontDirection(font_, direction) == 0; }

    // TTF_SetFontScriptName

    ~Font() {
        if(font_) {
            TTF_CloseFont(font_);
            font_       = nullptr;
            point_size_ = 0;
        }
    }

private:
    explicit Font(TTF_Font *&&font, int point_size) : font_{font}, point_size_(point_size) {}

    TTF_Font *font_{nullptr};
    int       point_size_{0};
};

} // namespace sdlpp

#endif // SDL2PP_BASE_SDLTTF_H_
