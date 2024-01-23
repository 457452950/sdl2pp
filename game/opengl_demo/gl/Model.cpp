#include <iostream>
#include "sdl2pp/common/SDLLog.h"
#include "sdl2pp/sdl//SDLImage.h"
#include "sdl2pp/sdl//SDLSurface.hpp"
#include "Model.hpp"
#include "fmt/printf.h"

namespace gl {

void Model::Draw(std::shared_ptr<Shader> shader) {
    for(auto &mesh : meshes)
        mesh.Draw(shader);
}

void Model::loadModel(std::string_view path) {
    Assimp::Importer import;
    const aiScene   *scene = import.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERR(log::APP, "ERROR::ASSIMP:: {}", import.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // 处理节点所有的网格（如果有的话）
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // 接下来对它的子节点重复这一过程
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex    vertex;
        // 处理顶点位置、法线和纹理坐标
        glm::vec3 vector;
        vector.x        = mesh->mVertices[i].x;
        vector.y        = mesh->mVertices[i].y;
        vector.z        = mesh->mVertices[i].z;
        vertex.Position = vector;

        // normals
        if(mesh->HasNormals()) {
            vector.x      = mesh->mNormals[i].x;
            vector.y      = mesh->mNormals[i].y;
            vector.z      = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if(mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
        {
            glm::vec2 vec;
            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // 处理索引
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // 处理材质
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial          *material    = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> reflectionMaps =
                loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflection");
        textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
    }

    //    fmt::print("{} {} {}\n", vertices.size(), indices.size(), textures.size());

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one.
                             // (optimization)
                break;
            }
        }
        if(!skip) { // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id   = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // store it as texture loaded for entire model, to ensure we won't
                                                // unnecessary load duplicate textures.
        }
    }
    return textures;
}

unsigned int TextureFromFile(std::string_view path, std::string_view directory, bool gamma) {
    std::string filename(directory);
    filename += "/";
    filename += path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    auto img = sdlpp::IMG_LoadSurfaceFromFile(filename);

    if(img) {
        GLint format;
        auto  f = img->GetFormat();
        if(f->BytesPerPixel == 1)
            format = GL_RED;
        else if(f->BytesPerPixel == 3)
            format = GL_RGB;
        else if(f->BytesPerPixel == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format,
                     img->GetWidth(),
                     img->GetHeight(),
                     0,
                     format,
                     GL_UNSIGNED_BYTE,
                     img->GetPixels());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        LOG_ERR(log::APP, "Texture failed to load at path: {}", filename);
    }

    LOG_INF(log::APP, "{} {}", filename, textureID);

    return textureID;
}

} // namespace gl