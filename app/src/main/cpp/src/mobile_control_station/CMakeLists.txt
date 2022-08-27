# Set path to Java Activity class in game engine
set(JNI_ENV_CLASS_PATH "com/example/androidgameengine/MobileControlStation")

string(REPLACE / _ JNI_CLASS_PATH "${JNI_ENV_CLASS_PATH}")

target_compile_definitions(android_game_engine
    PUBLIC "JNI_CLASS_PATH=${JNI_CLASS_PATH}"
    PRIVATE JNI_ENV_CLASS_PATH="${JNI_ENV_CLASS_PATH}"
)

# mobile_control_station lib
add_library(mobile_control_station SHARED
    ImageMsgDisplay.cpp
    MobileControlStation.cpp
)

target_include_directories(mobile_control_station PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

target_link_libraries(mobile_control_station PUBLIC android_game_engine)