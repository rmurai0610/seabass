#pragma once
#include <seabass/vertex_array.h>
#include <seabass/vertex_buffer.h>
#include <seabass/widgets/base.h>

#include <Eigen/Dense>
#include <sophus/se3.hpp>
namespace sb {
namespace Widget {
class Camera : public sb::Widget::Base {
private:
    Sophus::SE3f T_wc_;
    bool y_up_;
    float scale_, line_width_;

public:
    Camera(Sophus::SE3f T_wc = Sophus::SE3f(), bool y_up = true,
           float scale = 1.f, float line_width = 1.f)
        : scale_(scale),
          line_width_(line_width),
          y_up_(y_up),
          T_wc_(fix_coordinate_frame(T_wc, y_up)),
          sb::Widget::Base() {}

    const float &scale() const { return scale_; }
    float &scale() { return scale_; }
    const float &line_width() const { return line_width_; }
    float &line_width() { return line_width_; }

    static Sophus::SE3f fix_coordinate_frame(Sophus::SE3f T_wc, bool y_up) {
        if (!y_up) {
            Sophus::SE3f w = Sophus::SE3f(
                (Eigen::Matrix<float, 4, 4>() << 1.f, 0.f, 0.f, 0.f, 0.f, -1.f,
                 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
                    .finished());
            T_wc = w * T_wc;
        }
        return T_wc;
    }

    static std::vector<float> camera_vertices(Sophus::SE3f T_wc,
                                              float scale = 1.f) {
        std::vector<Eigen::Vector3f> camera_vertices({{0.f, 0.f, 0.f},
                                                      {-1.f, -1.f, 1.f},
                                                      {-1.f, -1.f, 1.f},
                                                      {1.f, -1.f, 1.f},
                                                      {1.f, -1.f, 1.f},
                                                      {0.f, 0.f, 0.f},
                                                      {0.f, 0.f, 0.f},
                                                      {-1.f, 1.f, 1.f},
                                                      {-1.f, 1.f, 1.f},
                                                      {1.f, 1.f, 1.f},
                                                      {1.f, 1.f, 1.f},
                                                      {0.f, 0.f, 0.f},
                                                      {1.f, 1.f, 1.f},
                                                      {1.f, -1.f, 1.f},
                                                      {-1.f, 1.f, 1.f},
                                                      {-1.f, -1.f, 1.f}});
        std::vector<float> vertices;
        for (auto &v : camera_vertices) {
            auto v_w = T_wc * (scale * v);
            vertices.push_back(v_w.x());
            vertices.push_back(v_w.y());
            vertices.push_back(v_w.z());
        }
        return vertices;
    }

    void set_T_wc(Sophus::SE3f T_wc) {
        T_wc_ = fix_coordinate_frame(T_wc, y_up_);
    }

    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        std::vector<float> vertices, colors;
        vertices = camera_vertices(T_wc_, scale_);
        for (size_t i = 0; i < vertices.size() / 3; ++i) {
            colors.push_back(colorscheme->camera()[0]);
            colors.push_back(colorscheme->camera()[1]);
            colors.push_back(colorscheme->camera()[2]);
        }

        auto vbo_v = sb::VertexBuffer(vertices, 3);
        auto vbo_c = sb::VertexBuffer(colors, 3);
        auto vao = sb::VertexArray(
            program, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
        glLineWidth(1);
        vao.render(GL_LINES);
    }
};
}  // namespace Widget
}  // namespace sb
