cmake_minimum_required( VERSION 3.0 )
project( "achtung" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )


set( SOURCE

src/Main.cpp
src/Global.cpp
src/Logger.cpp
src/SettingsHandler.cpp
src/BaseGameState.cpp
src/StateSettings.cpp
src/StateTest.cpp

)

add_executable( ${PROJECT_NAME} ${SOURCE} )

INCLUDE( FindPkgConfig )
PKG_SEARCH_MODULE( SDL2 REQUIRED sdl2 )
INCLUDE_DIRECTORIES( ${SDL2_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${SDL2_LIBRARIES} )
