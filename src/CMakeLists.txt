add_library(My_STL_Mock 
    ./container/my_vector.cc
    ./iterator/my_allocator.cc)


target_include_directories(My_STL_Mock PUBLIC
    ${CMAKE_SOURCE_DIR}/include/)