cmake_minimum_required(VERSION 3.0)

project(STL_Mock)

message("BUilding type is ${CMAKE_BUILD_TYPE}")

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

if (ENABLE_MINISTL_TEST)
    find_package(GTest REQUIRED)
    add_subdirectory("Test")
endif()


