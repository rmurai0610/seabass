cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(seabass)

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

execute_process(
  COMMAND ./build.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
add_library(imgui STATIC
  imgui/imgui_demo.cpp
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_widgets.cpp
  imgui/examples/imgui_impl_glfw.cpp
  imgui/examples/imgui_impl_opengl3.cpp
  imgui/examples/libs/gl3w/GL/gl3w.c
  imgui/implot/implot.cpp
  imgui/implot/implot_items.cpp
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
  imgui/
  imgui/examples/
  imgui/examples/libs/gl3w/
  imgui/implot/
)
target_include_directories(imgui SYSTEM PUBLIC $ENV{INCLUDE})
target_link_libraries(imgui PUBLIC
  OpenGL::GL
  glfw
  ${CMAKE_DL_LIBS}
)

add_library(seabass INTERFACE)

target_compile_options(seabass INTERFACE -Werror -Wall -Wno-unused-variable -DGL_SILENCE_DEPRECATION)
target_compile_features(seabass INTERFACE cxx_std_17)
target_include_directories(seabass INTERFACE include/)

target_link_libraries(seabass INTERFACE imgui)

add_subdirectory(examples)

