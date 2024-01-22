#include "Window.h"

#include "sdl2pp/SDLpp.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace game {

Window::Window() {
    // Setup Dear ImGui context
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
    glEnable(GL_CULL_FACE);

    this->model_shader_ = std::make_shared<gl::Shader>();
    if(!this->model_shader_->SetShader(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\model_loading.vs)",
                                       R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\model_loading.fs)")) {
        LOG_ERR(log::APP, "load shader error ");
        return;
    }
    this->screen_shader_ = std::make_shared<gl::Shader>();
    if(!this->screen_shader_->SetShader(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\screen_shader.vs)",
                                        R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\screen_shader.fs)")) {
        LOG_ERR(log::APP, "load shader error ");
        return;
    }

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    model_ = std::make_shared<gl::Model>("H:/Code/CLion/sdl2pp/game/opengl_demo/nanosuit/nanosuit.obj");


    {
        screen_shader_->Use();
        screen_shader_->SetInt("screenTexture", 0);

        fbo_.Bind();

        // 生成纹理
        gl_texture_.Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 将它附加到当前绑定的帧缓冲对象
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl_texture_.GetId(), 0);

        rbo_.Bind();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 900);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_.GetId());
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOG_ERR(log::APP, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        }
        fbo_.Unbind();
    }

    static float quadVertices[] = {
            // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f};
    vao_.Bind();
    vbo_.Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Window::~Window() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Window::RenderProcess() {
    //    ImGui_ImplOpenGL3_NewFrame();
    //    ImGui_ImplSDL2_NewFrame();
    //    ImGui::NewFrame();
    //    this->IMGUIProcess();


    glm::mat4 projection = glm::perspective(
            glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 100.0f);

    // 第一处理阶段(Pass)
    fbo_.Bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 我们现在不使用模板缓冲
    {
        model_shader_->Use();
        {
            glm::mat4 view = camera_->GetViewMatrix();
            model_shader_->SetMat4("view", view);

            model_shader_->SetMat4("projection", projection);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            //        model           = glm::translate(model,
            //                               glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of
            //                               the scene
            //        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so
            //        scale it down
            model_shader_->SetMat4("model", model);
        }
        { model_->Draw(model_shader_); }
    }
    VAO::Unbind();

    // 第二处理阶段
    fbo_.Unbind(); // 返回默认
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    screen_shader_->Use();
    vao_.Bind();
    gl_texture_.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Window::RenderClear() {
    //    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::RenderFlush() {
    //    ImGui::Render();
    //    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
}

void Window::eventHandle(const SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    SWindow::eventHandle(event);
}

void Window::IMGUIProcess() {}

} // namespace game