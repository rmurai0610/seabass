#pragma once

#pragma once
#include <seabass/vertex_array.h>
#include <seabass/vertex_buffer.h>
#include <seabass/widgets/base.h>

namespace sb {
namespace Widget {
class Pointcloud : public sb::Widget::Base {
private:
    std::vector<float> vertices_, colors_;
    float point_size_;

public:
    Pointcloud(const std::vector<float> &vertices,
               const std::vector<float> &colors, float point_size = 1.f)
        : sb::Widget::Base(), vertices_(vertices), colors_(colors), point_size_(point_size) {}

    const float &point_size() const { return point_size_; }
    float &point_size() { return point_size_; }
    void set_pointcloud(const std::vector<float> &vertices,
                        const std::vector<float> &colors) {
        vertices_ = vertices;
        colors_ = colors;
    }

    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        auto vbo_v = sb::VertexBuffer(vertices_, 3);
        auto vbo_c = sb::VertexBuffer(colors_, 3);
        auto vao = sb::VertexArray(
            program, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
        glPointSize(point_size_);
        vao.render(GL_POINTS);
    }
};
}  // namespace Widget
}  // namespace sb
