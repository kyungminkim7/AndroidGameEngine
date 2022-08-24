include(FetchContent)

FetchContent_Declare(bullet3
    GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
    GIT_TAG 3.24
)

OPTION(USE_GRAPHICAL_BENCHMARK "Use Graphical Benchmark" OFF)
OPTION(ENABLE_VHACD "Use VHACD in BulletRobotics and pybullet" OFF)
OPTION(BUILD_CPU_DEMOS "Build original Bullet CPU examples" OFF)
OPTION(USE_GLUT "Use Glut"	OFF)
OPTION(BUILD_ENET "Set when you want to build apps with enet UDP networking support" OFF)
OPTION(BUILD_CLSOCKET "Set when you want to build apps with enet TCP networking support" OFF)
OPTION(BUILD_OPENGL3_DEMOS "Set when you want to build the OpenGL3+ demos" OFF)
OPTION(BUILD_BULLET2_DEMOS "Set when you want to build the Bullet 2 demos" OFF)
OPTION(BUILD_EXTRAS "Set when you want to build the extras" OFF)
OPTION(INSTALL_LIBS "Set when you want to install libraries" OFF)
OPTION(BUILD_UNIT_TESTS "Build Unit Tests"	OFF)
OPTION(INSTALL_CMAKE_FILES "Install generated CMake files" OFF)

FetchContent_MakeAvailable(bullet3)

target_include_directories(Bullet2FileLoader INTERFACE
    $<BUILD_INTERFACE:${bullet3_SOURCE_DIR}/src>
)

target_include_directories(Bullet3Collision INTERFACE
    $<BUILD_INTERFACE:${bullet3_SOURCE_DIR}/src>
)

target_include_directories(Bullet3Common INTERFACE
    $<BUILD_INTERFACE:${bullet3_SOURCE_DIR}/src>
)

target_include_directories(BulletCollision INTERFACE
    $<BUILD_INTERFACE:${bullet3_SOURCE_DIR}/src>
)

target_include_directories(BulletDynamics INTERFACE
    $<BUILD_INTERFACE:${bullet3_SOURCE_DIR}/src>
)

target_include_directories(BulletInverseDynamics INTERFACE
    $<BUILD_INTERFACE:${bullet3_SOURCE_DIR}/src>
)

target_include_directories(BulletSoftBody INTERFACE
    $<BUILD_INTERFACE:${bullet3_SOURCE_DIR}/src>
)

target_include_directories(LinearMath INTERFACE
    $<BUILD_INTERFACE:${bullet3_SOURCE_DIR}/src>
)
