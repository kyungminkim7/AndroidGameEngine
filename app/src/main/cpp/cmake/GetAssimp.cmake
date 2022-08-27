include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.2.4
)

OPTION(BUILD_SHARED_LIBS "Build package with shared libraries." OFF)
OPTION(ASSIMP_ANDROID_JNIIOSYSTEM "Android JNI IOSystem support is active" ON)
OPTION(ASSIMP_NO_EXPORT "Disable Assimp's export functionality." ON)
OPTION(ASSIMP_BUILD_ASSIMP_TOOLS "If the supplementary tools for Assimp are built in addition to the library." OFF)
OPTION(ASSIMP_BUILD_TESTS "If the test suite for Assimp is built in addition to the library." OFF)
OPTION(ASSIMP_INSTALL "Disable this if you want to use assimp as a submodule." OFF)
OPTION(ASSIMP_IGNORE_GIT_HASH "Don't call git to get the hash." ON)

FetchContent_MakeAvailable(assimp)
