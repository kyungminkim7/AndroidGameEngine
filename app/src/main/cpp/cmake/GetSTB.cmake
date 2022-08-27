include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
)

FetchContent_MakeAvailable(stb)

add_library(stb INTERFACE)

target_include_directories(stb INTERFACE
    "$<BUILD_INTERFACE:${stb_SOURCE_DIR}>"
)

target_compile_definitions(stb INTERFACE STB_IMAGE_IMPLEMENATION)
