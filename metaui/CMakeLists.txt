cmake_minimum_required(VERSION 3.19.1)

project(metaui)

project(metaexe)

add_executable(${PROJECT_NAME}
        "meta.cpp"
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CONAN_PKG::sdl
        )