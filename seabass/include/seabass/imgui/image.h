#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>
#pragma clang diagnostic pop

#include <iostream>
#if __has_include(<opencv2/core.hpp>)
#define have_opencv 1
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#else
#define have_opencv 0
#endif

#include <seabass/opengl_loader.h>

namespace sb {
class Image {
private:
    std::string filename_;
    int color_depths_, channels_;
    int width_, height_, alpha_;
    GLuint id_;
    unsigned char *data_;

    void upload(unsigned char *data) {
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void format_and_upload(void *data, int width_img, int height_img) {
        if (data_ != nullptr) {
            delete[] data_;
            data_ = nullptr;
        }
        if (width_ <= 0 || height_ <= 0) {
            width_ = width_img;
            height_ = height_img;
        }
        if (width_ > width_img || height_ > height_img) {
            std::cerr << "Specified image size must be smaller than the size "
                         "of loaded image.\n";
            std::cerr << width_ << " " << height_ << "\n";
            std::cerr << width_img << " " << height_img << "\n";
            std::exit(EXIT_FAILURE);
        }

        int i1 = std::floor((height_img - height_) / 2.f);
        int j1 = std::floor((width_img - width_) / 2.f);

        if (color_depths_ == 8 && channels_ == 1) {
            data_ = new unsigned char[width_ * height_ * 4];
            for (int i = 0; i < height_; ++i) {
                for (int j = 0; j < width_; ++j) {
                    int idx = j + i * width_;
                    int data_idx = (j + j1) + (i + i1) * width_img;
                    data_[idx * 4] = ((unsigned char *)data)[data_idx];
                    data_[idx * 4 + 1] = ((unsigned char *)data)[data_idx];
                    data_[idx * 4 + 2] = ((unsigned char *)data)[data_idx];
                    data_[idx * 4 + 3] = alpha_;
                }
            }
            upload(data_);
        } else if (color_depths_ == 8 && channels_ == 3) {
            data_ = new unsigned char[width_ * height_ * 4];
            for (int i = 0; i < height_; ++i) {
                for (int j = 0; j < width_; ++j) {
                    int idx = j + i * width_;
                    int data_idx = (j + j1) + (i + i1) * width_img;
                    data_[idx * 4] = ((unsigned char *)data)[data_idx * 3];
                    data_[idx * 4 + 1] =
                        ((unsigned char *)data)[data_idx * 3 + 1];
                    data_[idx * 4 + 2] =
                        ((unsigned char *)data)[data_idx * 3 + 2];
                    data_[idx * 4 + 3] = alpha_;
                }
            }
            upload(data_);
        } else if (color_depths_ == 16 && channels_ == 1) {
            data_ = new unsigned char[width_ * height_ * 4];
            float i_max = 0.f;
            for (int i = 0; i < height_; ++i) {
                for (int j = 0; j < width_; ++j) {
                    int data_idx = (j + j1) + (i + i1) * width_img;
                    i_max = std::max(float(((short *)data)[data_idx]), i_max);
                }
            }
            for (int i = 0; i < height_; ++i) {
                for (int j = 0; j < width_; ++j) {
                    int idx = j + i * width_;
                    int data_idx = (j + j1) + (i + i1) * width_img;
                    unsigned char p =
                        (float(((short *)data)[data_idx]) / i_max) *
                        std::numeric_limits<unsigned char>::max();
                    data_[idx * 4] = p;
                    data_[idx * 4 + 1] = p;
                    data_[idx * 4 + 2] = p;
                    data_[idx * 4 + 3] = alpha_;
                }
            }
            upload(data_);
        } else {
            std::cerr << color_depths_ << " bit " << channels_
                      << " channel image is not supported.\n";
            std::exit(EXIT_FAILURE);
        }
    }

public:
    Image(const std::string &filename, int color_depths = 8, int width = -1,
          int height = -1, float alpha = 1.f)
        : color_depths_(color_depths),
          width_(width),
          height_(height),
          alpha_(alpha * 255),
          data_(nullptr) {
        glGenTextures(1, &id_);
        set(filename);
    }
    ~Image() {
        if (data_ != nullptr) {
            delete[] data_;
        }
    }
    void set(const std::string &filename) {
        filename_ = filename;
        void *img;
        int width_img, height_img;
        switch (color_depths_) {
            case 8:
                img = stbi_load(filename_.c_str(), &width_img, &height_img,
                                &channels_, 0);
                break;
            case 16:
                img = stbi_load_16(filename_.c_str(), &width_img, &height_img,
                                   &channels_, 0);
                break;
            default:
                std::cerr << color_depths_
                          << " bit color depth not supported.\n";
                std::exit(EXIT_FAILURE);
        }
        format_and_upload(img, width_img, height_img);
        stbi_image_free(img);
    }
#if have_opencv
    Image(const cv::Mat &img, int width = -1, int height = -1,
          float alpha = 1.f)
        : width_(width), height_(height), alpha_(alpha * 255), data_(nullptr) {
        glGenTextures(1, &id_);
        switch (img.depth()) {
            case CV_8U:
            case CV_8S:
                color_depths_ = 8;
                break;
            case CV_16U:
            case CV_16S:
                color_depths_ = 16;
                break;
            case CV_32S:
            case CV_32F:
                color_depths_ = 32;
                break;
            case CV_64F:
                color_depths_ = 64;
                break;
            default:
                std::cerr << "Unsupported image depth: " << img.depth() << "\n";
                std::exit(EXIT_FAILURE);
        };
        channels_ = img.channels();
        cv::Mat img_rgb = img;
        if (channels_ == 3) {
            cv::cvtColor(img, img_rgb, cv::COLOR_BGR2RGB);
        }
        set(img_rgb);
    }
    void set(const cv::Mat &img) {
        format_and_upload(img.data, img.cols, img.rows);
    }
#endif
    uint32_t width() const { return width_; }
    uint32_t height() const { return height_; }
    GLuint id() const { return id_; }

    void set_alpha(float alpha) {
        if (alpha_ == alpha) {
            return;
        }
        alpha_ = alpha * 255.f;
        if (data_ != nullptr) {
            for (int i = 0; i < width_ * height_; ++i) {
                data_[i * 4 + 3] = alpha_;
            }
        }
        upload(data_);
    }
};  // namespace Image
};  // namespace sb

#undef have_opencv
