#include "Window.h"

#include "sdl2pp/SDLpp.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace game {

Window::Window() { // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io     = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(this->Get(), SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init();

    this->SetSize({1200, 900});
    //    this->SetFps(120);

    //    SDL_SetRelativeMouseMode(SDL_TRUE);

    sdlpp::gl::ext::DepthTest();
    // 背面剔除
    //    glEnable(GL_CULL_FACE);

    this->light_shader_ = std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightShader.vert)",
                                                   R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightShader.frag)");
    this->light_source_shader_ =
            std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightSourceShader.vert)",
                                     R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightSourceShader.frag)");

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 1.5f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
}

Window::~Window() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Window::RenderProcess() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    this->IMGUIProcess();

    glm::mat4 projection = glm::perspective(
            glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 100.0f);

    light_shader_->use();
    {
        // set mvp

        // camera/view transformation
        glm::mat4 view = camera_->GetViewMatrix();
        light_shader_->setMat4("view", view);

        light_shader_->setMat4("model", glm::mat4(1.0f));
        light_shader_->setMat4("projection", projection);
    }
    {
        light_shader_->setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
        light_shader_->setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
        light_shader_->setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
        light_shader_->setFloat("material.shininess", 32.0f);

        light_shader_->setVec3("light.ambient", glm::vec3{1.0f});
        light_shader_->setVec3("light.diffuse", glm::vec3{1.0f}); // 将光照调暗了一些以搭配场景
        light_shader_->setVec3("light.specular", glm::vec3{1.0f});
        light_shader_->setVec3("light.position", lightPos);

        light_shader_->setVec3("viewPos", camera_->Position);
        light_shader_->setBool("ambient_enable_", ambient_enable_);
        light_shader_->setBool("diffuse_enable_", diffuse_enable_);
        light_shader_->setBool("specular_enable_", specular_enable_);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    light_source_shader_->use();
    {
        // set mvp
        light_source_shader_->setMat4("projection", projection);

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

void Window::RenderFlush() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    GlWindow::RenderFlush();
}

void Window::Tick(double_t tick_ms) {
    const double_t move_speed = 0.001;

    tick_ms *= move_speed;

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


    theta      += this->speed * 0.01;
    lightPos.x  = r * cos(theta);
    lightPos.z  = r * sin(theta);
}

void Window::eventHandle(const SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    SWindow::eventHandle(event);
}

void Window::IMGUIProcess() {
    ImGui::SliderFloat("ambientLight", &ambientLight, 0.0f, 1.0f);
    ImGui::SliderFloat("diffuseLight", &diffuseLight, 0.0f, 1.0f);
    ImGui::SliderFloat("r", &r, 1.0f, 5.0f);
    ImGui::SliderFloat("speed", &speed, 0.0f, 10.0f);
    ImGui::Checkbox("ambient", &ambient_enable_);
    ImGui::Checkbox("diffuse", &diffuse_enable_);
    ImGui::Checkbox("specular", &specular_enable_);
}

} // namespace game