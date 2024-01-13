#include "SWindow.hpp"

#include <thread>

#include <fmt/format.h>

#include "sdl2pp/sdl/SDLRenderer.hpp"

namespace sdlpp {

const PointI SWindow::DEFAULT_SIZE = {640, 480};

int SWindow::Exec() {
    active_.store(true);

    // first render
    this->render();

    current_physic_time_ = current_time_ = sdlpp::GetPerformanceCounter();

    while(active_) {
        SDL_PollEvent(&event_);

        this->eventHandle(event_);

        if(!active_) {
            break;
        }

        if(frame_delay_mics_ > 0) {
            while(true) {
                CheckPhysicFrame();

                auto pass = sdlpp::GetPerformanceCounter() - current_time_;
                if((pass >= frame_delay_mics_)) {
                    current_time_ += pass;
                    break;
                }
            }
        } else {
            CheckPhysicFrame();
        }

        this->render();
    }

    return 0;
}

void SWindow::Close() { this->active_.store(false); }

void SWindow::DefaultWindowEvent(const SDL_WindowEvent &event) {
    switch(event.event) {
    case SDL_WINDOWEVENT_CLOSE: {
        LOG_INF(log::LIB, "SDL_WINDOW EVENT close.");
        this->Close();
        break;
    }
    }
}

void SWindow::CheckPhysicFrame() {
    if(physic_delay_micrs_ == 0) {
        return;
    }

    auto pass_time = (sdlpp::GetPerformanceCounter() - current_physic_time_);
    if((double)pass_time >= physic_delay_micrs_) {
        this->Tick((double)pass_time / (double)sdlpp::GetPerformanceFrequency() * 1000.0);
        current_physic_time_ += pass_time;
    }
}

void SWindow::eventHandle(const SDL_Event &event) {
    switch(static_cast<SDL_EventType>(event_.type)) {
    case SDL_QUIT: {
        LOG_INF(log::LIB, "SDL_QUIT");
        break;
    }
    case SDL_DISPLAYEVENT: {
        auto &ev = event_.display;
        switch(static_cast<SDL_DisplayEventID>(ev.event)) {
        case SDL_DISPLAYEVENT_NONE:
            LOG_INF(log::LIB, "display event: none index {}", ev.display);
            break;
        case SDL_DISPLAYEVENT_ORIENTATION:
            LOG_INF(log::LIB, "display event: orientation {} change to {}", ev.display, ev.data1);
            break;
        case SDL_DISPLAYEVENT_CONNECTED:
            LOG_INF(log::LIB, "display event: connected {}", ev.display);
            break;
        case SDL_DISPLAYEVENT_DISCONNECTED:
            LOG_INF(log::LIB, "display event: disconnected {}", ev.display);
            break;
        }
        break;
    }
    case SDL_KEYUP:
    case SDL_KEYDOWN: {
        if(event_.key.windowID == this->GetID())
            this->KeyEvent(event_.key);
        break;
    }
    case SDL_MOUSEMOTION: {
        if(event_.motion.windowID == this->GetID())
            this->MouseMoveEvent(event_.motion);
        break;
    }
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN: {
        if(event_.button.windowID == this->GetID())
            this->MouseButtonEvent(event_.button);
        break;
    }
    case SDL_MOUSEWHEEL: {
        if(event_.wheel.windowID == this->GetID())
            this->MouseWheelEvent(event_.wheel);
        break;
    }
    case SDL_WINDOWEVENT: {
        if(event_.key.windowID == this->GetID()) {
            auto res = this->WindowEvent(event_.window);
            if(res == Continue) {
                DefaultWindowEvent(event_.window);
            }
        }
        break;
    }
    default:
        break;
    }
}

} // namespace sdlpp