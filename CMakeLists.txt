cmake_minimum_required(VERSION 3.27)
project(Engine_1_Testing)

set(CMAKE_CXX_STANDARD 17)

add_library(Engine_1_Testing INTERFACE)
target_include_directories(Engine_1_Testing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(Engine_1_Testing INTERFACE)
