cmake_minimum_required(VERSION 3.16)

project(
  Experimental
  VERSION 0.1
  DESCRIPTION "Random ideas for different stuff"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
add_compile_options("$<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wall;-Werror;-Wno-attributes>")

add_subdirectory(meta_struct)
add_subdirectory(variant_sm)
add_subdirectory(any_callable)