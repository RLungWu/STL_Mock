cmake_minimum_required(VERSION 3.0)
project(STL_Mock)

set(CMAKE_CXX_STANDARD 11)

add_executable(My_STL_Mock main.cpp)

    
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)


target_include_directories(
    STL_Mock PUBLIC ${CMAKE_SOURCE_DIR}/include/container
    STL_Mock PUBLIC ${CMAKE_SOURCE_DIR}/include/iterator
    STL_Mock PUBLIC ${CMAKE_SOURCE_DIR}/test/container
    STL_Mock PUBLIC ${CMAKE_SOURCE_DIR}/test/iterator)