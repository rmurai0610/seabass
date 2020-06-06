cmake_minimum_required(VERSION 3.10)
project(seabass)

add_library(seabass src/seabass.cc)

target_compile_options(seabass PRIVATE -Werror -Wall -Wno-unused-variable -DGL_SILENCE_DEPRECATION)
target_compile_features(seabass PRIVATE cxx_std_17)
target_include_directories(seabass PUBLIC include/)

target_link_libraries(seabass PUBLIC imgui)
