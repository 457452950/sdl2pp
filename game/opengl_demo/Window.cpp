#include "Window.h"

#include "sdl2pp/common/SDLLog.h"

namespace game {

std::once_flag glad_init_flag;

Window::Window() : sdlpp::SWindow(SDL_WINDOW_OPENGL) {
    this->SetPhysicPerS(0);

    auto context = SDL_GL_CreateContext(this->Get());
    if(!context) {
        LOG_ERR(log::APP, "sdl gl create context false, {}", SDL_GetError());
        return;
    }
    if(SDL_GL_MakeCurrent(this->Get(), context) != 0) {
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

    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

    float vertices[] = {
            -0.5f,
            -0.5f,
            0.0f, // left
            0.5f,
            -0.5f,
            0.0f, // right
            0.0f,
            0.5f,
            0.0f, // top

            //                        0.5f,
            //                        0.5f,
            //                        0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

void Window::RenderProcess() {
    glClear(GL_COLOR_BUFFER_BIT);

    //    glBegin(GL_QUADS);
    //    glColor3f(1.0f, 0.0f, 0.0f);
    //    glVertex2f(-0.5f, -0.5f);
    //    glVertex2f(0.5f, -0.5f);
    //    glVertex2f(0.5f, 0.5f);
    //    glVertex2f(-0.5f, 0.5f);
    //    glEnd();

    //     添加数据

    // 调用几何着色器命令

    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制0,1,2连城的三角

    SDL_GL_SwapWindow(this->Get());
}

void Window::Tick(double_t tick_ms) {}

} // namespace game