find_package(Eigen3 3.3.4)
if(Eigen3_FOUND)
  add_executable(hello_world_2d main.cc)
  target_include_directories(hello_world_2d PUBLIC ${EIGEN3_INCLUDE_DIRS})
  target_link_libraries(hello_world_2d seabass Eigen3::Eigen)
  target_compile_features(hello_world_2d PRIVATE cxx_std_17)
endif()

