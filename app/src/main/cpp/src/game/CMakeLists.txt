target_compile_definitions(android_game_engine
    PUBLIC JNI_CLASS_PATH=com_example_androidgameengine_GameActivity
    PRIVATE JNI_ENV_CLASS_PATH="com/example/androidgameengine/GameActivity"
)

add_library(game SHARED TestGame.cpp)

target_include_directories(game INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(game PUBLIC android_game_engine)