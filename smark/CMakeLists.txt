cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

project(smark_project
  LANGUAGES CXX
  VERSION 0.0.1
)

find_package(OpenGL REQUIRED)
find_package(DearImGui REQUIRED)
find_package(glfw REQUIRED)

add_subdirectory(example)
add_subdirectory(src)
