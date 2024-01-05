cmake_minimum_required(VERSION 3.27)
project(Engine_1_Testing)

set(CMAKE_CXX_STANDARD 17)

add_library(Engine_1_Testing INTERFACE engine_1_tests.h test_mod.h)
target_include_directories(Engine_1_Testing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Engine_1_Testing_Test main.cpp)
target_link_libraries(Engine_1_Testing_Test Engine_1_Testing)