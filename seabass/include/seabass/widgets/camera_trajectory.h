#pragma once
#include <seabass/vertex_array.h>
#include <seabass/vertex_buffer.h>
#include <seabass/widgets/base.h>
#include <seabass/widgets/camera.h>

#include <Eigen/Dense>
#include <sophus/se3.hpp>

namespace sb {
namespace Widget {
class CameraTrajectory : public sb::Widget::Base {
private:
    bool y_up_;
    float line_width_;
    std::vector<Sophus::SE3f> poses_;

public:
    CameraTrajectory(bool y_up = true, float line_width = 1.f)
        : y_up_(y_up), line_width_(line_width), sb::Widget::Base() {}
    const float &line_width() const { return line_width_; }
    float &line_width() { return line_width_; }
    void add_pose(Sophus::SE3f T_wc) {
        poses_.push_back(sb::Widget::Camera::fix_coordinate_frame(T_wc, y_up_));
    }

    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        std::vector<float> vertices, colors;
        for (auto &T_wc : poses_) {
            vertices.push_back(T_wc.translation().x());
            vertices.push_back(T_wc.translation().y());
            vertices.push_back(T_wc.translation().z());
        }
        for (size_t i = 0; i < vertices.size() / 3; ++i) {
            colors.push_back(colorscheme->trajectory()[0]);
            colors.push_back(colorscheme->trajectory()[1]);
            colors.push_back(colorscheme->trajectory()[2]);
        }

        auto vbo_v = sb::VertexBuffer(vertices, 3);
        auto vbo_c = sb::VertexBuffer(colors, 3);
        auto vao = sb::VertexArray(
            program, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
        glLineWidth(line_width_);
        vao.render(GL_LINE_STRIP);
    }
};
}  // namespace Widget
}  // namespace sb
