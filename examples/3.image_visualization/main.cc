#include <seabass/seabass.h>

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main() {
    auto ctx = sb::Context(sb::Theme::LIGHT);
    auto window = ctx.create_window("Segmentation", {960, 640});
    auto camera = ctx.create_camera3d(45.f, {0.1, 100.f});
    auto recorder = ctx.create_recorder("out.mp4");
    auto widgets = sb::Widgets(camera, ctx.colorscheme());
    auto img = sb::Image(
        cv::imread("examples/3.image_visualization/data/image.jpg"), 256, 256);
    auto seg =
        sb::Image(cv::imread("examples/3.image_visualization/data/seg.png"),
                  256, 256, 0.4f);
    float alpha = 0.7f;
    while (ctx.render()) {
        {
            ImGui::Begin("Segmentation");
            ImGui::Text("Loaded using stb");
            ImGui::Image((void *)(intptr_t)img.id(), ImVec2(256, 256));
            ImGui::SameLine();
            ImGui::Image((void *)(intptr_t)seg.id(), ImVec2(256, 256));
            ImGui::SameLine();
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddImage((void *)(intptr_t)img.id(),
                                                 ImVec2(p.x, p.y),
                                                 ImVec2(p.x + 256, p.y + 256));
            ImGui::GetWindowDrawList()->AddImage((void *)(intptr_t)seg.id(),
                                                 ImVec2(p.x, p.y),
                                                 ImVec2(p.x + 256, p.y + 256));
            ImGui::NewLine();
            ImGui::SliderFloat("alpha", &alpha, 0.f, 1.f);
            seg.set_alpha(alpha);
            ImGui::End();
        }
    }
    return 0;
}
