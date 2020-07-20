#include <seabass/seabass.h>

int main () {

    auto ctx = sb::Context(sb::Theme::LIGHT);
    auto window = ctx.create_window("Hello World 2d", {960, 640});
    auto camera = ctx.create_camera2d();
    // Load trajectory data
    auto loader = sb::loader::ICLTrajectory();
    loader.load("examples/1.hello_world/data/trajectory.txt");

    auto widgets = sb::Widgets(camera, ctx.colorscheme());

    widgets.add<sb::Widget::Coordinate>(3.f);
    widgets.add<sb::Widget::Grid>(-1.f, camera->up_vec());

    auto trajectory = widgets.add<sb::Widget::CameraTrajectory>();
    trajectory->set_poses(loader.camera_poses_T_wc());
    while (ctx.render()) {
        widgets.render();
    }

    return 0;
}

