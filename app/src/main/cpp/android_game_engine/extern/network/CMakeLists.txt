cmake_minimum_required(VERSION 3.5...3.10)
project(network VERSION 1.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(package_name network)

add_subdirectory(extern)

# Create targets and set properties
add_library(${PROJECT_NAME}
    "src/Compression.cpp"
    "src/Node.cpp"
    "src/Rate.cpp"
    "src/TcpPublisher.cpp"
    "src/TcpSubscriber.cpp"
)

add_library(${package_name}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        asio::asio
        flatbuffers
    PRIVATE
        turbojpeg-static
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_auto_type
        cxx_generic_lambdas
        cxx_lambdas
        cxx_lambda_init_captures
        cxx_range_for
)
