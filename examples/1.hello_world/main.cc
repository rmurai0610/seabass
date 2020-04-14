#include <seabass/seabass.h>

#include <Eigen/Dense>
#include <sophus/se3.hpp>

int main() {
    auto ctx = sb::Context(sb::colorscheme(sb::Theme::LIGHT));
    auto window = ctx.create_window("Hello World", {960, 640});
    auto tum_loader = sb::loader::ICLTrajectory();
    tum_loader.load("examples/1.hello_world/data/trajectory.txt");

    auto camera =
        ctx.create_camera3d(45.f, {0.1, 100.f}, sb::AxisDirection::AxisNegY,
                            sb::AxisDirection::AxisZ);
    auto widgets = sb::Widgets(camera, ctx.colorscheme());

    widgets.add<sb::Widget::Coordinate>(3.f);
    widgets.add<sb::Widget::Grid>(-1.f, camera->up_vec());

    // auto keyframes = widgets.add<sb::Widget::Keyframes>(true, 0.1f);
    auto trajectory = widgets.add<sb::Widget::CameraTrajectory>();
    trajectory->set_poses(tum_loader.camera_poses_T_wc());
    while (ctx.render()) {
        widgets.render();
    }
    return 0;
}
