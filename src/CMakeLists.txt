cmake_minimum_required(VERSION 3.0)

project(mystl)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES *.cc *.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms
        ${CMAKE_CURRENT_SOURCE_DIR}/Allocator
        ${CMAKE_CURRENT_SOURCE_DIR}/AssociativeContainers
        ${CMAKE_CURRENT_SOURCE_DIR}/Function
        ${CMAKE_CURRENT_SOURCE_DIR}/Iterator
        ${CMAKE_CURRENT_SOURCE_DIR}/SequenceContainers
        ${CMAKE_CURRENT_SOURCE_DIR}/Utils)