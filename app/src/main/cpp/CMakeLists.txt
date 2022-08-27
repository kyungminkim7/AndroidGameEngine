cmake_minimum_required(VERSION 3.14)
project(game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add ARCore library dependency
add_library(arcore SHARED IMPORTED)
set_target_properties(arcore PROPERTIES IMPORTED_LOCATION
    "${ARCORE_LIBPATH}/${ANDROID_ABI}/libarcore_sdk_c.so"
    INTERFACE_INCLUDE_DIRECTORIES "${ARCORE_INCLUDE}"
)

add_subdirectory(src bin)