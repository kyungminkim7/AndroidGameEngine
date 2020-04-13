# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.5...3.10)
project(game)

add_subdirectory(android_game_engine)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} SHARED TestGame.cpp TestGame.h)
#add_library(${PROJECT_NAME} SHARED TestGameAR.cpp TestGameAR.h)

target_include_directories(game
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(game PUBLIC android_game_engine)
