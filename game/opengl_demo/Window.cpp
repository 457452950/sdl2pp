#include "Window.h"

#include "sdl2pp/SDLpp.h"

namespace game {

Window::Window() {
    this->SetSize({1200, 900});

    SDL_SetRelativeMouseMode(SDL_TRUE);

    // fixme: context SDL_GL_DeleteContext
    sdlpp::gl::CreateContext(this->Get());

    sdlpp::gl::ext::Init();
    sdlpp::gl::ext::DepthTest();

    this->shader_ = std::make_shared<Shader>(R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\shader.vert)",
                                             R"(H:\Code\CLion\sdl2pp\game\opengl_demo\shader\shader.frag)");

    camera_ = std::make_shared<FPSCamera>(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // clang-format off
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 2, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };
    // clang-format on

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto img = sdlpp::IMG_LoadSurfaceFromFile("H:/Resources/image/container.jpg");
    Assert(img, "load surface from file {} err {}", "H:/Resources/image/container.jpg", SDL_GetError());
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
    shader_->setInt("texture1", 0);

    // texture 2
    // ---------
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    img = sdlpp::IMG_LoadSurfaceFromFile("H:/Resources/image/awesomeface.png");
    Assert(img, "load surface from file {} err {}", "H:/Resources/image/awesomeface.png", SDL_GetError());
    f = img->GetFormat();
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
    img.reset();

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader_->use();
    shader_->setInt("texture2", 1);
}

Window::~Window() {}

void Window::RenderProcess() {
    shader_->use();

    glm::mat4 projection = glm::perspective(
            glm::radians(camera_->GetFOV()), (float)this->GetWidth() / (float)this->GetHeight(), 0.1f, 100.0f);
    shader_->setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera_->GetViewMatrix();
    shader_->setMat4("view", view);

    shader_->setMat4("model", glm::mat4(1.0f));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Window::RenderClear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::RenderFlush() { sdlpp::GlWindow::RenderFlush(); }

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