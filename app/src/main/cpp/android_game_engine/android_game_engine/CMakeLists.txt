cmake_minimum_required(VERSION 3.0)
project(android_game_engine)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_link_libraries(native_app_glue PUBLIC log)

add_library(${PROJECT_NAME} STATIC
        "src/Asset.cpp"
        "src/Box.cpp"
        "src/Camera.cpp"
        "src/CameraFPV.cpp"
        "src/DirectionalLight.cpp"
        "src/ElementBufferObject.cpp"
        "src/Game.cpp"
        "src/GameObject.cpp"
        "src/Joystick.cpp"
        "src/JoystickHandle.cpp"
        "src/Light.cpp"
        "src/Log.cpp"
        "src/ManagerAssets.cpp"
        "src/ManagerWindowing.cpp"
        "src/Mesh.cpp"
        "src/Model.cpp"
        "src/Quad.cpp"
        "src/ShaderProgram.cpp"
        "src/Skybox.cpp"
        "src/Texture2D.cpp"
        "src/Utilities.cpp"
        "src/VertexBufferObject.cpp"
        "src/Widget.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        android
        EGL
        GLESv2
        glm
        log
        native_app_glue
    PRIVATE
        stb)

target_compile_features(${PROJECT_NAME}
    PUBLIC
        cxx_constexpr
        cxx_noexcept
        cxx_nullptr
        cxx_override
        cxx_uniform_initialization
    PRIVATE
        cxx_auto_type
        cxx_generic_lambdas
        cxx_range_for
        cxx_strong_enums)