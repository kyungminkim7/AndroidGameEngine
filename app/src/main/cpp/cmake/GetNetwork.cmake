include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(network
    GIT_REPOSITORY https://github.com/kyungminkim7/network.git
)

FetchContent_MakeAvailable(network)
