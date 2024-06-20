cmake_minimum_required(VERSION 3.0)
project(STL_Mock)

set(CMAKE_CXX_STANDARD 11)

add_executable(My_STL_Mock main.cc)

    
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)


target_include_directories(
    MY_STL_Mock PUBLIC ${CMAKE_SOURCE_DIR}/include/container
    MY_STL_Mock PUBLIC ${CMAKE_SOURCE_DIR}/include/iterator)