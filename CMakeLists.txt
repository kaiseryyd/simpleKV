cmake_minimum_required(VERSION 3.23)
project(kv)

set(GOOGLETEST_VERSION 1.12.1)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory(
        external/googletest
)
include_directories(
        external/googletest/include
)
add_executable(kv main.cpp)
target_link_libraries(kv gtest_main)
