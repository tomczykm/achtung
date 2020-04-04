cmake_minimum_required(VERSION 3.13)
project("freeachtung" VERSION 0.2 LANGUAGES CXX)

add_subdirectory(external)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
find_package(TGUI 0.8 REQUIRED)
if(MSVC)
	# for some reason, on ubuntu with sqlite3 installed from distro repositories,
	# find_package does not find findsqlite3.cmake, but still builds and links just fine
	find_package(sqlite3 CONFIG REQUIRED)
endif(MSVC)

if(MSVC)
	set(COMPILE_OPTIONS /W0)
else(MSVC)
	set(COMPILE_OPTIONS -Wall -Wpedantic -Wextra -Werror)
endif(MSVC)

if (NOT MSVC)
	enable_testing()
	add_executable(preview_test)
	add_subdirectory(test)
endif()

add_executable(${PROJECT_NAME})

add_subdirectory(src)
add_subdirectory(include/app)

target_link_libraries(${PROJECT_NAME} PUBLIC freeachtung_app)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/generated_include include)
target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILE_OPTIONS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
