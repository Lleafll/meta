cmake_minimum_required(VERSION 3.19.1)

project(metacore)

add_library(${PROJECT_NAME}
        "src/DefaultState.cpp"
        "src/DungeonLayoutMechanic.cpp"
        "src/Clock.cpp"
        "src/Enemies.cpp"
        "src/GameOverState.cpp"
        "src/InternalGameState.cpp"
        "src/Layout.cpp"
        "src/MetaEngine.cpp"
        "src/OpenWorldLayoutMechanic.cpp"
        "src/PickingUpState.cpp"
        "src/PickUpGenerator.cpp"
        "src/Player.cpp"
        "src/ShootAttackMechanic.cpp"
        "src/SlashAttackMechanic.cpp"
        )
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