cmake_minimum_required( VERSION 3.0 )
project( "achtung" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror" )
set( CMAKE_CXX_STANDARD 14)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY .. )

include( FindPkgConfig )
pkg_search_module( SDL2 REQUIRED sdl2 )
pkg_search_module( SDL2IMAGE REQUIRED SDL2_image>=2.0.0 )
include_directories( ${SDL2_INCLUDE_DIRS} )

add_subdirectory( src )
