cmake_minimum_required(VERSION 3.19.1)

project(metacoretest)

add_executable(${PROJECT_NAME}
        "MetaEngineTest.cpp"
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CONAN_PKG::gtest
        )