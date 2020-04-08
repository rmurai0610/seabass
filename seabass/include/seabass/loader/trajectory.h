#pragma once
#include <Eigen/Eigen>
#include <sophus/se3.hpp>

namespace sb {
namespace loader {
class Trajectory {
protected:
    std::vector<Sophus::SE3f> camera_poses_T_wc_;
    std::vector<float> timestamps_;

public:
    virtual void load(const std::string &filename) = 0;
    const std::vector<Sophus::SE3f> &camera_poses_T_wc() const {
        return camera_poses_T_wc_;
    }
    std::vector<Sophus::SE3f> &camera_poses_T_wc() {
        return camera_poses_T_wc_;
    }
    const std::vector<float> &timestamps() const { return timestamps_; }
    std::vector<float> &timestamps() { return timestamps_; }
};
};  // namespace loader
};  // namespace sb
