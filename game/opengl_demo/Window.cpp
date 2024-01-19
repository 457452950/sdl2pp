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

    //    this->light_shader_ =
    //    std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightShader.vert)",
    //                                                   R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightShader.frag)");
    //    this->light_shader_ =
    //            std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\DirectionalLightShader.vert)",
    //                                     R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\DirectionalLightShader.frag)");
    //    this->light_shader_ =
    //            std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\PointLightShader.vert)",
    //                                     R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\PointLightShader.frag)");
    this->light_shader_ =
            std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\SpotLightShader.vert)",
                                     R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\SpotLightShader.frag)");
    this->light_source_shader_ =
            std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightSourceShader.vert)",
                                     R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\LightSourceShader.frag)");

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 1.5f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // clang-format off
    float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    // clang-format on


    {
        cube_vao.Bind();
        cube_vbo.Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
    {
        light_vao.Bind();
        cube_vbo.Bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
    {
        // texture 1
        // ---------
        glActiveTexture(GL_TEXTURE0);
        diffuse_tex_.Bind();
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto img = sdlpp::IMG_LoadSurfaceFromFile("H:/Resources/image/container2.png");
        Assert(img, "load surface from file {} err {}", "H:/Resources/image/container2.jpg", SDL_GetError());
        auto f = img->GetFormat();
        LOG_INF(log::APP, "{} {} {} {}", f->BytesPerPixel, f->BitsPerPixel, f->format, f->Amask);
        if(f->BytesPerPixel == 3) {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         img->GetWidth(),
                         img->GetHeight(),
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         img->GetPixels());
        } else if(f->BytesPerPixel == 4) {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         img->GetWidth(),
                         img->GetHeight(),
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         img->GetPixels());
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    {
        glActiveTexture(GL_TEXTURE1);
        specular_tex_.Bind();
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto img = sdlpp::IMG_LoadSurfaceFromFile("H:/Resources/image/container2_specular.png");
        Assert(img, "load surface from file {} err {}", "H:/Resources/image/container2_specular.jpg", SDL_GetError());
        auto f = img->GetFormat();
        LOG_INF(log::APP, "{} {} {} {}", f->BytesPerPixel, f->BitsPerPixel, f->format, f->Amask);
        if(f->BytesPerPixel == 3) {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         img->GetWidth(),
                         img->GetHeight(),
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         img->GetPixels());
        } else if(f->BytesPerPixel == 4) {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         img->GetWidth(),
                         img->GetHeight(),
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         img->GetPixels());
        }
        glGenerateMipmap(GL_TEXTURE_2D);
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

    static glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),
                                        glm::vec3(2.0f, 5.0f, -15.0f),
                                        glm::vec3(-1.5f, -2.2f, -2.5f),
                                        glm::vec3(-3.8f, -2.0f, -12.3f),
                                        glm::vec3(2.4f, -0.4f, -3.5f),
                                        glm::vec3(-1.7f, 3.0f, -7.5f),
                                        glm::vec3(1.3f, -2.0f, -2.5f),
                                        glm::vec3(1.5f, 2.0f, -2.5f),
                                        glm::vec3(1.5f, 0.2f, -1.5f),
                                        glm::vec3(-1.3f, 1.0f, -1.5f)};

    glm::mat4 projection = glm::perspective(
            glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 100.0f);

    light_shader_->use();
    {
        // set mvp

        // camera/view transformation
        glm::mat4 view = camera_->GetViewMatrix();
        light_shader_->setMat4("view", view);

        light_shader_->setMat4("projection", projection);
    }
    {
        light_shader_->setInt("material.diffuse", 0);
        light_shader_->setInt("material.specular", 1);
        light_shader_->setFloat("material.shininess", 64.0f);

        light_shader_->setVec3("light.ambient", glm::vec3{ambientLight});
        light_shader_->setVec3("light.diffuse", glm::vec3{diffuseLight});
        light_shader_->setVec3("light.specular", glm::vec3{specularLight});
        //        light_shader_->setVec3("light.position", lightPos);

        light_shader_->setVec3("light.position", camera_->Position);
        light_shader_->setVec3("light.direction", camera_->Front);
        light_shader_->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        light_shader_->setFloat("light.outerCutOff", glm::cos(glm::radians(15.0f)));

        light_shader_->setVec3("viewPos", camera_->Position);

        light_shader_->setBool("ambient_enable_", ambient_enable_);
        light_shader_->setBool("diffuse_enable_", diffuse_enable_);
        light_shader_->setBool("specular_enable_", specular_enable_);
    }
    for(unsigned int i = 0; i < 10; i++) {
        glm::mat4 model{1.0f};
        model       = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model       = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        light_shader_->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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
    ImGui::SliderFloat("specularLight", &specularLight, 0.0f, 5.0f);
    ImGui::SliderFloat("r", &r, 1.0f, 5.0f);
    ImGui::SliderFloat("speed", &speed, 0.0f, 10.0f);
    ImGui::Checkbox("ambient", &ambient_enable_);
    ImGui::Checkbox("diffuse", &diffuse_enable_);
    ImGui::Checkbox("specular", &specular_enable_);
}

} // namespace game