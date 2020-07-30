#pragma once

#if __has_include(<Eigen/Dense>)
#if __has_include(<sophus/se3.hpp>)
#include <seabass/widgets/base.h>
#include <seabass/widgets/camera.h>

#include <Eigen/Dense>
#include <sophus/se3.hpp>

namespace sb {
namespace Widget {
class Keyframes : public sb::Widget::Base {
private:
    float scale_;
    float line_width_;
    std::vector<Sophus::SE3f> poses_;

public:
    Keyframes(float scale = 1.f, float line_width = 1.f)
        : sb::Widget::Base(), scale_(scale), line_width_(line_width) {}
    const float &line_width() const { return line_width_; }
    float &line_width() { return line_width_; }
    void add_keyframe(const Sophus::SE3f &T_wc) { poses_.push_back(T_wc); }
    void set_keyframes(std::vector<Sophus::SE3f> poses) { poses_ = poses; }

    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        std::vector<float> vertices, colors;
        for (auto &T_wc : poses_) {
            auto vs = sb::Widget::Camera::camera_vertices(T_wc, scale_);
            vertices.insert(vertices.end(), vs.begin(), vs.end());
        }
        for (size_t i = 0; i < vertices.size() / 3; ++i) {
            colors.push_back(colorscheme->keyframes()[0]);
            colors.push_back(colorscheme->keyframes()[1]);
            colors.push_back(colorscheme->keyframes()[2]);
        }

        auto vbo_v = sb::VertexBuffer(vertices, 3);
        auto vbo_c = sb::VertexBuffer(colors, 3);
        auto vao = sb::VertexArray(
            program, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
        glLineWidth(line_width_);
        vao.render(GL_LINES);
    }
};
}  // namespace Widget
}  // namespace sb
#endif
#endif
