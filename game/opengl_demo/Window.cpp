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

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    model_shader_->Use();
    model_ = std::make_shared<gl::Model>("H:/Code/CLion/sdl2pp/game/opengl_demo/nanosuit/nanosuit.obj");
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

    model_shader_->Use();
    {
        glm::mat4 view = camera_->GetViewMatrix();
        model_shader_->SetMat4("view", view);

        model_shader_->SetMat4("projection", projection);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        //        model           = glm::translate(model,
        //                               glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the
        //                               scene
        //        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so scale
        //        it down
        model_shader_->SetMat4("model", model);
    }
    { model_->Draw(model_shader_); }
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
}

void Window::eventHandle(const SDL_Event &event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    SWindow::eventHandle(event);
}

void Window::IMGUIProcess() {}

} // namespace game