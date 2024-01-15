#include "GLWindow.hpp"

namespace sdlpp {

GlWindow::GlWindow() : sdlpp::SWindow(SDL_WINDOW_OPENGL) {
    sdlpp::gl::CreateContext(this->Get());
    sdlpp::gl::ext::Init();
}

void GlWindow::RenderFlush() { SDL_GL_SwapWindow(this->Get()); }


} // namespace sdlpp