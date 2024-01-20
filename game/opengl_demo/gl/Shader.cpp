#include "Shader.hpp"

#include <memory>
#include <sstream>

#include "sdl2pp/common/SDLLog.h"


namespace gl {

void Shader::Use() const { glUseProgram(id_); }

static constexpr int LOG_LEN = 1024;

bool Shader::checkCompileError(GLuint shader) const {
    GLint  success{0};
    GLchar _info_log[LOG_LEN];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success == 0) {
        glGetShaderInfoLog(shader, LOG_LEN, nullptr, _info_log);
        LOG_ERR(log::APP, "ERROR::SHADER_COMPILATION_ERROR of {} {}", shader, _info_log);
        return false;
    }

    return true;
}

bool Shader::checkLinkError() const {
    GLint  success{0};
    GLchar _info_log[LOG_LEN];

    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if(success == 0) {
        glGetProgramInfoLog(id_, LOG_LEN, nullptr, _info_log);
        LOG_ERR(log::APP, "ERROR::PROGRAM_LINKING_ERROR of {}", _info_log);
        return false;
    }
    return true;
}

Shader::Shader() { id_ = glCreateProgram(); }

bool Shader::SetShader(std::string_view vertex_shader, std::string_view fragment_shader) {
    std::ifstream v_file;
    std::ifstream f_file;
    v_file.open(vertex_shader.data());
    f_file.open(fragment_shader.data());
    if(!v_file.is_open()) {
        return false;
    }
    if(!f_file.is_open()) {
        return false;
    }

    std::string vertex_source((std::istreambuf_iterator<char>(v_file)), std::istreambuf_iterator<char>());
    std::string fragment_source((std::istreambuf_iterator<char>(f_file)), std::istreambuf_iterator<char>());

    LOG_DBG(log::APP, "source {}", vertex_source);
    LOG_DBG(log::APP, "source {}", fragment_source);

    const char *v_source_c = vertex_source.c_str();
    const char *f_source_c = fragment_source.c_str();

    // vertex shader
    auto v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &v_source_c, nullptr);
    glCompileShader(v);
    if(checkCompileError(v) == false)
        return false;


    // fragment shader
    auto f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &f_source_c, nullptr);
    glCompileShader(f);
    if(checkCompileError(f) == false)
        return false;

    // link
    glAttachShader(id_, v);
    glAttachShader(id_, f);
    glLinkProgram(id_);
    if(checkLinkError() == false) {
        return false;
    }

    glDeleteShader(v);
    glDeleteShader(f);

    // ok
    ready_ = true;
    return true;
}

bool Shader::Ready() const { return ready_; }

Shader::~Shader() { glDeleteProgram(id_); }

} // namespace gl