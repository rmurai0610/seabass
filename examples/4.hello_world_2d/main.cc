#include <seabass/seabass.h>


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
    auto widgets = sb::Widgets(camera, ctx.colorscheme());

    widgets.add<sb::Widget::Circle>(480, 320, 100, true);
    widgets.add<sb::Widget::Ellipse>(480, 320, 200, 150, 3.14 / 4, false);
    while (ctx.render()) {
        camera->activate();
        widgets.render();
    }
    return 0;
}

