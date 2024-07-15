cmake_minimum_required(VERSION 3.8)

project(STL_Mock)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CSS_STANDARD_REQUIRED ON)

add_subdirectory(stl_mock)

if(ENABLE_STL_MOCK_TESTS)
    find_package(GTest REQUIRED)
    add_subdirectory("test")
endif()