include(FetchContent)

FetchContent_Declare(GLM
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)

FetchContent_MakeAvailable(GLM)