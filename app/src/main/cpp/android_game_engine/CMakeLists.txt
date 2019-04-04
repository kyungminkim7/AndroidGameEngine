cmake_minimum_required(VERSION 3.0)

add_subdirectory(android_game_engine)
add_subdirectory(glm)

# lib3ds library
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

# STB library
add_library(stb INTERFACE)

target_include_directories(stb INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/stb>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/stb>)