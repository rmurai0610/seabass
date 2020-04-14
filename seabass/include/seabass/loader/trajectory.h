#pragma once

#if __has_include(<Eigen/Dense>)
#if __has_include(<sophus/se3.hpp>)
#include <Eigen/Dense>
#include <sophus/se3.hpp>

namespace sb {
namespace loader {
class Trajectory {
protected:
    std::vector<Sophus::SE3f> camera_poses_T_wc_;
    std::vector<float> timestamps_;

    template <typename T>
    T from_string(const std::string &str) const {
        return str;
    }
    template <>
    int from_string<int>(const std::string &str) const {
        return std::stoi(str);
    }
    template <>
    float from_string<float>(const std::string &str) const {
        return std::stof(str);
    }

    template <typename T>
    void split(const std::string &s, char delim, std::vector<T> &result) const {
        std::istringstream iss(s);
        std::string item;
        while (std::getline(iss, item, delim)) {
            result.push_back(from_string<T>(item));
        }
    }

    template <typename T>
    std::vector<T> split(const std::string &s, char delim = ' ') const {
        std::vector<T> elems;
        split(s, delim, elems);
        return elems;
    }

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
#endif
#endif
