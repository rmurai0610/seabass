#pragma once

#include <seabass/camera.h>
#include <seabass/program.h>
#include <seabass/themes.h>
#include <seabass/vertex_array.h>
#include <seabass/vertex_buffer.h>
#include <seabass/widgets/base.h>
#include <seabass/widgets/camera.h>
#include <seabass/widgets/camera_trajectory.h>
#include <seabass/widgets/coordinate.h>
#include <seabass/widgets/cube.h>
#include <seabass/widgets/grid.h>
#include <seabass/widgets/keyframes.h>
#include <seabass/widgets/pointcloud.h>

#include <Eigen/Dense>
#include <array>
#include <sophus/se3.hpp>

namespace sb {

class Widgets {
private:
    const sb::Camera *camera_;
    sb::Program program_;  // Use a default program
    const sb::ColorScheme colorscheme_;
    std::vector<std::unique_ptr<sb::Widget::Base>> widgets_;

public:
    Widgets(const sb::Camera *camera, const sb::ColorScheme &colorscheme)
        : camera_(camera), colorscheme_(colorscheme) {}
    template <class T>
    T *add() {
        widgets_.push_back(std::move(std::make_unique<T>()));
        return static_cast<T *>(widgets_.back().get());
    }
    template <class T, class... U>
    T *add(U &&... args) {
        widgets_.push_back(
            std::move(std::make_unique<T>(std::forward<U>(args)...)));
        return static_cast<T *>(widgets_.back().get());
    }
    void render() {
        program_.set_uniform("MVP", camera_->MVP());
        for (auto &&widget : widgets_) {
            widget->render(&program_, &colorscheme_);
        }
    }
};
};  // namespace sb
