add_subdirectory(game)
add_subdirectory(menu)

set(APP_CORE_SOURCE
    Application.cpp
    Utils.cpp
    Main.cpp
    SettingsManager.cpp
)

add_library(app_core OBJECT ${APP_CORE_SOURCE})
target_link_libraries(app_core PRIVATE fmt::fmt-header-only)
