#include <seabass/seabass.h>

int main () {

    auto ctx = sb::Context(sb::Theme::LIGHT);
    auto window = ctx.create_window("Hello World 2d", {960, 640});
    auto camera = ctx.create_camera2d();
    auto program = sb::Program();
    float triangle_size = 100.f;
    std::vector<float> vertices = {{
        480, 320 - triangle_size, 0.f,
        480 + triangle_size * sqrtf(2), 320 + triangle_size * sqrtf(2), 0.f,
        480 - triangle_size * sqrtf(2), 320 + triangle_size * sqrtf(2), 0.f,
    }};
    std::vector<float> colors = {{
        1.0f, 0.0f, 0.f,
        0.0f, 1.0f, 0.f,
        0.0f, 0.0f, 1.f
    }};

    auto vbo_v = sb::VertexBuffer(vertices, 3);
    auto vbo_c = sb::VertexBuffer(colors, 3);
    auto vao = sb::VertexArray(
        &program, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
    while (ctx.render()) {
        camera->activate();
        program.set_uniform("MVP", camera->MVP());
        vao.render(GL_TRIANGLES);
    }
    return 0;
}

