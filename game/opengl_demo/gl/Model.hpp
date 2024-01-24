#pragma once
#ifndef SDL2PP_GAME_OPENGL_DEMO_GL_MODEL_HPP_
#define SDL2PP_GAME_OPENGL_DEMO_GL_MODEL_HPP_

#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"
#include "Mesh.hpp"

namespace gl {

unsigned int TextureFromFile(std::string_view path, std::string_view directory, bool gamma = false);

class Model {
public:
    /*  函数   */
    explicit Model(std::string_view path) { loadModel(path); }

    void Draw(std::shared_ptr<Shader> shader);

    std::vector<Mesh> meshes;

private:
    /*  模型数据  */
    std::string          directory;
    std::vector<Texture> textures_loaded;

    /*  函数   */
    void                 loadModel(std::string_view path);
    void                 processNode(aiNode *node, const aiScene *scene);
    Mesh                 processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

} // namespace gl

#endif // SDL2PP_GAME_OPENGL_DEMO_GL_MODEL_HPP_
