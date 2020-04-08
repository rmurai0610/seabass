add_executable(hello_world main.cc)
target_link_libraries(hello_world seabass)
target_compile_features(hello_world PRIVATE cxx_std_17)
