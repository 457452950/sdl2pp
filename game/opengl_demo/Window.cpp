#include "Window.h"

#include "sdl2pp/common/SDLLog.h"

namespace game {

std::once_flag glad_init_flag;

Window::Window() : sdlpp::SWindow(SDL_WINDOW_OPENGL) {
    this->SetPhysicPerS(0);

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

    std::call_once(glad_init_flag, []() {
        // 初始化GLAD
        if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            LOG_ERR(log::APP, "gladLoadGLLoader false.");
            return;
        }
    });

    this->shader_ = std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\shader.vert)",
                                             R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\shader.frag)");

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

    // clang-format off
    float vertices[] = {-0.5f, -0.5f, 0.0f,    1,  0,  0,
                        0.5f,  -0.5f, 0.0f,    0,  1,  0,
                        0.0f,   0.5f, 0.0f,    0,  0,  1};
    // clang-format on

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Window::RenderClear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::RenderFlush() { SDL_GL_SwapWindow(this->Get()); }

void Window::Tick(double_t tick_ms) {}

} // namespace game