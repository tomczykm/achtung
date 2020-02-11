cmake_minimum_required(VERSION 3.0)
project("freeachtung")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(TGUI 0.8 REQUIRED)

include_directories(${CMAKE_BINARY_DIR}/generated_include)
include_directories(include)

enable_testing()
option(BUILD_GMOCK "" OFF)
option(BUILD_GTEST "" ON)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(external)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:app_core> $<TARGET_OBJECTS:game> $<TARGET_OBJECTS:menu>)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics tgui)
