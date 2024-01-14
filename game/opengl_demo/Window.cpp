#include "Window.h"

#include "sdl2pp/common/SDLLog.h"
#include "base/Sysinfo.h"

namespace game {

std::once_flag glad_init_flag;

Window::Window() : sdlpp::SWindow(SDL_WINDOW_OPENGL) {
    this->SetSize({1200, 800});

    SDL_SetRelativeMouseMode(SDL_TRUE);

    // fixme: context SDL_GL_DeleteContext
    gl_context_ = SDL_GL_CreateContext(this->Get());
    if(gl_context_ == nullptr) {
        LOG_ERR(log::APP, "sdl gl create context false, {}", SDL_GetError());
        return;
    }

    if(SDL_GL_MakeCurrent(this->Get(), gl_context_) != 0) {
        LOG_ERR(log::APP, "SDL_GL_MakeCurrent false, {}", SDL_GetError());
        return;
    }

    sdlpp::gl::GL::GladInit();
    sdlpp::gl::GL::DepthTest();

    this->shader_ = std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\shader.vert)",
                                             R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\shader.frag)");

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // clang-format off
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    // clang-format on

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shader_->use();
}

Window::~Window() {
    if(this->gl_context_ != nullptr) {
        SDL_GL_DeleteContext(this->gl_context_);
        this->gl_context_ = nullptr;
    }
}

void Window::RenderProcess() {
    shader_->use();

    glm::mat4 projection = glm::perspective(
            glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 100.0f);
    shader_->setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera_->GetViewMatrix();
    shader_->setMat4("view", view);

    shader_->setMat4("model", glm::mat4(1.0f));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Window::RenderClear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::RenderFlush() { SDL_GL_SwapWindow(this->Get()); }

void Window::Tick(double_t tick_ms) {
    const double_t speed = 0.001;

    tick_ms *= speed;

    if(this->speed_x == 1) {
        camera_->ProcessKeyboard(FORWARD, tick_ms);
    } else if(this->speed_x == -1) {
        camera_->ProcessKeyboard(BACKWARD, tick_ms);
    }

    if(this->speed_y == 1) {
        camera_->ProcessKeyboard(LEFT, tick_ms);
    } else if(this->speed_y == -1) {
        camera_->ProcessKeyboard(RIGHT, tick_ms);
    }
}

} // namespace game