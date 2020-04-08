#pragma once
#include <seabass/loader/trajectory.h>

#include <fstream>
namespace sb::loader {
class ICLTrajectory : public sb::loader::Trajectory {
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
                std::cerr << "Error ICLTrajectory: Failed to parse\n" << line;
                std::exit(EXIT_FAILURE);
            }
            std::array<float, 8> pose;
            std::copy(tokens.begin(), tokens.end(), pose.begin());
            auto [timestamp, tx, ty, tz, qw, qx, qy, qz] = pose;
            timestamps_.push_back(timestamp);
            camera_poses_T_wc_.emplace_back(Eigen::Quaternionf(qw, qx, qy, qz),
                                            Eigen::Vector3f(tx, ty, tz));
        }
    }
};

};  // namespace sb::loader
