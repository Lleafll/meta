cmake_minimum_required(VERSION 3.19.1)

project(metacore)

add_library(${PROJECT_NAME}
        "src/MetaEngine.cpp"
        "src/Player.cpp"
        "src/PickingUpState.cpp"
        "src/DefaultState.cpp"
        "src/SlashAttackMechanic.cpp"
        "src/Enemies.cpp"
        "src/InitialState.cpp"
        "src/InitialPickingUpState.cpp"
        "src/GameOverState.cpp")
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "include"
        PRIVATE
        "include/metacore"
        "src"
        )
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

add_subdirectory(test)