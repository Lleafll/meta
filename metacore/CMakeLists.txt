cmake_minimum_required(VERSION 3.19.1)

project(metacore)

add_library(${PROJECT_NAME}
        "src/MetaEngine.cpp"
        "src/Player.cpp"
        "src/PickingUpState.cpp"
        "src/DefaultState.cpp"
        "src/SlashAttackMechanic.cpp"
        "src/Enemies.cpp"
        "src/GameOverState.cpp"
        "src/PickUpGenerator.cpp"
        "src/ShootAttackMechanic.cpp")
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "include"
        PRIVATE
        "include/metacore"
        "src"
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CONAN_PKG::ms-gsl
        )
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

add_subdirectory(test)