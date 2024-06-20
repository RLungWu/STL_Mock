add_library(STL_Mock_Test 
    ./container/vector_test.cc)


target_include_directories(MY_STL_Mock PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/container)