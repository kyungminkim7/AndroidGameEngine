project(asio)

find_package(Threads REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asio/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/asio/include>"
    "$<INSTALL_INTERFACE:include>"
)

target_link_libraries(${PROJECT_NAME} INTERFACE Threads::Threads)
