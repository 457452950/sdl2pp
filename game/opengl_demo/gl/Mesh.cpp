#include <memory>

#include "sdl2pp/common/SDLLog.h"

#include "Mesh.hpp"

namespace gl {

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
    vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)) {
    vao_ = std::make_shared<VAO>();
    vbo_ = std::make_shared<VBO>();
    ebo_ = std::make_shared<EBO>();
    setupMesh();
}

void Mesh::setupMesh() {
    //    {
    //        static float vertices[] = {// positions          // normals           // texture coords
    //                                   -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f, -0.5f,
    //                                   0.0f, 0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,
    //                                   -1.0f, 1.0f,  1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,
    //                                   -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f,  -1.0f, 0.0f,  1.0f,  -0.5f, -0.5f, -0.5f,
    //                                   0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,
    //
    //                                   -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    //                                   0.0f, 0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    //                                   0.0f,  1.0f,  1.0f,  1.0f, 0.5f,  0.5f,  0.5f,  0.0f,
    //                                   0.0f,  1.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  1.0f,
    //                                   0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    //
    //                                   -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    //                                   -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    //                                   0.0f,  1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  -0.5f,
    //                                   -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f,
    //                                   0.0f,  0.0f,  1.0f,  0.0f,
    //
    //                                   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    //                                   -0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,
    //                                   0.0f,  0.0f,  1.0f, 0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    //                                   0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  0.0f,  0.5f,  0.5f,
    //                                   0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    //
    //                                   -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f,
    //                                   0.0f, -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f,
    //                                   0.0f,  1.0f,  0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,
    //                                   -0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f,  0.0f,  0.0f,  -0.5f, -0.5f, -0.5f,
    //                                   0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,
    //
    //                                   -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  -0.5f,
    //                                   0.0f, 1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
    //                                   0.0f,  1.0f,  0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    //                                   -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    //                                   0.0f,  1.0f,  0.0f,  0.0f,  1.0f};
    //
    //        static int indices[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17,
    //                                18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
    //        vao_->Bind();
    //        vbo_->Bind();
    //        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //        ebo_->Bind();
    //        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //
    //        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    //        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    //        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    //        glEnableVertexAttribArray(0);
    //        glEnableVertexAttribArray(1);
    //        glEnableVertexAttribArray(2);
    //    }
    //    return;

    vao_->Bind();
    vbo_->Bind();

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    ebo_->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(std::shared_ptr<Shader> shader) {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        //        LOG_DBG(log::APP, "material.{} {} ", name + number, textures[i].id);

        shader->SetInt("material." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // 绘制网格
    vao_->Bind();
    //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}


} // namespace gl