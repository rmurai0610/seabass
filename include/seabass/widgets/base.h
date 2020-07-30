#pragma once

#include <seabass/base/camera.h>
#include <seabass/base/program.h>
#include <seabass/base/themes.h>
#include <seabass/base/vertex_array.h>
#include <seabass/base/vertex_buffer.h>

namespace sb {
namespace Widget {
class Base {
public:
    Base() {}
    virtual ~Base() {}
    virtual void render(const sb::Program *program,
                        const sb::ColorScheme *colorscheme) = 0;
};
};  // namespace Widget
};  // namespace sb
