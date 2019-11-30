add_subdirectory(game/)

set(APP_CORE_SOURCE
    Application.cpp
    Texture.cpp
    Global.cpp
    Main.cpp
    SettingsHandler.cpp
)

add_library(app_core OBJECT ${APP_CORE_SOURCE})
target_link_libraries(app_core PRIVATE fmt::fmt-header-only)
