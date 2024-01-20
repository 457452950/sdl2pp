#pragma once
#ifndef SDL2PP_GAME_OPENGL_DEMO_GL_SHADER_HPP_
#define SDL2PP_GAME_OPENGL_DEMO_GL_SHADER_HPP_

#include <glad/glad.h>

#include <string>
#include <string_view>

#include <glm/glm.hpp>

namespace gl {

class Shader final {
public:
    Shader();
    ~Shader();

    bool SetShader(std::string_view vertex_shader, std::string_view fragment_shader);

    bool Ready() const;
    void Use() const;

    // utility uniform functions
    // ------------------------------------------------------------------------
    void SetBool(const std::string_view name, bool value) const {
        glUniform1i(glGetUniformLocation(id_, name.data()), static_cast<int>(value));
    }
    // ------------------------------------------------------------------------
    void SetInt(const std::string_view name, int value) const {
        glUniform1i(glGetUniformLocation(id_, name.data()), value);
    }

    // ------------------------------------------------------------------------
    void SetFloat(const std::string_view name, float value) const {
        glUniform1f(glGetUniformLocation(id_, name.data()), value);
    }
    // ------------------------------------------------------------------------
    void SetVec2(const std::string_view name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(id_, name.data()), 1, &value[0]);
    }
    void SetVec2(const std::string_view name, float x, float y) const {
        glUniform2f(glGetUniformLocation(id_, name.data()), x, y);
    }
    // ------------------------------------------------------------------------
    void SetVec3(const std::string_view name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(id_, name.data()), 1, &value[0]);
    }
    void SetVec3(const std::string_view name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(id_, name.data()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void SetVec4(const std::string_view name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(id_, name.data()), 1, &value[0]);
    }
    void SetVec4(const std::string_view name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(id_, name.data()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void SetMat2(const std::string_view name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void SetMat3(const std::string_view name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void SetMat4(const std::string_view name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, &mat[0][0]);
    }

protected:
    bool checkCompileError(GLuint shader) const;

    bool checkLinkError() const;

private:
    GLuint id_{0};
    bool   ready_{false};
};

} // namespace gl

#endif // SDL2PP_GAME_OPENGL_DEMO_GL_SHADER_HPP_
