include(FetchContent)

FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.3.6
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Turn off glfw examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Turn off glfw tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Turn off glfw docs")

FetchContent_MakeAvailable(glfw)