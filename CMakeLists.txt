cmake_minimum_required( VERSION 3.0 )
project( "achtung" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )

set( SOURCE

src/BaseGameState.cpp
src/CTexture.cpp
src/Global.cpp
src/Logger.cpp
src/Main.cpp
src/PlayerThing.cpp
src/SettingsHandler.cpp
src/StateSettings.cpp
src/StateTest.cpp
src/Thing.cpp
src/TrailThing.cpp

)

add_executable( ${PROJECT_NAME} ${SOURCE} )

INCLUDE( FindPkgConfig )
PKG_SEARCH_MODULE( SDL2 REQUIRED sdl2 )
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
INCLUDE_DIRECTORIES( ${SDL2_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
