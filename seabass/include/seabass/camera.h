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
    float FoV_, min_disp_range_, max_disp_range_;
    bool left_drag_, right_drag_;
    static inline double scroll_x_, scroll_y_;
    sb::AxisDirection right_vec_, up_vec_;
    glm::mat4 coordinate_system_;

    virtual void left_release(double x, double y) {}
    virtual void right_release(double x, double y) {}
    virtual void left_drag(double x, double y) {}
    virtual void right_drag(double x, double y) {}
    virtual void left_press(double x, double y) {}
    virtual void right_press(double x, double y) {}
    virtual void scroll(double x, double y){};

private:
    glm::vec3 to_vec3(AxisDirection axis) const {
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
    }
    void set_up_coordinate_frame() {
        glm::vec3 right, up, forward;
        right = to_vec3(right_vec_);
        up = to_vec3(up_vec_);
        forward = glm::normalize(glm::cross(right, up));
        coordinate_system_[0] = glm::vec4(right, 0.f);
        coordinate_system_[1] = glm::vec4(up, 0.f);
        coordinate_system_[2] = glm::vec4(forward, 0.f);
        coordinate_system_[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);
        coordinate_system_ = glm::transpose(coordinate_system_);
    }

public:
    Camera(sb::Window *window, float FoV, float min_disp_range,
           float max_disp_range, sb::AxisDirection right_vec,
           sb::AxisDirection up_vec)
        : window_(window),
          FoV_(FoV),
          min_disp_range_(min_disp_range),
          max_disp_range_(max_disp_range),
          right_vec_(right_vec),
          up_vec_(up_vec),
          left_drag_(false),
          right_drag_(false) {
        set_up_coordinate_frame();
        glfwSetScrollCallback(window_->window(), scroll_callback);
    }

    virtual ~Camera() {}

    static void scroll_callback(GLFWwindow *window, double x, double y) {
        Camera::scroll_x_ = x;
        Camera::scroll_y_ = y;
    }

    void activate() {
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        double xpos, ypos;
        glfwGetCursorPos(window_->window(), &xpos, &ypos);
        int left_state =
            glfwGetMouseButton(window_->window(), GLFW_MOUSE_BUTTON_LEFT);
        int right_state =
            glfwGetMouseButton(window_->window(), GLFW_MOUSE_BUTTON_RIGHT);
        if (left_state == GLFW_RELEASE) {
            left_release(xpos, ypos);
            left_drag_ = false;
        }
        if (right_state == GLFW_RELEASE) {
            right_release(xpos, ypos);
            right_drag_ = false;
        }
        if (left_state == GLFW_PRESS) {
            if (left_drag_) {
                left_drag(xpos, ypos);
            } else {
                left_press(xpos, ypos);
                left_drag_ = true;
            }
        }
        if (right_state == GLFW_PRESS) {
            if (right_drag_) {
                right_drag(xpos, ypos);
            } else {
                right_press(xpos, ypos);
                right_drag_ = true;
            }
        }
        scroll(Camera::scroll_x_, Camera::scroll_y_);
        Camera::scroll_x_ = 0;
        Camera::scroll_y_ = 0;
    }

    virtual glm::mat4 MVP() const = 0;
    sb::AxisDirection up_vec() const { return up_vec_; }
    sb::AxisDirection right_vec() const { return right_vec_; }
};

class ArcBallCamera : public Camera {
private:
    double last_mx_, last_my_, cur_mx_, cur_my_;
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
        : Camera(window, FoV, min_disp_range, max_disp_range, right_vec,
                 up_vec),
          last_mx_(0),
          last_my_(0),
          cur_mx_(0),
          cur_my_(0),
          yaw_(M_PI_2),
          pitch_(M_PI_2),
          yaw_step_(0.005),
          pitch_step_(0.005),
          dist_step_(0.1),
          dist_(5.f),
          at_(0, 0, 0) {}

    void left_press(double x, double y) override {
        last_mx_ = x;
        last_my_ = y;
        cur_mx_ = x;
        cur_my_ = y;
    }

    void right_press(double x, double y) override {
        last_mx_ = x;
        last_my_ = y;
        cur_mx_ = x;
        cur_my_ = y;
    }

    void left_drag(double x, double y) override {
        yaw_ += (x - last_mx_) * yaw_step_;
        pitch_ -= (y - last_my_) * pitch_step_;
        restrict();
        cur_mx_ = x;
        cur_my_ = y;
        last_mx_ = x;
        last_my_ = y;
    }

    void right_drag(double x, double y) override {
        glm::vec3 up(0, 1, 0);
        glm::vec3 dir = glm::normalize(at_ - eye());
        glm::vec3 tangent = glm::normalize(glm::cross(dir, up));
        glm::vec3 bi_tangent = glm::cross(dir, tangent);
        at_ -= float((x - last_mx_) / 500.f) * tangent +
               float((y - last_my_) / 500.f) * bi_tangent;
        cur_mx_ = x;
        cur_my_ = y;
        last_mx_ = x;
        last_my_ = y;
    }

    void scroll(double x, double y) override {
        dist_ += y * dist_step_;
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
};  // namespace sb
