#pragma once
#ifndef SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_
#define SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_

#include <glad/glad.h>

#include "Toy/NonCopyAble.hpp"

class VAO final : public lbox::NonCopyAble {
public:
    VAO();
    ~VAO();

    void Bind();

private:
    GLuint id_{0};
};

class VBO final : public lbox::NonCopyAble {
public:
    VBO();
    ~VBO();

    void Bind();

private:
    GLuint id_{0};
};

class EBO final : public lbox::NonCopyAble {
public:
    EBO();
    ~EBO();

    void Bind();

private:
    GLuint id_{0};
};

class GLTexture final : public lbox::NonCopyAble {
public:
    GLTexture();
    ~GLTexture();

    void Bind();

private:
    GLuint id_{0};
};

class FBO final : public lbox::NonCopyAble {
public:
    FBO();
    ~FBO();

    enum BufferType { READ, DRAW, READ_WRITE };

    void Bind(BufferType type = READ_WRITE);

private:
    GLuint id_{0};
};

#endif // SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_
