cmake_minimum_required(VERSION 3.0)
project("freeachtung")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2IMAGE_INCLUDE_DIRS})
include_directories(include)

enable_testing()
option(BUILD_GMOCK "" OFF)
option(BUILD_GTEST "" ON)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(external)
add_subdirectory(tests)
