# bullet library for physics and collision detection
OPTION(USE_GRAPHICAL_BENCHMARK "Use Graphical Benchmark" OFF)
OPTION(BUILD_CPU_DEMOS "Build original Bullet CPU examples" OFF)
OPTION(BUILD_ENET "Set when you want to build apps with enet UDP networking support" OFF)
OPTION(BUILD_CLSOCKET "Set when you want to build apps with enet TCP networking support" OFF)
OPTION(BUILD_EGL "Build OpenGL/EGL" OFF)
OPTION(BUILD_OPENGL3_DEMOS "Set when you want to build Bullet 3 OpenGL3+ demos" OFF)
OPTION(BUILD_BULLET2_DEMOS "Set when you want to build the Bullet 2 demos" OFF)
OPTION(INSTALL_LIBS "Set when you want to install libraries" OFF)
OPTION(BUILD_UNIT_TESTS "Build Unit Tests"	OFF)
OPTION(INSTALL_CMAKE_FILES "Install generated CMake files" OFF)
add_subdirectory(bullet3)

target_include_directories(Bullet2FileLoader INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>)

target_include_directories(Bullet3Collision INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>)

target_include_directories(Bullet3Common INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>)

target_include_directories(BulletCollision INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>)

target_include_directories(BulletDynamics INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>)

target_include_directories(BulletInverseDynamics INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>)

target_include_directories(BulletSoftBody INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>)

target_include_directories(LinearMath INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bullet3/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/bullet3/src>)

# glm library for linear algebra
add_subdirectory(glm)

target_include_directories(glm_static INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glm>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/glm>)

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
        lib3ds/src/lib3ds.rc)

target_include_directories(lib3ds PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib3ds/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/lib3ds/src>)

# STB library for loading image files
add_library(stb INTERFACE)

target_include_directories(stb INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/stb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/stb>)