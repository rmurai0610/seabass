#include <seabass/seabass.h>


void circle(std::pair<float, float> center, float radius, std::vector<float> &vertices, std::vector<float> &colors) {
    auto [x, y] = center;
    int n_triangles = 100;
    for (int i = 1; i <= n_triangles; ++i) {
        float theta1 = 2 * (i - 1) * M_PI / n_triangles;
        float theta2 = 2 * i * M_PI / n_triangles;
        float dx1 = radius * sin(theta1);
        float dy1 = radius * cos(theta1);
        float dx2 = radius * sin(theta2);
        float dy2 = radius * cos(theta2);
        vertices.insert(vertices.end(), {x, y, 0});
        vertices.insert(vertices.end(), {x + dx1, y + dy1, 0});
        vertices.insert(vertices.end(), {x + dx2, y + dy2, 0});
        colors.insert(colors.end(), {1.0, 0.0, 0.0});
        colors.insert(colors.end(), {1.0, 0.0, 0.0});
        colors.insert(colors.end(), {1.0, 0.0, 0.0});
    }
}

void circle_outline(std::pair<float, float> center, float radius, std::vector<float> &vertices, std::vector<float> &colors) {
    auto [x, y] = center;
    int n_lines = 100;
    for (int i = 1; i <= n_lines ; ++i) {
        float theta1 = 2 * (i - 1) * M_PI / n_lines;
        float theta2 = 2 * i * M_PI / n_lines;
        float dx1 = radius * sin(theta1);
        float dy1 = radius * cos(theta1);
        float dx2 = radius * sin(theta2);
        float dy2 = radius * cos(theta2);
        vertices.insert(vertices.end(), {x + dx1, y + dy1, 0});
        vertices.insert(vertices.end(), {x + dx2, y + dy2, 0});
        colors.insert(colors.end(), {1.0, 0.0, 0.0});
        colors.insert(colors.end(), {1.0, 0.0, 0.0});
    }
}

void ellipse_outline(std::pair<float, float> center, std::pair<float, float> radius, float angle, std::vector<float> &vertices, std::vector<float> &colors) {
    auto [x, y] = center;
    auto [rx, ry] = radius;
    int n_lines = 100;
    for (int i = 1; i <= n_lines ; ++i) {
        float alpha1 = 2 * (i - 1) * M_PI / n_lines;
        float alpha2 = 2 * i * M_PI / n_lines;
        float x1 = rx * cos(alpha1) * cos(angle) - ry * sin(alpha1) * sin(angle) + x;
        float y1 = -rx * cos(alpha1) * sin(angle) + ry * sin(alpha1) * cos(angle) + y;
        float x2 = rx * cos(alpha2) * cos(angle) - ry * sin(alpha2) * sin(angle) + x;
        float y2 = -rx * cos(alpha2) * sin(angle) + ry * sin(alpha2) * cos(angle) + y;
        vertices.insert(vertices.end(), {x1, y1, 0});
        vertices.insert(vertices.end(), {x2, y2, 0});
        colors.insert(colors.end(), {1.0, 0.0, 0.0});
        colors.insert(colors.end(), {1.0, 0.0, 0.0});
    }

}

int main () {
    auto ctx = sb::Context(sb::Theme::LIGHT);
    auto window = ctx.create_window("Hello World 2d", {960, 640});
    auto camera = ctx.create_camera2d();
    auto program = sb::Program();
    //float triangle_size = 100.f;
    //std::vector<float> vertices = {{
        //480, 320 - triangle_size, 0.f,
        //480 + triangle_size * sqrtf(2), 320 + triangle_size * sqrtf(2), 0.f,
        //480 - triangle_size * sqrtf(2), 320 + triangle_size * sqrtf(2), 0.f,
    //}};
    //std::vector<float> colors = {{
        //1.0f, 0.0f, 0.f,
        //0.0f, 1.0f, 0.f,
        //0.0f, 0.0f, 1.f
    //}};
    //
    std::vector<float> vertices, colors;
    ellipse_outline({200.f, 200.f}, {100.f, 50.f}, 0.1, vertices, colors);
    auto vbo_v = sb::VertexBuffer(vertices, 3);
    auto vbo_c = sb::VertexBuffer(colors, 3);
    auto vao = sb::VertexArray(
        &program, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
    while (ctx.render()) {
        camera->activate();
        program.set_uniform("MVP", camera->MVP());
        vao.render(GL_LINES);
    }
    return 0;
}

