#pragma once
#include <array>
#include <iostream>

namespace sb {
enum class Theme { LIGHT, DARK };
class ColorScheme {
protected:
    Theme theme_;
    std::array<float, 4> bg_;
    std::array<float, 4> grid_;
    std::array<float, 4> camera_;
    std::array<float, 4> keyframes_;
    std::array<float, 4> trajectory_;

public:
    ColorScheme() {}
    virtual ~ColorScheme() {}
    Theme theme() const { return theme_; }
    std::array<float, 4> bg() const { return bg_; }
    std::array<float, 4> grid() const { return grid_; }
    std::array<float, 4> camera() const { return camera_; }
    std::array<float, 4> keyframes() const { return keyframes_; }
    std::array<float, 4> trajectory() const { return trajectory_; }
};

class ColorSchemeLight : public ColorScheme {
public:
    ColorSchemeLight() {
        theme_ = sb::Theme::LIGHT;
        bg_ = {1.f, 1.f, 1.f, 1.f};
        grid_ = {0.7f, 0.7f, 0.7f, 1.f};
        camera_ = {1.f, 0.f, 0.f, 1.f};
        keyframes_ = {0.f, 1.f, 0.f, 1.f};
        trajectory_ = {0.2, 0.2, 0.2, 1.f};
    }
};

class ColorSchemeDark : public ColorScheme {
public:
    ColorSchemeDark() {
        theme_ = sb::Theme::DARK;
        bg_ = {0.f, 0.f, 0.f, 1.f};
        grid_ = {0.3f, 0.3f, 0.3f, 1.f};
        camera_ = {1.f, 0.f, 0.f, 1.f};
        keyframes_ = {0.f, 1.f, 0.f, 1.f};
        trajectory_ = {0.8, 0.8, 0.8, 1.f};
    }
};

ColorScheme colorscheme(Theme theme) {
    switch (theme) {
        case Theme::LIGHT:
            return ColorSchemeLight();
        case Theme::DARK:
            return ColorSchemeDark();
        default:
            std::cerr << "Colorscheme not found\n";
            std::exit(EXIT_FAILURE);
    }
}
};  // namespace sb
