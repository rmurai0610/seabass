#pragma once
#include <seabass/base/program.h>
#include <seabass/base/vertex_array.h>
#include <seabass/base/vertex_buffer.h>
#include <seabass/base/window.h>

#include <cstdio>
#include <iostream>
#include <string>

namespace sb {
class Recorder {
private:
    sb::Window* window_;
    FILE* ffmpeg_file_;
    std::string filename_;
    int width_, height_;
    sb::Program program_;

    const std::string cmd() const {
        std::string c =
            "ffmpeg -use_wallclock_as_timestamps 1 -f rawvideo -pix_fmt rgb24 "
            "-s " +
            std::to_string(width_) + "x" + std::to_string(height_) +
            " -i - -y -pix_fmt yuv420p -vf vflip -an -b:v 5000k " + filename_ +
            " 2> /dev/null";
        return c;
    }
    void draw_rec_dot() {
        float radius = 20;
        auto vbo_v = sb::VertexBuffer(std::vector<float>({0.9f, 0.9f, 0.f}), 3);
        auto vbo_c = sb::VertexBuffer(std::vector<float>({1.f, 0.f, 0.f}), 3);
        program_.set_uniform("radius", radius);
        auto vao = sb::VertexArray(
            &program_, {{"in_vert", 0, &vbo_v}, {"in_color", 1, &vbo_c}});
        glPointSize(radius);
        vao.render(GL_POINTS);
    }

public:
    Recorder(sb::Window* window, std::string filename)
        : window_(window),
          filename_(filename),
          width_(window_->width()),
          height_(window_->height()) {
        ffmpeg_file_ = popen(cmd().c_str(), "w");
        program_ = sb::Program(
            R"(
            #version 330 core
            layout(location = 0) in vec3 in_vert;
            uniform float radius;
            void main() {
                gl_Position = vec4(in_vert, 1);
                gl_PointSize = radius;
            }
        )",
            R"(
            #version 330 core
            out vec4 color;
            void main(){
                vec2 circle_coord = 2.0 * gl_PointCoord - 1.0;
                if (dot(circle_coord, circle_coord) > 1.0) { discard; }
                color = vec4(1.f, 0.f, 0.f, 1.f);
            }
        )");
    }

    ~Recorder() { pclose(ffmpeg_file_); }

    void write() {
        if (width_ != window_->width() || height_ != window_->height()) {
            return;
        }
        unsigned char pixels[width_ * height_ * 3];
        glReadPixels(0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        if (ffmpeg_file_) {
            fwrite(pixels, width_ * height_ * 3, 1, ffmpeg_file_);
        }
        draw_rec_dot();
    }
};
};  // namespace sb
