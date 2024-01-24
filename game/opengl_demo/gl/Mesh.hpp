#pragma once
#ifndef SDL2PP_GAME_OPENGL_DEMO_GL_MESH_HPP_
#define SDL2PP_GAME_OPENGL_DEMO_GL_MESH_HPP_

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/types.h>

#include "GLObject.hpp"
#include "Shader.hpp"

namespace gl {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string  type;
    aiString     path; // 我们储存纹理的路径用于与其它纹理进行比较
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(std::shared_ptr<Shader> shader);

    std::shared_ptr<VAO>      vao_;
    std::vector<unsigned int> indices;

private:
    std::shared_ptr<VBO> vbo_;
    std::shared_ptr<EBO> ebo_;

    std::vector<Vertex>  vertices;
    std::vector<Texture> textures;

    void setupMesh();
};

} // namespace gl

#endif // SDL2PP_GAME_OPENGL_DEMO_GL_MESH_HPP_
