set(GAME_SOURCE
    BorderThing.cpp
    Player.cpp
    PowerupThing.cpp
    StateGame.cpp
    TrailThing.cpp
)

add_library(game OBJECT ${GAME_SOURCE})
target_link_libraries(game PRIVATE fmt::fmt-header-only)
