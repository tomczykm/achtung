cmake_minimum_required( VERSION 3.0 )

set( LIB_GAME_SOURCE
  PlayerThing.cpp
  StateGame.cpp
  StateSandbox.cpp
  Thing.cpp
  TrailThing.cpp
)

add_library( libGame OBJECT ${LIB_GAME_SOURCE} )
