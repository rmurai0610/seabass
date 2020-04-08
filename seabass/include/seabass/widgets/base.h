#pragma once

#include <seabass/camera.h>
#include <seabass/program.h>
#include <seabass/themes.h>

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
