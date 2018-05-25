set( LIB_GAME_SOURCE
  PlayerThing.cpp
  StateGame.cpp
  StateSandbox.cpp
  Thing.cpp
  TrailThing.cpp
)

add_library( libGame OBJECT ${LIB_GAME_SOURCE} )
add_library( libCollisions OBJECT Collisions.cpp )
