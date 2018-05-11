cmake_minimum_required( VERSION 3.0 )

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

include_directories( inc/ )

add_executable( ${PROJECT_NAME} ${SOURCE} )
target_link_libraries( ${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} )
