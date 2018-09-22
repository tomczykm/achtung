add_subdirectory(Game/)

set(CORE_SOURCE
    Application.cpp
    Texture.cpp
    Global.cpp
    Logger.cpp
    Main.cpp
    SettingsHandler.cpp
    StateSettings.cpp
)

add_library(libCore OBJECT ${CORE_SOURCE})
add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:libCore> $<TARGET_OBJECTS:libGame> $<TARGET_OBJECTS:libCollisions>)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
