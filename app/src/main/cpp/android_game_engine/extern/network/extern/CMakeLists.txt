add_subdirectory(asio)

add_subdirectory(flatbuffers)

add_subdirectory(libjpeg-turbo)
target_include_directories(turbojpeg-static INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)
