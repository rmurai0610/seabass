#pragma once
#include <seabass/camera.h>
#include <seabass/opengl_loader.h>
#include <seabass/recorder.h>
#include <seabass/vertex_array.h>
#include <seabass/vertex_buffer.h>
#include <seabass/window.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace sb {
class Context {
private:
    std::unique_ptr<sb::Window> window_;
    std::unique_ptr<sb::Camera> camera_;
    std::unique_ptr<sb::Recorder> recorder_;
    ColorScheme colorscheme_;

    void imgui_enable(const char *glsl_version) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // Setup Dear ImGui style
        if (colorscheme_.theme() == Theme::DARK) {
            ImGui::StyleColorsDark();
        }
        if (colorscheme_.theme() == Theme::LIGHT) {
            ImGui::StyleColorsLight();
        }
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window_->window(), true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

public:
    Context() : colorscheme_(sb::ColorSchemeLight()) {}
    Context(Theme theme) : colorscheme_(sb::colorscheme(theme)) {}
    Context(ColorScheme colorscheme) : colorscheme_(colorscheme) {}
    ~Context() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    sb::Window *create_window(std::string name, std::pair<int, int> size) {
#if __APPLE__
        // GL 3.2 + GLSL 150
        const char *glsl_version = "#version 150";
        int major = 3;
        int minor = 2;
#else
        // GL 3.0 + GLSL 130
        const char *glsl_version = "#version 130";
        int major = 3;
        int minor = 2;
#endif
        window_ = std::make_unique<sb::Window>(
            name, std::pair<int, int>({major, minor}), size, colorscheme_);
        imgui_enable(glsl_version);
        return window_.get();
    }

    sb::Camera *create_camera3d(
        float FoV, std::pair<float, float> range,
        sb::AxisDirection right_vec = sb::AxisDirection::AxisX,
        sb::AxisDirection up_vec = sb::AxisDirection::AxisY) {
        camera_ = std::make_unique<sb::ArcBallCamera>(
            window_.get(), FoV, range.first, range.second, right_vec, up_vec);
        return camera_.get();
    }

    sb::Recorder *create_recorder(std::string filename) {
        recorder_ = std::make_unique<sb::Recorder>(window_.get(), filename);
        return recorder_.get();
    }

    sb::ColorScheme colorscheme() const { return colorscheme_; }

    bool render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (recorder_) {
            recorder_->write();
        }
        window_->swap_buffers();
        window_->poll_events();
        if (camera_) {
            camera_->activate();
        }
        window_->activate();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        return !window_->should_close();
    }
};
};  // namespace sb
