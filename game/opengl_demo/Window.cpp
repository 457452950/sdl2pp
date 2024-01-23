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
    this->sky_box_shader_ = std::make_shared<gl::Shader>();
    if(!this->sky_box_shader_->SetShader(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\sky_box_shader.vs)",
                                         R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\sky_box_shader.fs)")) {
        LOG_ERR(log::APP, "load shader error ");
        return;
    }
    sky_box_shader_->Use();
    sky_box_shader_->SetInt("skybox", 0);

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    model_ = std::make_shared<gl::Model>("H:/Code/CLion/sdl2pp/game/opengl_demo/nanosuit/nanosuit.obj");


    {
        sky_box_.Bind();
        static std::string              path = "H:/Resources/image/skybox/";
        static std::vector<std::string> faces{
                "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};
        for(unsigned int i = 0; i < faces.size(); i++) {
            auto img = sdlpp::IMG_LoadSurfaceFromFile(path + faces[i]);
            if(img) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,
                             GL_RGB,
                             img->GetWidth(),
                             img->GetHeight(),
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             img->GetPixels());

            } else {
                LOG_ERR(log::APP, "Cubemap texture failed to load at path: {}", faces[i]);
                return;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        {
            sky_box_shader_->Use();
            vao_.Bind();
            vbo_.Bind();
            float skyboxVertices[] = {// positions
                                      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                                      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                                      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                                      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                                      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                                      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                                      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                                      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                                      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                                      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                                      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                                      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        }
    }
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
    glm::mat4 view = camera_->GetViewMatrix();

    {
        model_shader_->Use();
        {
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
    {
        glDepthFunc(GL_LEQUAL);
        sky_box_shader_->Use();
        // ... 设置观察和投影矩阵
        glm::mat4 vview = glm::mat4(glm::mat3(camera_->GetViewMatrix()));
        sky_box_shader_->SetMat4("view", vview);
        sky_box_shader_->SetMat4("projection", projection);

        vao_.Bind();
        glActiveTexture(GL_TEXTURE0);
        sky_box_.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        vao_.Unbind();
        glDepthFunc(GL_LESS); // set depth function back to default
    }
}

void Window::RenderClear() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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