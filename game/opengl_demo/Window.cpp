#include "Window.h"

#include "sdl2pp/SDLpp.h"

namespace game {

Window::Window() {
    this->SetSize({1200, 900});
    this->SetFps(120);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    sdlpp::gl::ext::DepthTest();
    // 背面剔除
    //    glEnable(GL_CULL_FACE);

    this->light_shader_ = std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightShader.vert)",
                                                   R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightShader.frag)");
    this->light_source_shader_ =
            std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightSourceShader.vert)",
                                     R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightSourceShader.frag)");

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // clang-format on

    {
        cube_vao.Bind();
        cube_vbo.Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
    {
        light_vao.Bind();
        cube_vbo.Bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

    glm::mat4 projection = glm::perspective(
            glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 100.0f);
    light_shader_->use();
    light_shader_->setMat4("projection", projection);
    light_source_shader_->use();
    light_source_shader_->setMat4("projection", projection);
}

Window::~Window() {}

void Window::RenderProcess() {
    light_shader_->use();
    {
        // set mvp

        // camera/view transformation
        glm::mat4 view = camera_->GetViewMatrix();
        light_shader_->setMat4("view", view);

        light_shader_->setMat4("model", glm::mat4(1.0f));
    }
    {
        light_shader_->setVec3("cubeColor", {0, 1, 0});
        light_shader_->setFloat("ambientLight", 0.1);
        light_shader_->setFloat("diffuseLight", 1);
        light_shader_->setVec3("lightPos", lightPos);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    light_source_shader_->use();
    {
        // set mvp

        // camera/view transformation
        glm::mat4 view = camera_->GetViewMatrix();
        light_source_shader_->setMat4("view", view);


        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, lightPos);
        model      = glm::scale(model, glm::vec3(0.2f));
        light_source_shader_->setMat4("model", model);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Window::RenderClear() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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