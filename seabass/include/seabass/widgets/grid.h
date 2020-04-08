#pragma once
#include <seabass/vertex_array.h>
#include <seabass/vertex_buffer.h>
#include <seabass/widgets/base.h>

namespace sb {
namespace Widget {
class Grid : public sb::Widget::Base {
private:
    float y_pos_;
    size_t n_partition_;
    float scale_;
    float line_width_;

public:
    Grid(float y_pos, size_t n_partition = 10, float scale = 1.f,
         float line_width = 1.f)
        : y_pos_(y_pos),
          n_partition_(n_partition),
          scale_(scale),
          line_width_(line_width),
          sb::Widget::Base() {}

    const float &y_pos() const { return y_pos_; }
    float &y_pos() { return y_pos_; }
    const size_t &n_partition() const { return n_partition_; }
    size_t &n_partition() { return n_partition_; }
    const float &scale() const { return scale_; }
    float &scale() { return scale_; }
    const float &line_width() const { return line_width_; }
    float &line_width() { return line_width_; }

    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        std::vector<float> vertices, colors;
        float mid_point = scale_ * float(n_partition_) / 2;
        for (size_t i = 0; i <= n_partition_; ++i) {
            vertices.push_back(-mid_point);
            vertices.push_back(y_pos_);
            vertices.push_back(i * scale_ - mid_point);
            vertices.push_back(mid_point);
            vertices.push_back(y_pos_);
            vertices.push_back(i * scale_ - mid_point);
        }
        for (size_t i = 0; i <= n_partition_; ++i) {
            vertices.push_back(i * scale_ - mid_point);
            vertices.push_back(y_pos_);
            vertices.push_back(-mid_point);
            vertices.push_back(i * scale_ - mid_point);
            vertices.push_back(y_pos_);
            vertices.push_back(mid_point);
        }

        for (size_t i = 0; i < vertices.size() / 3; ++i) {
            colors.push_back(colorscheme->grid()[0]);
            colors.push_back(colorscheme->grid()[1]);
            colors.push_back(colorscheme->grid()[2]);
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
