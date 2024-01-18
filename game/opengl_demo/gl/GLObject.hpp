#pragma once
#ifndef SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_
#define SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_

#include <glad/glad.h>

class VAO final {
public:
    VAO();
    ~VAO();

    void Bind();

private:
    GLuint id_{0};
};

class VBO final {
public:
    VBO();
    ~VBO();

    void Bind();

private:
    GLuint id_{0};
};

class EBO final {
public:
    EBO();
    ~EBO();

    void Bind();

private:
    GLuint id_{0};
};

class GLTexture final {
public:
    GLTexture();
    ~GLTexture();

    void Bind();

private:
    GLuint id_{0};
};

#endif // SDL2PP_GAME_OPENGL_DEMO_GL_GLOBJECT_HPP_
