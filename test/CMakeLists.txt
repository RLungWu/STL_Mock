cmake_minimum_required(VERSION 3.0)
project (test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE TEST_SRC "*.cc")

include_directories(${CMAKE_SOURCE_DIR}/../src)

add_executable(${PROJECT_NAME} ${TEST_SRC})

target_link_libraries(${PROJECT_NAME} gtest::gtest gtest::gtest_main STL)