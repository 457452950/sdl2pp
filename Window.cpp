#include "Window.h"

#include "sdl2pp/SDLpp.h"

namespace game {
Window::Window() {

    auto render = this->CreateRenderer();

    SDL_RendererInfo info;
    render->GetRendererInfo(&info);
    SDL_Log("Renderer name: %s", info.name);
    if(info.flags & SDL_RENDERER_SOFTWARE) {
        SDL_Log("Renderer is software");
    }
    if(info.flags & SDL_RENDERER_ACCELERATED) {
        SDL_Log("Renderer is accelerated");
    }
    if(info.flags & SDL_RENDERER_PRESENTVSYNC) {
        SDL_Log("Renderer supports vsync");
    }
    if(info.flags & SDL_RENDERER_TARGETTEXTURE) {
        SDL_Log("Renderer supports render to texture");
    }
    SDL_Log("Renderer available %d texture formats.", info.num_texture_formats);
    for(int ii = 0; ii < info.num_texture_formats; ++ii) {
        SDL_Log("Renderer texture format %d: %s", ii, SDL_GetPixelFormatName(info.texture_formats[ii]));
    }
    SDL_Log("Renderer max texture width: %d", info.max_texture_width);
    SDL_Log("Renderer max texture height: %d", info.max_texture_height);

    font_ = sdlpp::Font::Open(R"(H:\Resources\ttf\hk4e_zh-cn.ttf)", 64);
    if(font_ == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load font: %s", SDL_GetError());
        return;
    }

    auto surface_image = sdlpp::IMG_LoadSurfaceFromFile("H:/Resources/cos.jpg");
    if(surface_image == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load image: %s", SDL_GetError());
        return;
    }

    surface_image = sdlpp::ConvertSurface(surface_image, *this->GetPixelFormat());

    auto texture = this->GetRenderer()->CreateTextureFromSurface(surface_image);
    if(texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "One or more textures failed to load. %s\n", SDL_GetError());
        SDL_assert(texture);
    }

    texture_image_  = std::make_shared<sdlpp::STexture>(texture);
    texture_image2_ = std::make_shared<sdlpp::STexture>(texture);

    auto surface_image2 = sdlpp::IMG_LoadSurfaceFromFile("H:/Resources/stand.png");
    if(surface_image2 == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load image: %s", SDL_GetError());
        return;
    }

    surface_image2 = sdlpp::ConvertSurface(surface_image2, *this->GetPixelFormat());

    //        surface_image2->SetColorKey(true, {255, 255, 255});

    texture = this->GetRenderer()->CreateTextureFromSurface(surface_image2);
    if(texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "One or more textures failed to load. %s\n", SDL_GetError());
        SDL_assert(texture);
    }
    texture_man_ = std::make_shared<sdlpp::STexture>(texture);

    texture_man_->GetTexture()->SetBlendMode(values[index]);
    texture_image_->GetTexture()->SetBlendMode(values[index]);

    //    this->ViewMoveTo({40, 0});

    texture_image_->SetScale({this->GetWidth() / (double)texture_image_->GetTexture()->GetWidth(),
                              this->GetHeight() / (double)texture_image_->GetTexture()->GetHeight()});

    texture_image2_->MoveTo({80, 80});
    texture_image2_->SetRotation(45);
    texture_image2_->SetRotationCenter({50, 50});
    texture_image2_->SetScale({100.0f / (double)texture_image2_->GetTexture()->GetWidth(),
                               100.0f / (double)texture_image2_->GetTexture()->GetHeight()});
}

void Window::RenderProcess(SDL_FPoint view_pos, double view_angle) {
    int         m_x, m_y;
    auto        s    = SDL_GetMouseState(&m_x, &m_y);
    std::string text = fmt::format("{}, {} : {}", s, m_x, m_y);

    SDL_Color color     = {255, 255, 255, 200};
    auto      txt_image = font_->RenderText_Solid(text, color);
    if(txt_image == nullptr) {
        SDL_Log("render text error %s", SDL_GetError());
        SDL_assert(txt_image);
        return;
    }
    // Get the texture w/h so we can center it in the screen
    auto [iW, iH] = txt_image->GetSize();
    int x         = this->GetWidth() / 2 - iW / 2;
    int y         = this->GetHeight() / 2 - iH / 2;

    auto renderer = this->GetRenderer();

    texture_image_->Render(renderer, view_pos, view_angle, {1, 1});

    texture_image2_->Render(renderer, {80, 80}, 0, {1, 1});

    SDL_Rect rect = {x, y, iW, iH};
    this->GetRenderer()->Update(this->GetRenderer()->CreateTextureFromSurface(txt_image), nullptr, &rect);
}

} // namespace game