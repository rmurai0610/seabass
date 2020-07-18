cmake_minimum_required(VERSION 3.10)
project(seabass)

add_library(seabass INTERFACE)

target_compile_options(seabass INTERFACE -Werror -Wall -Wno-unused-variable -DGL_SILENCE_DEPRECATION)
target_compile_features(seabass INTERFACE cxx_std_17)
target_include_directories(seabass INTERFACE include/)

target_link_libraries(seabass INTERFACE imgui)
