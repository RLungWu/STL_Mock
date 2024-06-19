cmake_minimum_required(VERSION 3.0)
project(STL_Mock)

include_directories(
    ${CMAKE_SOURCE_DIR}/include/container
    ${CMAKE_SOURCE_DIR}/include/iterator)

set(CMAKE_CXX_STANDARD 11)

add_executable(STL_Mock 
    main.cpp)

target_include_directories(
    STL_Mock PUBLIC ${CMAKE_SOURCE_DIR}/include)