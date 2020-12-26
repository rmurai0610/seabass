#pragma once
#include <seabass/widgets/base.h>
namespace sb {
namespace Widget {

class Circle: public sb::Widget::Base {
private:
    float cx_, cy_;
    float radius_;
    bool filled_;

    void create_filled_circle(std::vector<float> &vertices, std::vector<float> &colors) {
        vertices.clear();
        colors.clear();
        int n_triangles = 100;
        for (int i = 1; i <= n_triangles; ++i) {
            float theta1 = 2 * (i - 1) * M_PI / n_triangles;
            float theta2 = 2 * i * M_PI / n_triangles;
            float dx1 = radius_ * sin(theta1);
            float dy1 = radius_ * cos(theta1);
            float dx2 = radius_ * sin(theta2);
            float dy2 = radius_ * cos(theta2);
            vertices.insert(vertices.end(), {cx_, cy_, 0});
            vertices.insert(vertices.end(), {cx_ + dx1, cy_ + dy1, 0});
            vertices.insert(vertices.end(), {cx_ + dx2, cy_ + dy2, 0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
        }
    }
    void create_outline_circle(std::vector<float> &vertices, std::vector<float> &colors) {
        vertices.clear();
        colors.clear();
        int n_lines= 100;
        for (int i = 1; i <= n_lines; ++i) {
            float theta1 = 2 * (i - 1) * M_PI / n_lines;
            float theta2 = 2 * i * M_PI / n_lines;
            float dx1 = radius_ * sin(theta1);
            float dy1 = radius_ * cos(theta1);
            float dx2 = radius_ * sin(theta2);
            float dy2 = radius_ * cos(theta2);
            vertices.insert(vertices.end(), {cx_ + dx1, cy_ + dy1, 0});
            vertices.insert(vertices.end(), {cx_ + dx2, cy_ + dy2, 0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
        }
    }
public:
    Circle(float cx, float cy, float radius, bool filled=false) : sb::Widget::Base(), cx_(cx), cy_(cy), radius_(radius), filled_(filled) {}
    const float &cx() const { return cx_; }
    const float &cy() const { return cy_; }
    const float &radius() const { return radius_; }
    float &cx() { return cx_; }
    float &cy() { return cy_; }
    float &radius() { return radius_; }

    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        std::vector<float> vertices, colors;
        if (filled_) {
            create_filled_circle(vertices, colors);
        } else {
            create_outline_circle(vertices, colors);
        }
        auto vbo_v = sb::VertexBuffer(vertices, 3);
        auto vbo_c = sb::VertexBuffer(colors, 3);
        auto vao = sb::VertexArray(program, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
        if (filled_) {
            vao.render(GL_TRIANGLES);
        } else {
            vao.render(GL_LINES);
        }
    };
};
}  // namespace Widget
}  // namespace sb
