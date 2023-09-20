#include "SWindow.h"

#include <fmt/format.h>

#include "base/SDLRenderer.h"

namespace sdlpp {

    const SDL_Point SWindow::DEFAULT_SIZE = {640, 480};

    int SWindow::Exec() {
        active_.store(true);

        while (active_) {
            SDL_PollEvent(&event_);

            switch (static_cast<SDL_EventType>(event_.type)) {
                case SDL_QUIT: {
                    SDL_LogDebug(0, "SDL_QUIT");
                    break;
                }
                case SDL_DISPLAYEVENT: {
                    auto &ev = event_.display;
                    switch (static_cast<SDL_DisplayEventID>(ev.event)) {
                        case SDL_DISPLAYEVENT_NONE:
                            SDL_LogDebug(0, "display event: none index %d", ev.display);
                            break;
                        case SDL_DISPLAYEVENT_ORIENTATION:
                            SDL_LogDebug(0, "display event: orientation %d change to %d", ev.display, ev.data1);
                            break;
                        case SDL_DISPLAYEVENT_CONNECTED:
                            SDL_LogDebug(0, "display event: connected %d", ev.display);
                            break;
                        case SDL_DISPLAYEVENT_DISCONNECTED:
                            SDL_LogDebug(0, "display event: disconnected %d", ev.display);
                            break;
                    }
                    break;
                }
                case SDL_KEYUP:
                case SDL_KEYDOWN: {
                    if (event_.key.windowID == this->GetID())
                        this->KeyEvent(event_.key);
                    break;
                }
                case SDL_MOUSEMOTION: {
                    if (event_.motion.windowID == this->GetID())
                        this->MouseMoveEvent(event_.motion);
                    break;
                }
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN: {
                    if (event_.button.windowID == this->GetID())
                        this->MouseButtonEvent(event_.button);
                    break;
                }
                case SDL_MOUSEWHEEL: {
                    if (event_.wheel.windowID == this->GetID())
                        this->MouseWheelEvent(event_.wheel);
                    break;
                }
                case SDL_WINDOWEVENT: {
                    if (event_.key.windowID == this->GetID()) {
                        auto res = this->WindowEvent(event_.window);
                        if (res == Continue) {
                            DefaultWindowEvent(event_.window);
                        }
                    }
                    break;
                }
                default:
                    break;
            }

            if (renderer_) {
                renderer_->Clear();
                this->RenderProcess();
                renderer_->Flush();
            }
        }

        return 0;
    }

    void SWindow::Close() {
        this->active_.store(false);
    }

    void SWindow::DefaultWindowEvent(const SDL_WindowEvent &event) {
        switch (event.event) {
            case SDL_WINDOWEVENT_CLOSE: {
                SDL_LogDebug(0, "SDL_WINDOW EVENT close");
                this->Close();
                break;
            }
        }
    }

} // sdlpp