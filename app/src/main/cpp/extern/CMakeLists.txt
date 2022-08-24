# assimp library for loading 3D models
add_subdirectory(assimp)

# bullet library for physics and collision detection
add_subdirectory(bullet3)

target_include_directories(Bullet2FileLoader INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>
)

target_include_directories(Bullet3Collision INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>
)

target_include_directories(Bullet3Common INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>
)

target_include_directories(BulletCollision INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>
)

target_include_directories(BulletDynamics INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>
)

target_include_directories(BulletInverseDynamics INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>
)

target_include_directories(BulletSoftBody INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>
)

target_include_directories(LinearMath INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>
)

add_library(bullet3::Bullet2FileLoader ALIAS Bullet2FileLoader)
add_library(bullet3::Bullet3Collision ALIAS Bullet3Collision)
add_library(bullet3::Bullet3Common ALIAS Bullet3Common)
add_library(bullet3::BulletCollision ALIAS BulletCollision)
add_library(bullet3::BulletDynamics ALIAS BulletDynamics)
add_library(bullet3::BulletInverseDynamics ALIAS BulletInverseDynamics)
add_library(bullet3::BulletSoftBody ALIAS BulletSoftBody)
add_library(bullet3::LinearMath ALIAS LinearMath)

# network library for networking
add_subdirectory(network)

# STB library for loading image files
add_library(stb INTERFACE)
add_library(stb::stb ALIAS stb)
target_compile_definitions(stb INTERFACE STB_IMAGE_IMPLEMENATION)

target_include_directories(stb INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/stb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/stb>
)