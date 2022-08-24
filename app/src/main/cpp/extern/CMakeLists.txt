# assimp library for loading 3D models
add_subdirectory(assimp)

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