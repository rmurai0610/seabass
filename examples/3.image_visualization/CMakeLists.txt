find_package(OpenCV)

if(OpenCV_LIBS)
  add_executable(image_visualization main.cc)
  target_link_libraries(image_visualization seabass ${OpenCV_LIBS})
  target_compile_features(image_visualization PRIVATE cxx_std_17)
endif()
