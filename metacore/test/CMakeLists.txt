cmake_minimum_required(VERSION 3.19.1)

project(metacoretest)

add_executable(${PROJECT_NAME}
        "MetaEngineTest.cpp"
        "PositionTest.cpp"
        "PickingUpStateTest.cpp"
        "DefaultStateTest.cpp"
        "PlayerTest.cpp"
        "SlashAttackMechanicTest.cpp"
        "InitialStateTest.cpp"
        "InitialPickingUpStateTest.cpp"
        "EnemiesTest.cpp")
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
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
