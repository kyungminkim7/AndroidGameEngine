cmake_minimum_required(VERSION 3.0)

add_subdirectory(android_game_engine)
add_subdirectory(glm)

# STB library
add_library(stb INTERFACE)

target_include_directories(stb INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/stb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/stb>)