#include "GLObject.hpp"

VAO::VAO() { glGenVertexArrays(1, &id_); }

VAO::~VAO() { glDeleteVertexArrays(1, &id_); }

void VAO::Bind() { glBindVertexArray(id_); }

VBO::VBO() { glGenBuffers(1, &id_); }

VBO::~VBO() { glDeleteBuffers(1, &id_); }

void VBO::Bind() { glBindBuffer(GL_ARRAY_BUFFER, id_); }

EBO::EBO() { glGenBuffers(1, &id_); }

EBO::~EBO() { glDeleteBuffers(1, &id_); }

void EBO::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }

GLTexture::GLTexture() { glGenTextures(1, &id_); }

GLTexture::~GLTexture() { glDeleteTextures(1, &id_); }

void GLTexture::Bind() { glBindTexture(GL_TEXTURE_2D, id_); }

FBO::FBO() { glGenFramebuffers(1, &id_); }

FBO::~FBO() { glDeleteFramebuffers(1, &id_); }

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
