cmake_minimum_required(VERSION 3.19.1)

project(metacore)

add_library(${PROJECT_NAME}
        "src/MetaEngine.cpp"
        )

add_subdirectory(test)