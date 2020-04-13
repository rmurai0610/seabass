cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

project(seabass)

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(imgui
  build/imgui/imgui_demo.cpp
  build/imgui/imgui.cpp
  build/imgui/imgui_draw.cpp
  build/imgui/imgui_widgets.cpp
  build/imgui/examples/imgui_impl_glfw.cpp
  build/imgui/examples/imgui_impl_opengl3.cpp
  build/imgui/examples/libs/gl3w/GL/gl3w.c
)
set_target_properties(imgui PROPERTIES LINKER_LANGUAGE CXX)
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(imgui PUBLIC
    "-framework Cocoa, -framework OpenGL, -framework IOKit, -framework CoreVideo"
  )
endif()

target_compile_options(imgui PRIVATE -Werror -Wall)
target_compile_features(imgui PRIVATE cxx_std_17)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GL3W)
target_include_directories(imgui PUBLIC
  build/imgui/
  build/imgui/examples/
  build/imgui/examples/libs/gl3w/
)
target_include_directories(imgui SYSTEM PUBLIC $ENV{INCLUDE})
target_link_libraries(imgui PUBLIC
  OpenGL::GL
  glfw
  ${CMAKE_DL_LIBS}
)

add_subdirectory(seabass)
add_subdirectory(examples)

