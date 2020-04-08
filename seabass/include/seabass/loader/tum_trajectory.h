#pragma once
#include <seabass/loader/trajectory.h>

#include <fstream>
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
            std::stringstream ss(line);
            float timestamp, tx, ty, tz, qx, qy, qz, qw;
            ss >> timestamp >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
            timestamps_.push_back(timestamp);
            camera_poses_T_wc_.emplace_back(Eigen::Quaternionf(qw, qx, qy, qz),
                                            Eigen::Vector3f(tx, ty, tz));
        }
    }
};

};  // namespace sb::loader
