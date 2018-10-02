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
add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:app_core> $<TARGET_OBJECTS:game>)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${BOX2D_LIBRARIES})
