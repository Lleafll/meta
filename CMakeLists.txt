cmake_minimum_required(VERSION 3.19.1)

project(meta)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(${PROJECT_NAME}
        "meta.cpp"
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CONAN_PKG::sdl
        )

add_subdirectory(metacore)