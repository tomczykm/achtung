set(GAME_SOURCE
    BorderThing.cpp
    PlayerThing.cpp
    StateGame.cpp
    StateSandbox.cpp
    TrailThing.cpp
)

add_library(game OBJECT ${GAME_SOURCE})
target_link_libraries(game PRIVATE fmt::fmt-header-only)
