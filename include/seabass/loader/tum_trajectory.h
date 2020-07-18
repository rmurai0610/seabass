#pragma once
#if __has_include(<Eigen/Dense>)
#if __has_include(<sophus/se3.hpp>)
#include <seabass/loader/trajectory.h>

#include <Eigen/Dense>
#include <fstream>
#include <sophus/se3.hpp>
#include <sstream>

namespace sb::loader {
class TUMTrajectory : public sb::loader::Trajectory {
public:
    void load(const std::string &file_name) override {
        std::ifstream file_stream(file_name);
        std::string line;
        while (getline(file_stream, line)) {
            if (line.size() == 0 || line[0] == '#') {
                continue;
            }
            std::vector<float> tokens = split<float>(line, ',');
            if (tokens.size() != 8) {
                std::cerr << "Error TUMTrajectory: Failed to parse\n" << line;
                std::exit(EXIT_FAILURE);
            }
            std::array<float, 8> pose;
            std::copy(tokens.begin(), tokens.end(), pose.begin());
            auto [timestamp, tx, ty, tz, qx, qy, qz, qw] = pose;
            timestamps_.push_back(timestamp);
            camera_poses_T_wc_.emplace_back(Eigen::Quaternionf(qw, qx, qy, qz),
                                            Eigen::Vector3f(tx, ty, tz));
        }
    }
};
};  // namespace sb::loader
#endif
#endif
