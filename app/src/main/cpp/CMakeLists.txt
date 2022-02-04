# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.5...3.10)
project(game)

add_subdirectory(android_game_engine)

set(CMAKE_CXX_STANDARD 14)

# Create game shared library
set(gameType NORMAL)
if (gameType STREQUAL AR)
    add_library(${PROJECT_NAME} SHARED TestGameAR.cpp TestGameAR.h)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        JAVA_ACTIVITY=GameActivityAR
        JNI_ENV_CLASS_PATH="com/example/androidgameengine/GameActivityAR")
elseif(gameType STREQUAL MOBILE_CONTROL_STATION)
    add_library(${PROJECT_NAME} SHARED
        ImageMsgDisplay.cpp
        ImageMsgDisplay.h
        MobileControlStation.cpp
        MobileControlStation.h)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        JAVA_ACTIVITY=GameActivity
        JNI_ENV_CLASS_PATH="com/example/androidgameengine/GameActivity")
else() # Regular game type
    add_library(${PROJECT_NAME} SHARED TestGame.cpp TestGame.h)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        JAVA_ACTIVITY=GameActivity
        JNI_ENV_CLASS_PATH="com/example/androidgameengine/GameActivity")
endif()

# Target properties
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_link_libraries(${PROJECT_NAME} PUBLIC android_game_engine::android_game_engine)
