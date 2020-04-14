cmake_minimum_required(VERSION 3.10)
project(seabass)

#find_package(Eigen3 3.3.4 REQUIRED)
#find_package(Sophus 1.0.0 REQUIRED)

add_library(seabass
  src/seabass.cc
)

target_compile_options(seabass PRIVATE -Werror -Wall -Wno-unused-variable -DGL_SILENCE_DEPRECATION)
target_compile_features(seabass PRIVATE cxx_std_17)
target_include_directories(seabass
  PUBLIC
    include/
    #${EIGEN3_INCLUDE_DIR}
    #${Sophus_INCLUDE_DIRS}
)

target_link_libraries(seabass
 PUBLIC
    imgui
    glm
    #Eigen3::Eigen
    #${Sophus_LIBRARIES}
)
