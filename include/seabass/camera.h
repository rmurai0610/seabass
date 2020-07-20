#pragma once
#include <seabass/opengl_loader.h>
#include <seabass/window.h>

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace sb {
enum class AxisDirection { AxisX, AxisY, AxisZ, AxisNegX, AxisNegY, AxisNegZ };

class Camera {
protected:
    sb::Window *window_;
    sb::AxisDirection right_vec_, up_vec_;

    static glm::vec3 to_vec3(AxisDirection axis) {
        switch (axis) {
            case AxisDirection::AxisX:
                return glm::vec3(1, 0, 0);
            case AxisDirection::AxisNegX:
                return glm::vec3(-1, 0, 0);
            case AxisDirection::AxisY:
                return glm::vec3(0, 1, 0);
            case AxisDirection::AxisNegY:
                return glm::vec3(0, -1, 0);
            case AxisDirection::AxisZ:
                return glm::vec3(0, 0, 1);
            case AxisDirection::AxisNegZ:
                return glm::vec3(0, 0, -1);
        }
        std::cerr << "to_vec3: Invalid axis\n";
        std::exit(EXIT_FAILURE);
    }

    static glm::mat4 coordinate_system(const glm::vec3 &right, const glm::vec3&up) {
        glm::vec3 forward;
        forward = glm::normalize(glm::cross(right, up));
        glm::mat4 coordinate_system;
        coordinate_system[0] = glm::vec4(right, 0.f);
        coordinate_system[1] = glm::vec4(up, 0.f);
        coordinate_system[2] = glm::vec4(forward, 0.f);
        coordinate_system[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);
        return glm::transpose(coordinate_system);
    }

public:
    Camera(sb::Window *window, AxisDirection right_vec, AxisDirection up_vec)
        : window_(window), right_vec_(right_vec), up_vec_(up_vec) {}
    virtual ~Camera() {}
    virtual void activate() = 0;
    virtual glm::mat4 MVP() const = 0;

    sb::AxisDirection up_vec() const { return up_vec_; }
    sb::AxisDirection right_vec() const { return right_vec_; }
};

class ArcBallCamera : public Camera {
private:
    float FoV_, min_disp_range_, max_disp_range_;
    glm::vec3 right_, up_;
    glm::mat4 coordinate_system_;
    double yaw_, pitch_;
    double yaw_step_, pitch_step_, dist_step_;
    double dist_;
    glm::vec3 at_;

    void restrict() {
        double delta = 0.001;
        if (pitch_ < delta) {
            pitch_ = delta;
        }
        if (pitch_ > M_PI - delta) {
            pitch_ = M_PI - delta;
        }
        if (dist_ < delta) {
            dist_ = delta;
        }
    }

public:
    ArcBallCamera(sb::Window *window, float FoV, float min_disp_range,
                  float max_disp_range, sb::AxisDirection right_vec,
                  sb::AxisDirection up_vec)
        : Camera(window, right_vec, up_vec),
          FoV_(FoV),
          min_disp_range_(min_disp_range),
          max_disp_range_(max_disp_range),
          right_(Camera::to_vec3(right_vec)),
          up_(Camera::to_vec3(up_vec)),
          coordinate_system_(Camera::coordinate_system(right_, up_)),
          yaw_(M_PI_2),
          pitch_(M_PI_2),
          yaw_step_(0.005),
          pitch_step_(0.005),
          dist_step_(0.1),
          dist_(5.f),
          at_(0, 0, 0) {}

    void activate() override {
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        auto mouse_delta = ImGui::GetIO().MouseDelta;
        if (ImGui::GetIO().MouseDown[0]) {
            // Left mouse is pressed down
            yaw_ += mouse_delta.x * yaw_step_;
            pitch_ -= mouse_delta.y * pitch_step_;
        }
        if (ImGui::GetIO().MouseDown[1]) {
            // Right mouse is pressed down
            glm::vec3 up(0, 1, 0);
            glm::vec3 dir = glm::normalize(at_ - eye());
            glm::vec3 tangent = glm::normalize(glm::cross(dir, up));
            glm::vec3 bi_tangent = glm::cross(dir, tangent);
            at_ -= (mouse_delta.x / 500.f) * tangent +
                   (mouse_delta.y / 500.f) * bi_tangent;
        }
        // Handle Scrolling
        dist_ += ImGui::GetIO().MouseWheel * dist_step_;

        // Restrict the position of the camera
        restrict();
    }

    glm::vec3 eye() const {
        double px = at_.x + dist_ * cos(yaw_) * sin(pitch_);
        double py = at_.y + dist_ * cos(pitch_);
        double pz = at_.z + dist_ * sin(yaw_) * sin(pitch_);
        return glm::vec3(px, py, pz);
    }

    glm::mat4 MVP() const override {
        glm::mat4 P = glm::perspective(glm::radians(FoV_), window_->ratio(),
                                       min_disp_range_, max_disp_range_);
        glm::vec3 up(0, 1, 0);
        glm::mat4 V = glm::lookAtRH(eye(), at_, up);
        glm::mat4 M = coordinate_system_;
        return P * V * M;
    }
};
class Camera2d : public Camera {
private:
    glm::mat4 projection_;
public:
    Camera2d(sb::Window *window) : Camera(window, AxisDirection::AxisX, AxisDirection::AxisNegY) {
      projection_ = glm::ortho(0, window_->width(), window_->height(), 0);
    }
    ~Camera2d() {}
    void activate() {}
    glm::mat4 MVP() const {
        return projection_;
    }
};
};  // namespace sb
