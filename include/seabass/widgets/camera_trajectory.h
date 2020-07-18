#pragma once

#if __has_include(<Eigen/Dense>)
#if __has_include(<sophus/se3.hpp>)
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
    float line_width_;
    std::vector<Sophus::SE3f> poses_;

public:
    CameraTrajectory(float line_width = 1.f)
        : sb::Widget::Base(), line_width_(line_width) {}

    const float &line_width() const { return line_width_; }
    float &line_width() { return line_width_; }
    void add_pose(const Sophus::SE3f &T_wc) { poses_.push_back(T_wc); }
    void set_poses(std::vector<Sophus::SE3f> poses) { poses_ = poses; }

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
#endif
#endif
