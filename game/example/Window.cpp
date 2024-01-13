#include "Window.h"
#include "sdl2pp/utils/Assert.h"

namespace game {

sdlpp::PointD a{233, 324};
sdlpp::PointD b{433, 124};
sdlpp::PointD c{133, 321};
sdlpp::Circle cir{};

Window::Window() {
    auto render = this->CreateRenderer();

    font_ = sdlpp::Font::Open(R"(H:\Resources\ttf\hk4e_zh-cn.ttf)", 64);
    if(font_ == nullptr) {
        LOG_ERR(log::APP, "Could not load font: {}", SDL_GetError());
        return;
    }

    auto surface_image = sdlpp::IMG_LoadSurfaceFromFile("H:/Resources/image/mario.png");
    if(surface_image == nullptr) {
        LOG_ERR(log::APP, "Could not load image: {}", SDL_GetError());
        return;
    }

    surface_image = sdlpp::ConvertSurface(surface_image, *this->GetPixelFormat());
    auto t2       = surface_image->Flip(sdlpp::Flip ::SDL_FLIP_HORIZONTAL);
    auto t3       = surface_image->Flip(sdlpp::Flip ::SDL_FLIP_VERTICAL);
    auto t4       = surface_image->Flip(sdlpp::Flip ::SDL_FLIP_VERTICAL | sdlpp::Flip::SDL_FLIP_HORIZONTAL);

    auto texture = this->GetRenderer()->CreateTextureFromSurface(surface_image);
    Assert(texture, "One or more textures failed to load. {}", SDL_GetError());
    texture_image_ = std::make_shared<sdlpp::STexture>(texture);

    texture = this->GetRenderer()->CreateTextureFromSurface(t2);
    Assert(texture, "One or more textures failed to load. {}", SDL_GetError());
    texture_image2_ = std::make_shared<sdlpp::STexture>(texture);

    texture = this->GetRenderer()->CreateTextureFromSurface(t3);
    Assert(texture, "One or more textures failed to load. {}", SDL_GetError());
    texture_image3_ = std::make_shared<sdlpp::STexture>(texture);

    texture = this->GetRenderer()->CreateTextureFromSurface(t4);
    Assert(texture, "One or more textures failed to load. {}", SDL_GetError());
    texture_image4_ = std::make_shared<sdlpp::STexture>(texture);

    auto surface_image2 = sdlpp::IMG_LoadSurfaceFromFile("H:/Resources/stand.png");
    if(surface_image2 == nullptr) {
        LOG_ERR(log::APP, "Could not load image: {}", SDL_GetError());
        return;
    }

    surface_image2 = sdlpp::ConvertSurface(surface_image2, *this->GetPixelFormat());

    //        surface_image2->SetColorKey(true, {255, 255, 255});

    texture = this->GetRenderer()->CreateTextureFromSurface(surface_image2);
    Assert(texture, "One or more textures failed to load. {}", SDL_GetError());
    texture_man_ = std::make_shared<sdlpp::STexture>(texture);

    texture_man_->GetTexture()->SetBlendMode(values[index]);
    texture_image_->GetTexture()->SetBlendMode(values[index]);

    //    this->ViewMoveTo({40, 0});

    texture_image_->SetScale({this->GetWidth() / (double)texture_image_->GetTexture()->GetWidth() / 2,
                              this->GetHeight() / (double)texture_image_->GetTexture()->GetHeight() / 2});
    texture_image2_->SetScale({this->GetWidth() / (double)texture_image_->GetTexture()->GetWidth() / 2,
                               this->GetHeight() / (double)texture_image_->GetTexture()->GetHeight() / 2});
    texture_image3_->SetScale({this->GetWidth() / (double)texture_image_->GetTexture()->GetWidth() / 2,
                               this->GetHeight() / (double)texture_image_->GetTexture()->GetHeight() / 2});
    texture_image4_->SetScale({this->GetWidth() / (double)texture_image_->GetTexture()->GetWidth() / 2,
                               this->GetHeight() / (double)texture_image_->GetTexture()->GetHeight() / 2});

    texture_image2_->MoveTo({static_cast<float>(this->GetWidth() / 2), 0});
    texture_image3_->MoveTo({0, static_cast<float>(this->GetHeight() / 2)});
    texture_image4_->MoveTo({static_cast<float>(this->GetWidth() / 2), static_cast<float>(this->GetHeight() / 2)});

    music = sdlpp::MixMusic::Load("H:/Code/CLion/sdl2pp/demo/beat.wav");
    Assert(music->Valid(), "One or more music failed to load. {}", SDL_GetError());

    chunk1 = sdlpp::MixChunk::LoadWAV("H:/Code/CLion/sdl2pp/demo/scratch.wav");
    Assert(chunk1->Valid(), "load chunk fail, {}", SDL_GetError());

    chunk2 = sdlpp::MixChunk::LoadWAV("H:/Code/CLion/sdl2pp/demo/high.wav");
    Assert(chunk2->Valid(), "load chunk fail, {}", SDL_GetError());

    chunk3 = sdlpp::MixChunk::LoadWAV("H:/Code/CLion/sdl2pp/demo/medium.wav");
    Assert(chunk3->Valid(), "load chunk fail, {}", SDL_GetError());

    chunk4 = sdlpp::MixChunk::LoadWAV("H:/Code/CLion/sdl2pp/demo/low.wav");
    Assert(chunk4->Valid(), "load chunk fail, {}", SDL_GetError());

    auto man_surface = sdlpp::IMG_LoadSurfaceFromFile(R"(H:\Code\CLion\sdl2pp\demo\foo.png)");
    man_surface      = sdlpp::ConvertSurface(man_surface, *this->GetPixelFormat());
    man_surface->SetColorKey(true, {0, 255, 255});

    auto man_texture = render->CreateTextureFromSurface(man_surface);
    animation.AddFrame(man_texture, sdlpp::RectI{0, 0, 64, 205});
    animation.AddFrame(man_texture, sdlpp::RectI{64, 0, 64, 205});
    animation.AddFrame(man_texture, sdlpp::RectI{128, 0, 64, 205});
    animation.AddFrame(man_texture, sdlpp::RectI{192, 0, 64, 205});

    animation.MoveTo({100, 100});

    //    this->SetFps(1000);
    //    this->SetPhysicPerS(30);

    //    this->SetSize({1200, 800});

    cir = sdlpp::Circle::EnclosePoints({a, b, c});

    LOG_INF(log::APP,
            "{} {} {} {} ",
            cir.r,
            sdlpp::Distance(a, cir.center),
            sdlpp::Distance(b, cir.center),
            sdlpp::Distance(c, cir.center));
}

void Window::RenderProcess() {
    this->GetRenderer()->Clear();

    int         m_x, m_y;
    auto        s    = SDL_GetMouseState(&m_x, &m_y);
    std::string text = fmt::format("{}, {} : {}", s, m_x, m_y);

    SDL_Color color     = {170, 170, 30, 0};
    auto      txt_image = font_->RenderText_Solid(text, color);
    Assert(txt_image, "render text error {}", SDL_GetError());

    // Get the texture w/h so we can center it in the screen
    auto [iW, iH] = txt_image->GetSize();
    int x         = this->GetWidth() / 2 - iW / 2;
    int y         = this->GetHeight() / 2 - iH / 2;

    auto renderer = this->GetRenderer();

    sdlpp::RectI rect = sdlpp::RectI{x, y, iW, iH};
    this->GetRenderer()->Update(this->GetRenderer()->CreateTextureFromSurface(txt_image), nullptr, &rect);

    animation.Render(renderer, {}, 0, {1, 1});

    renderer->SetDrawColor({255, 255, 255});

    renderer->DrawLine(sdlpp::PointF(a), sdlpp::PointF(cir.center));
    renderer->DrawLine(sdlpp::PointF(b), sdlpp::PointF(cir.center));
    renderer->DrawLine(sdlpp::PointF(c), sdlpp::PointF(cir.center));

    texture_image_->Render(renderer, {}, 0, {1, 1});
    texture_image2_->Render(renderer, {}, 0, {1, 1});
    texture_image3_->Render(renderer, {}, 0, {1, 1});
    texture_image4_->Render(renderer, {}, 0, {1, 1});

    //    this->SetResizeable(true);
}

void Window::Tick(double_t tick_ms) {
    double pfs = (1000.0) / (tick_ms);
    LOG_VER(log::APP, "ms {:.4f},pps {:.3f}", tick_ms, pfs);

    static int animation_tick = 0;
    animation_tick++;
    if(animation_tick == 10) {
        this->animation.Update();
        animation_tick = 0;
    }
}

void Window::RenderFlush() { this->GetRenderer()->Flush(); }

} // namespace game