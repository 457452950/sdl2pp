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
