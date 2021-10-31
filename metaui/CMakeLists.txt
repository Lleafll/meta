cmake_minimum_required(VERSION 3.19.1)

project(metaui)

project(metaexe)

add_executable(${PROJECT_NAME}
        "meta.cpp"
        )
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CONAN_PKG::sdl
        CONAN_PKG::sdl_image
        CONAN_PKG::sdl_ttf
        metacore
        )
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/Pixel_Berry_08_84_Ltd.Edition.TTF"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Pixel_Berry_08_84_Ltd.Edition.TTF"
)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/alien.png"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/alien.png"
)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/car.png"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/car.png"
)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/knight.png"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/knight.png"
)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/robot.png"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/robot.png"
)
