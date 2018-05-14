cmake_minimum_required( VERSION 3.0 )

include_directories( inc/ )
include_directories( game/inc/ )

add_subdirectory( inc/ )
add_subdirectory( game/ )

set( MAIN_SOURCE
  BaseGameState.cpp
  Texture.cpp
  Global.cpp
  Logger.cpp
  Main.cpp
  SettingsHandler.cpp
  StateSettings.cpp
)

add_executable( ${PROJECT_NAME} ${MAIN_SOURCE} $<TARGET_OBJECTS:libGame> )
target_link_libraries( ${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} )
