CMAKE_MINIMUM_REQUIRED(VERSION 3.10.0)

project(mystl)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE MOCK_FILES *.cc *.h)

add_library(${PROJECT_NAME} STATIC ${MOCK_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 
        #${CMAKE_CURRENT_SOURCE_DIR}/Algorithms
        ${CMAKE_CURRENT_SOURCE_DIR}/allocator
        #${CMAKE_CURRENT_SOURCE_DIR}/AssociativeContainers
        #${CMAKE_CURRENT_SOURCE_DIR}/Function
        #${CMAKE_CURRENT_SOURCE_DIR}/Iterator
        #${CMAKE_CURRENT_SOURCE_DIR}/SequenceContainers
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
