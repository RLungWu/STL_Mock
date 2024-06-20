add_library(MY_STL_Mock 
    ./container/vector.cc)


target_include_directories(MY_STL_Mock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/container)