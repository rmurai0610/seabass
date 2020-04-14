find_package(Eigen3 3.3.4)
find_package(Sophus 1.0.0)
if(Eigen3_FOUND AND Sophus_FOUND)
  add_executable(hello_world main.cc)
  target_include_directories(hello_world
    PUBLIC
    ${EIGEN3_INCLUDE_DIRS}
      ${Sophus_INCLUDE_DIRS}
  )
  target_link_libraries(hello_world seabass Eigen3::Eigen ${Sophus_LIBRARIES})
  target_compile_features(hello_world PRIVATE cxx_std_17)

endif()

