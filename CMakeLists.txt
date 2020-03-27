cmake_minimum_required(VERSION 3.13)
project("freeachtung" VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(external)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(TGUI 0.8 REQUIRED)
if(MSVC)
	# for some reason, on ubuntu with sqlite3 installed from distro repositories,
	# find_package does not find findsqlite3.cmake, but still builds and links just fine
	find_package(sqlite3 CONFIG REQUIRED)
endif(MSVC)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics tgui fmt toml11 sqlite3)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/generated_include include)
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Wextra -Werror -Wno-narrowing)
endif(MSVC)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_subdirectory(src)
add_subdirectory(include/app)
