cmake_minimum_required( VERSION 3.0 )

add_subdirectory( inc/ )
include_directories( inc/ )

set( SOURCE
  BaseGameState.cpp
  Texture.cpp
  Global.cpp
  Logger.cpp
  Main.cpp
  PlayerThing.cpp
  SettingsHandler.cpp
  StateSettings.cpp
  StateTest.cpp
  Thing.cpp
  TrailThing.cpp
)

add_executable( ${PROJECT_NAME} ${SOURCE} )
target_link_libraries( ${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} )
