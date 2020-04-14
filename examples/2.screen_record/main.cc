#include <seabass/seabass.h>

#include <iostream>

int main() {
    auto ctx = sb::Context(sb::colorscheme(sb::Theme::DARK));
    auto window = ctx.create_window("Screen Record", {960, 640});
    auto recorder = ctx.create_recorder("out.mp4");
    auto camera = ctx.create_camera3d(45.f, {0.1, 100.f});
    auto widgets = sb::Widgets(camera, ctx.colorscheme());
    widgets.add<sb::Widget::Cube>();
    widgets.add<sb::Widget::Grid>(-1.f, camera->up_vec());
    while (ctx.render()) {
        widgets.render();
    }
    return 0;
}
