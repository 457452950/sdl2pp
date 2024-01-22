#include "GLObject.hpp"


// target must be either GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER or GL_FRAMEBUFFER
void FBO::Bind(BufferType type) {
    switch(type) {
    case READ:
        glBindFramebuffer(GL_READ_FRAMEBUFFER, id_);
        break;
    case DRAW:
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id_);
        break;
    case READ_WRITE:
        glBindFramebuffer(GL_FRAMEBUFFER, id_);
        break;
    }
}
