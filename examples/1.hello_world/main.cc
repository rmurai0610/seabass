#include <seabass/seabass.h>

#include <Eigen/Dense>
#include <sophus/se3.hpp>

int main() {
    auto ctx = sb::Context(sb::colorscheme(sb::Theme::LIGHT));
    auto window = ctx.create_window("Hello World", {960, 640});
    // Load trajectory data
    auto loader = sb::loader::ICLTrajectory();
    loader.load("examples/1.hello_world/data/trajectory.txt");

    // Set coordinate frame to neg_y right, z up
    auto camera =
        ctx.create_camera3d(45.f, {0.1, 100.f}, sb::AxisDirection::AxisNegY,
                            sb::AxisDirection::AxisZ);
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
