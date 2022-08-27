# Set path to Java Activity class in game engine
set(JNI_ENV_CLASS_PATH "com/example/androidgameengine/GameActivityAR")

string(REPLACE / _ JNI_CLASS_PATH "${JNI_ENV_CLASS_PATH}")

target_compile_definitions(android_game_engine
    PUBLIC "JNI_CLASS_PATH=${JNI_CLASS_PATH}"
    PRIVATE JNI_ENV_CLASS_PATH="${JNI_ENV_CLASS_PATH}"
)

# game_activity_ar lib
add_library(game_activity_ar SHARED GameActivityAR.cpp)

target_include_directories(game_activity_ar PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

target_link_libraries(game_activity_ar PUBLIC android_game_engine)