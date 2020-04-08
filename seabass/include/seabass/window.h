#pragma once
#include <seabass/opengl_loader.h>
#include <seabass/themes.h>

#include <iostream>
#include <string>
#include <vector>
namespace sb {

class Window {
private:
    std::string name_;
    uint32_t major_, minor_;
    int width_, height_;
    GLFWwindow *window_;
    ColorScheme colorscheme_;
    void build() {
        // glfwWindowHint(GLFW_SAMPLES, 4);
        // glEnable(GL_MULTISAMPLE);
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
#endif
        window_ = glfwCreateWindow(width_, height_, name_.c_str(), NULL, NULL);
        glfwMakeContextCurrent(window_);
        glfwSwapInterval(1);  // Enable vsync
        if (!glfwInit()) {
            std::exit(EXIT_FAILURE);
        }
        // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
        bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
        bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
        bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING)
        bool err = false;
        glbinding::initialize([](const char *name) {
            return (glbinding::ProcAddress)glfwGetProcAddress(name);
        });
#else
        bool err =
            false;  // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader
                    // is likely to requires some form of initialization.
#endif
        if (err) {
            std::exit(EXIT_FAILURE);
        }
    }

public:
    Window(std::string name, std::pair<int, int> version,
           std::pair<int, int> size, sb::ColorScheme colorscheme)
        : name_(name),
          major_(version.first),
          minor_(version.second),
          width_(size.first),
          height_(size.second),
          colorscheme_(colorscheme) {
        build();
    }
    ~Window() {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }
    bool should_close() const { return glfwWindowShouldClose(window_); }
    void swap_buffers() const { glfwSwapBuffers(window_); }
    void poll_events() const { glfwPollEvents(); }
    float ratio() const { return float(width_) / float(height_); }
    int width() const { return width_; }
    int height() const { return height_; }
    const GLFWwindow *window() const { return window_; }
    GLFWwindow *window() { return window_; }
    void activate() {
        glfwGetFramebufferSize(window_, &width_, &height_);
        glViewport(0, 0, width_, height_);
        glEnable(GL_DEPTH_TEST);
        std::array<float, 4> bg = colorscheme_.bg();
        glClearColor(bg[0], bg[1], bg[2], bg[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};
};  // namespace sb
