add_library(STL_Mock 
    ./container/vector.cpp)


target_include_directories(STL_Mock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/container)