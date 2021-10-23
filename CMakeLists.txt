cmake_minimum_required(VERSION 3.19.1)

project(meta)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

enable_testing()

add_subdirectory(metacore)
add_subdirectory(metaui)