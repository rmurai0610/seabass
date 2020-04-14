#pragma once
#include <seabass/camera.h>
#include <seabass/vertex_array.h>
#include <seabass/vertex_buffer.h>
#include <seabass/widgets/base.h>

namespace sb {
namespace Widget {
class Grid : public sb::Widget::Base {
private:
    float height_;
    sb::AxisDirection up_axis_;
    size_t n_partition_;
    float scale_;
    float line_width_;

    std::vector<float> fix_coordinate_frame(float x, float y, float z) {
        std::vector<float> vec({x, y, z});
        if (up_axis_ == AxisDirection::AxisX) {
            float tmp = vec[1];
            vec[1] = vec[0];
            vec[0] = tmp;
            return vec;
        }
        if (up_axis_ == AxisDirection::AxisNegX) {
            float tmp = -vec[1];
            vec[1] = vec[0];
            vec[0] = tmp;
            return vec;
        }
        if (up_axis_ == AxisDirection::AxisZ) {
            float tmp = vec[1];
            vec[1] = vec[2];
            vec[2] = tmp;
            return vec;
        }
        if (up_axis_ == AxisDirection::AxisNegZ) {
            float tmp = -vec[1];
            vec[1] = vec[2];
            vec[2] = tmp;
            return vec;
        }
        if (up_axis_ == AxisDirection::AxisNegY) {
            vec[1] *= -1;
            return vec;
        }

        return vec;
    }

public:
    Grid(float height, sb::AxisDirection up_axis = sb::AxisDirection::AxisY,
         size_t n_partition = 10, float scale = 1.f, float line_width = 1.f)
        : sb::Widget::Base(),
          height_(height),
          up_axis_(up_axis),
          n_partition_(n_partition),
          scale_(scale),
          line_width_(line_width) {}

    const float &y_pos() const { return height_; }
    float &y_pos() { return height_; }
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
            auto p1 = fix_coordinate_frame(-mid_point, height_,
                                           i * scale_ - mid_point);
            auto p2 = fix_coordinate_frame(mid_point, height_,
                                           i * scale_ - mid_point);
            vertices.insert(vertices.end(), p1.begin(), p1.end());
            vertices.insert(vertices.end(), p2.begin(), p2.end());
        }

        for (size_t i = 0; i <= n_partition_; ++i) {
            auto p1 = fix_coordinate_frame(i * scale_ - mid_point, height_,
                                           -mid_point);
            auto p2 = fix_coordinate_frame(i * scale_ - mid_point, height_,
                                           mid_point);
            vertices.insert(vertices.end(), p1.begin(), p1.end());
            vertices.insert(vertices.end(), p2.begin(), p2.end());
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
