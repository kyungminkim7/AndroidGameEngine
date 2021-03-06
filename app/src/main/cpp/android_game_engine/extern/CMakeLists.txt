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

# glm library for linear algebra
add_subdirectory(glm)

# lib3ds library for loading 3DS model files
add_library(lib3ds STATIC
        lib3ds/src/lib3ds.h
        lib3ds/src/lib3ds_impl.h
        lib3ds/src/lib3ds_atmosphere.c
        lib3ds/src/lib3ds_background.c
        lib3ds/src/lib3ds_camera.c
        lib3ds/src/lib3ds_chunk.c
        lib3ds/src/lib3ds_chunktable.c
        lib3ds/src/lib3ds_file.c
        lib3ds/src/lib3ds_io.c
        lib3ds/src/lib3ds_light.c
        lib3ds/src/lib3ds_material.c
        lib3ds/src/lib3ds_math.c
        lib3ds/src/lib3ds_matrix.c
        lib3ds/src/lib3ds_mesh.c
        lib3ds/src/lib3ds_node.c
        lib3ds/src/lib3ds_quat.c
        lib3ds/src/lib3ds_shadow.c
        lib3ds/src/lib3ds_track.c
        lib3ds/src/lib3ds_util.c
        lib3ds/src/lib3ds_vector.c
        lib3ds/src/lib3ds_viewport.c
        lib3ds/src/lib3ds.rc
)

add_library(lib3ds::lib3ds ALIAS lib3ds)

target_include_directories(lib3ds PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib3ds/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/lib3ds/src>
)

# network library for networking
add_subdirectory(network)

# STB library for loading image files
add_library(stb INTERFACE)
add_library(stb::stb ALIAS stb)

target_include_directories(stb INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/stb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/stb>
)