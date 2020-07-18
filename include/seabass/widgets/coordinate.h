#pragma once
#include <seabass/vertex_array.h>
#include <seabass/vertex_buffer.h>
#include <seabass/widgets/base.h>
namespace sb {
namespace Widget {
class Coordinate : public sb::Widget::Base {
private:
    float line_width_;

public:
    Coordinate(float line_width = 1.f)
        : sb::Widget::Base(), line_width_(line_width) {}

    const float &line_width() const { return line_width_; }
    float &line_width() { return line_width_; }

    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        const std::vector<float> vertices({0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
                                           0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
                                           0.f, 0.f, 0.f, 1.f});
        const std::vector<float> colors({1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f,
                                         0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f,
                                         0.f, 1.f});
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
