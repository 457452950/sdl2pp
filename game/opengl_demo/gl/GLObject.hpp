#pragma once
#ifndef SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_
#define SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_

#include <glad/glad.h>

#include "Toy/NonCopyAble.hpp"

class GLObject {
public:
    GLObject()  = default;
    ~GLObject() = default;

public:
    GLuint GetId() const { return id_; }

protected:
    GLuint id_{0};
};

class VAO final : public GLObject, public lbox::NonCopyAble {
public:
    VAO() { glGenVertexArrays(1, &id_); }
    ~VAO() { glDeleteVertexArrays(1, &id_); }

    void        Bind() { glBindVertexArray(id_); }
    static void Unbind() { glBindVertexArray(0); }
};

class VBO final : public GLObject, public lbox::NonCopyAble {
public:
    VBO() { glGenBuffers(1, &id_); }
    ~VBO() { glDeleteBuffers(1, &id_); }

    void Bind() { glBindBuffer(GL_ARRAY_BUFFER, id_); }
};

class EBO final : public GLObject, public lbox::NonCopyAble {
public:
    EBO() { glGenBuffers(1, &id_); }
    ~EBO() { glDeleteBuffers(1, &id_); }

    void Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }
};

class GLTexture final : public GLObject, public lbox::NonCopyAble {
public:
    GLTexture() { glGenTextures(1, &id_); }
    ~GLTexture() { glDeleteTextures(1, &id_); }

    void        Bind() { glBindTexture(GL_TEXTURE_2D, id_); }
    static void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
};

class GLTextureCubeMap final : public GLObject, public lbox::NonCopyAble {
public:
    GLTextureCubeMap() { glGenTextures(1, &id_); }
    ~GLTextureCubeMap() { glDeleteTextures(1, &id_); }

    void        Bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, id_); }
    static void Unbind() { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }
};


class FBO final : public GLObject, public lbox::NonCopyAble {
public:
    FBO() { glGenFramebuffers(1, &id_); }
    ~FBO() { glDeleteFramebuffers(1, &id_); }

    enum BufferType { READ, DRAW, READ_WRITE };

    void        Bind(BufferType type = READ_WRITE);
    static void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
};

class RBO final : public GLObject, public lbox::NonCopyAble {
public:
    RBO() { glGenRenderbuffers(1, &id_); }
    ~RBO() { glDeleteRenderbuffers(1, &id_); }

    void        Bind() { glBindRenderbuffer(GL_RENDERBUFFER, id_); }
    static void Unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }
};

#endif // SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_
