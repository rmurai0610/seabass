#include <seabass/seabass.h>

#include <Eigen/Eigen>
#include <sophus/se3.hpp>

int main() {
    auto ctx = sb::Context(sb::colorscheme(sb::Theme::LIGHT));
    auto window = ctx.create_window("Hello World", {960, 640});

    std::vector<float> vertices, colors;
    for (int i = 0; i < 1000; ++i) {
        vertices.push_back(rand() % 100 * 0.01 - 0.5f);
        vertices.push_back(rand() % 100 * 0.01 - 0.5f);
        vertices.push_back(rand() % 100 * 0.01 + 0.5f);
        colors.push_back(rand() % 100 * 0.01);
        colors.push_back(rand() % 100 * 0.01);
        colors.push_back(rand() % 100 * 0.01);
    }

    auto T_wc =
        Sophus::SE3f(Sophus::SO3f(Eigen::Quaternionf(0, 0.258819, 0, 0.965926)),
                     Eigen::Vector3f(-0.5f, 0.0f, 0.0f));
    auto T_wc1 =
        Sophus::SE3f(Sophus::SO3f(Eigen::Quaternionf(0, 0.258819, 0, 0.965926)),
                     Eigen::Vector3f(-0.5f, 0.1f, 0.0f));
    auto camera = ctx.create_camera3d(45.f, {0.1, 100.f});
    auto widgets = sb::Widgets(camera, ctx.colorscheme());
    widgets.add<sb::Widget::Coordinate>(3.f);
    widgets.add<sb::Widget::Grid>(-1.f);
    widgets.add<sb::Widget::Camera>(T_wc, true, 1.f, 1.f);
    auto keyframes = widgets.add<sb::Widget::Keyframes>(true, 0.1f);
    widgets.add<sb::Widget::Pointcloud>(vertices, colors);
    auto camera_trajectory = widgets.add<sb::Widget::CameraTrajectory>();
    keyframes->add_keyframe(T_wc);
    keyframes->add_keyframe(T_wc1);
    camera_trajectory->add_pose(T_wc);
    camera_trajectory->add_pose(T_wc1);

    while (ctx.render()) {
        widgets.render();
        {
            ImGui::Begin("Hello World");
            ImGui::Text("Hello world!");
            ImGui::End();
        }
    }
    return 0;
}
