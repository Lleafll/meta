cmake_minimum_required(VERSION 3.19.1)

project(metacoretest)

add_executable(${PROJECT_NAME}
        "DefaultStateTest.cpp"
        "EnemiesTest.cpp"
        "GameOverStateTest.cpp"
        "MetaEngineTest.cpp"
        "PickingUpStateTest.cpp"
        "PlayerTest.cpp"
        "PositionTest.cpp"
        "ShootAttackMechanicTest.cpp"
        "SlashAttackMechanicTest.cpp"
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        metacore
        CONAN_PKG::gtest
        )
target_include_directories(${PROJECT_NAME}
        PRIVATE
        "../include/metacore"
        "../src"
        )
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
