#include <random>
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

    this->instance_shader_ = std::make_shared<gl::Shader>();
    if(!this->instance_shader_->SetShader(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\instance_shader.vs)",
                                          R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\instance_shader.fs)")) {
        LOG_ERR(log::APP, "load shader error ");
        return;
    }
    this->model_shader_ = std::make_shared<gl::Shader>();
    if(!this->model_shader_->SetShader(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\model_loading.vs)",
                                       R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\model_loading.fs)")) {
        LOG_ERR(log::APP, "load shader error ");
        return;
    }

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    {
        modelMatrices = new glm::mat4[amount];
        srand(std::random_device{}()); // 初始化随机种子
        float radius = 150.0;
        float offset = 2.5f;
        for(unsigned int i = 0; i < amount; i++) {
            glm::mat4 model(1);
            // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
            float     angle        = (float)i / (float)amount * 360.0f;
            float     displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float     x            = sin(angle) * radius + displacement;
            displacement           = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y                = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
            displacement           = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z                = cos(angle) * radius + displacement;
            model                  = glm::translate(model, glm::vec3(x, y, z));

            // 2. 缩放：在 0.05 和 0.25f 之间缩放
            float scale = (rand() % 20) / 100.0f + 0.05;
            //            LOG_DBG(log::APP, "scale {}\n", scale);
            model       = glm::scale(model, glm::vec3(scale));

            // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
            float rotAngle = (rand() % 360);
            //            LOG_DBG(log::APP, "rotAngle {}\n", rotAngle);
            model          = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. 添加到矩阵的数组中
            modelMatrices[i] = model;
        }
    }

    planet_ = std::make_shared<gl::Model>("H:/Code/CLion/sdl2pp/game/opengl_demo/planet/planet.obj");
    rock_   = std::make_shared<gl::Model>("H:/Code/CLion/sdl2pp/game/opengl_demo/rock/rock.obj");
    {
        // 顶点缓冲对象
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

        for(unsigned int i = 0; i < rock_->meshes.size(); i++) {
            auto VAO = rock_->meshes[i].vao_;
            VAO->Bind();
            // 顶点属性
            GLsizei vec4Size = sizeof(glm::vec4);
            {
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(vec4Size));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(2 * vec4Size));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(3 * vec4Size));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);
            }
            glBindVertexArray(0);
        }
    }
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
            glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 1000.0f);
    glm::mat4 view = camera_->GetViewMatrix();
    {
        model_shader_->Use();
        model_shader_->SetMat4("view", view);
        model_shader_->SetMat4("projection", projection);
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        model_shader_->SetMat4("model", model);

        planet_->Draw(model_shader_);
    }
    //    {
    //        for(unsigned int i = 0; i < amount; i++) {
    //            model_shader_->SetMat4("model", modelMatrices[i]);
    //            rock_->Draw(model_shader_);
    //        }
    //    }
    {
        instance_shader_->Use();
        {
            instance_shader_->SetMat4("view", view);
            instance_shader_->SetMat4("projection", projection);
            for(unsigned int i = 0; i < rock_->meshes.size(); i++) {
                rock_->meshes[i].vao_->Bind();
                glDrawElementsInstanced(GL_TRIANGLES, rock_->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
            }
        }
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
    const double_t move_speed = 0.01;

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