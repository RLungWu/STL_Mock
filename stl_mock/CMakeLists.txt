CMAKE_MINIMUM_REQUIRED_VERSION(3.8)

project(STL_Mock)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE STL_MOCK_SOURCES *.cc *.h)

add_library(STL_Mock STATIC ${SOURCE_FILES})

target_include_directories(STL_Mock PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/allocator
)