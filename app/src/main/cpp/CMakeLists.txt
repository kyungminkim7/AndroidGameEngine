# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.13)
project(game)

add_subdirectory(android_game_engine)

set(CMAKE_CXX_STANDARD 14)

# Create game shared library
set(gameType NORMAL)
#set(gameType AR)
#set(gameType MOBILE_CONTROL_STATION)

if (gameType STREQUAL AR)
    add_library(${PROJECT_NAME} SHARED TestGameAR.cpp TestGameAR.h)

#   Add arcore dependency to android_game_engine
    target_sources(android_game_engine PRIVATE
        android_game_engine/src/ARCameraBackground.cpp
        android_game_engine/src/ARPlane.cpp
        android_game_engine/src/GameAR.cpp)

    target_compile_definitions(android_game_engine
        PUBLIC JNI_CLASS_PATH=com_example_androidgameengine_GameActivityAR
        PRIVATE JNI_ENV_CLASS_PATH="com/example/androidgameengine/GameActivityAR")

    add_library(arcore SHARED IMPORTED)
    set_target_properties(arcore PROPERTIES IMPORTED_LOCATION
        ${ARCORE_LIBPATH}/${ANDROID_ABI}/libarcore_sdk_c.so
        INTERFACE_INCLUDE_DIRECTORIES ${ARCORE_INCLUDE})
    target_link_libraries(android_game_engine PUBLIC arcore)

else()
    if (gameType STREQUAL MOBILE_CONTROL_STATION)
        add_library(${PROJECT_NAME} SHARED
            ImageMsgDisplay.cpp
            ImageMsgDisplay.h
            MobileControlStation.cpp
            MobileControlStation.h)
    else() # Regular game type
        add_library(${PROJECT_NAME} SHARED TestGame.cpp TestGame.h)
    endif()

    target_compile_definitions(android_game_engine
        PUBLIC JNI_CLASS_PATH=com_example_androidgameengine_GameActivity
        PRIVATE JNI_ENV_CLASS_PATH="com/example/androidgameengine/GameActivity")
endif()

# Target properties
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_link_libraries(${PROJECT_NAME} PUBLIC android_game_engine::android_game_engine)
