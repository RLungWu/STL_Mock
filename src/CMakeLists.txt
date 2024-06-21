add_library(My_STL_Mock 
    ./container/my_vector.cc)


target_include_directories(My_STL_Mock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/container)