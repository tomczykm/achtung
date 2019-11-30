cmake_minimum_required(VERSION 3.0)
project("freeachtung")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2IMAGE_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR}/generated_include)
include_directories(include)

enable_testing()
option(BUILD_GMOCK "" OFF)
option(BUILD_GTEST "" ON)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(external)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:app_core> $<TARGET_OBJECTS:game>)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
