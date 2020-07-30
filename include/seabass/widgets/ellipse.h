#pragma once
#include <seabass/widgets/base.h>
namespace sb {
namespace Widget {

class Ellipse : public sb::Widget::Base {
private:
    float cx_, cy_;
    float radius_x_, radius_y_;
    float theta_;
    bool filled_;

    void create_filled_ellipse(std::vector<float> &vertices, std::vector<float> &colors) {
        vertices.clear();
        colors.clear();
        int n = 100;
        for (int i = 1; i <= n; ++i) {
            float alpha1 = 2 * (i - 1) * M_PI / n;
            float alpha2 = 2 * i * M_PI / n;
            float dx1 = radius_x_ * cos(alpha1) * cos(theta_) - radius_y_ * sin(alpha1) * sin(theta_);
            float dy1 = -radius_x_ * cos(alpha1) * sin(theta_) + radius_y_ * sin(alpha1) * cos(theta_);
            float dx2 = radius_x_ * cos(alpha2) * cos(theta_) - radius_y_ * sin(alpha2) * sin(theta_);
            float dy2 = -radius_x_ * cos(alpha2) * sin(theta_) + radius_y_ * sin(alpha2) * cos(theta_);
            vertices.insert(vertices.end(), {cx_, cy_, 0});
            vertices.insert(vertices.end(), {cx_ + dx1, cy_ + dy1, 0});
            vertices.insert(vertices.end(), {cx_ + dx2, cy_ + dy2, 0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
        }
    }
    void create_outline_ellipse(std::vector<float> &vertices, std::vector<float> &colors) {
        vertices.clear();
        colors.clear();
        int n = 100;
        for (int i = 1; i <= n; ++i) {
            float alpha1 = 2 * (i - 1) * M_PI / n;
            float alpha2 = 2 * i * M_PI / n;
            float x1 = radius_x_ * cos(alpha1) * cos(theta_) - radius_y_ * sin(alpha1) * sin(theta_) + cx_;
            float y1 = -radius_x_ * cos(alpha1) * sin(theta_) - radius_y_ * sin(alpha1) * cos(theta_) + cy_;
            float x2 = radius_x_ * cos(alpha2) * cos(theta_) - radius_y_ * sin(alpha2) * sin(theta_) + cx_;
            float y2 = -radius_x_ * cos(alpha2) * sin(theta_) - radius_y_ * sin(alpha2) * cos(theta_) + cy_;
            vertices.insert(vertices.end(), {x1, y1, 0});
            vertices.insert(vertices.end(), {x2, y2, 0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
            colors.insert(colors.end(), {1.0, 0.0, 0.0});
        }
    }
public:
    Ellipse(float cx, float cy, float radius_x, float radius_y, float theta, bool filled=false) : sb::Widget::Base(),
        cx_(cx), cy_(cy), radius_x_(radius_x), radius_y_(radius_y), theta_(theta), filled_(filled) {}
    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        std::vector<float> vertices, colors;
        if (filled_) {
            create_filled_ellipse(vertices, colors);
        } else {
            create_outline_ellipse(vertices, colors);
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
