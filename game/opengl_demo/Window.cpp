#include "Window.h"

#include "sdl2pp/SDLpp.h"

namespace game {

Window::Window() {
    this->SetSize({1200, 900});
    //    glViewport(0, 0, 1200, 900);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    sdlpp::gl::ext::DepthTest();

    this->light_shader_ = std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightShader.vert)",
                                                   R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightShader.frag)");
    this->light_source_shader_ =
            std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightSourceShader.vert)",
                                     R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightSourceShader.frag)");

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // clang-format off
    float vertices[] = {
        // positions
         0.5f,  0.5f,  0.5f,    // 0
         0.5f,  0.5f, -0.5f,    // 1
         0.5f, -0.5f,  0.5f,    // 2
         0.5f, -0.5f, -0.5f,    // 3

        -0.5f,  0.5f,  0.5f,    // 4
        -0.5f,  0.5f, -0.5f,    // 5
        -0.5f, -0.5f,  0.5f,    // 6
        -0.5f, -0.5f, -0.5f,    // 7
    };
    unsigned int indices[] = {
        // front
        0, 1, 2,
        1, 2, 3,

        // back
        4, 5, 6,
        5, 6, 7,

        // up
        4, 0, 6,
        0, 6, 2,

        // bottom
        5, 1, 7,
        1, 3, 7,

        // left
        2, 3, 7,
        2, 6, 7,

        // right
        1, 4, 5,
        0, 1, 4,
    };
    // clang-format on

    {
        cube_vao.Bind();
        cube_vbo.Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        cube_ebo.Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
    {
        light_vao.Bind();
        cube_vbo.Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        cube_ebo.Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
}

Window::~Window() {}

void Window::RenderProcess() {
    light_shader_->use();
    {
        // set mvp
        glm::mat4 projection = glm::perspective(
                glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 100.0f);
        light_shader_->setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera_->GetViewMatrix();
        light_shader_->setMat4("view", view);

        light_shader_->setMat4("model", glm::mat4(1.0f));
    }
    {
        light_shader_->setVec3("inColor", {1, 0, 0});
        light_shader_->setVec3("inLight", {0.3, 0.3, 0.3});
    }
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    light_source_shader_->use();
    {
        // set mvp
        glm::mat4 projection = glm::perspective(
                glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 100.0f);
        light_source_shader_->setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera_->GetViewMatrix();
        light_source_shader_->setMat4("view", view);

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, lightPos);
        model      = glm::scale(model, glm::vec3(0.2f));
        light_source_shader_->setMat4("model", model);
    }
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Window::RenderClear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::RenderFlush() { GlWindow::RenderFlush(); }

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