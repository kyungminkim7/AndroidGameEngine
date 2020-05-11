project(android_game_engine)

add_subdirectory(extern)
add_library(arcore SHARED IMPORTED)

set_target_properties(arcore PROPERTIES IMPORTED_LOCATION
        ${ARCORE_LIBPATH}/${ANDROID_ABI}/libarcore_sdk_c.so
        INTERFACE_INCLUDE_DIRECTORIES ${ARCORE_INCLUDE}
)

add_library(${PROJECT_NAME} STATIC
        src/ARCameraBackground.cpp
        src/ARPlane.cpp
        src/Asset.cpp
        src/Box.cpp
        src/Camera.cpp
        src/CameraChase.cpp
        src/CameraFPV.cpp
        src/Game.cpp
        src/GameAR.cpp
        src/GameObject.cpp
        src/GameTemplate.cpp
        src/Light.cpp
        src/LightDirectional.cpp
        src/Log.cpp
        src/ManagerAssets.cpp
        src/ManagerWindowing.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/ModelLoader.cpp
        src/ModelLoader3ds.cpp
        src/PID.cpp
        src/PhysicsCompoundShape.cpp
        src/PhysicsDebugDrawer.cpp
        src/PhysicsEngine.cpp
        src/PhysicsMotionState.cpp
        src/PhysicsRigidBody.cpp
        src/Quad.cpp
        src/Quadcopter.cpp
        src/ShaderProgram.cpp
        src/ShadowMap.cpp
        src/Skybox.cpp
        src/Texture2D.cpp
        src/UniformBuffer.cpp
        src/Utilities.cpp
        src/Vehicle.cpp
        src/VertexArray.cpp
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        EGL
        GLESv3
        android
        arcore
        bullet3::BulletDynamics
        bullet3::BulletCollision
        bullet3::LinearMath
        glm::glm
        lib3ds::lib3ds
        log
        network::network
    PRIVATE
        stb::stb)

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