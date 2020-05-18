add_subdirectory(asio)

add_subdirectory(flatbuffers)

# libjpeg-turbo library
add_subdirectory(libjpeg-turbo)

target_include_directories(turbojpeg-static INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

target_compile_options(turbojpeg-static PRIVATE "-fvisibility=hidden")

# zlib library
add_subdirectory(zlib)

target_include_directories(zlibstatic INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

target_compile_options(zlibstatic PRIVATE "-fvisibility=hidden")
