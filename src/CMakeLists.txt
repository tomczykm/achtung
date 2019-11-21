add_subdirectory(game/)

set(APP_CORE_SOURCE
    Application.cpp
    Texture.cpp
    Global.cpp
    Log.cpp
    Main.cpp
    SettingsHandler.cpp
)

add_library(app_core OBJECT ${APP_CORE_SOURCE})
