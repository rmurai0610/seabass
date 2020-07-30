#pragma once
namespace sb {
namespace Widget {

class Circle: public sb::Widget::Base {
public:
    Circle() : sb::Widget::Base() {}
    void render(const sb::Program *program,
                const sb::ColorScheme *colorscheme) override {
        std::vector<float> vertices({-1.0, -1.0, 1.0,  1.0,  -1.0, 1.0,
                                     1.0,  1.0,  1.0,  -1.0, 1.0,  1.0,
                                     -1.0, -1.0, -1.0, 1.0,  -1.0, -1.0,
                                     1.0,  1.0,  -1.0, -1.0, 1.0,  -1.0});
        std::vector<float> colors({1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                                   1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
                                   1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0});
        std::vector<uint32_t> indices({0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                                       7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
                                       4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3});

        auto vbo_v = sb::VertexBuffer(vertices, 3);
        auto vbo_c = sb::VertexBuffer(colors, 3);
        auto vao = sb::VertexArray(
            program, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
        vao.render(GL_TRIANGLES, indices);
    }
};
}  // namespace Widget
}  // namespace sb
