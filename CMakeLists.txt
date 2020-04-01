cmake_minimum_required(VERSION 3.13)
project("freeachtung" VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(external)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(TGUI 0.8 REQUIRED)

enable_testing()

add_executable(${PROJECT_NAME})
add_executable(testcase_preview)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(include/app)

target_link_libraries(${PROJECT_NAME} PUBLIC freeachtung_app)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/generated_include include)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wpedantic -Wextra -Werror -Wno-narrowing)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
