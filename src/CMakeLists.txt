target_sources(${PROJECT_NAME} PRIVATE
    Main.cpp
)

add_library(achtung_app STATIC)
target_link_libraries(achtung_app PUBLIC sfml-graphics tgui fmt toml11 sqlite3)
target_include_directories(achtung_app PUBLIC ${CMAKE_BINARY_DIR}/generated_include ../include)
target_compile_options(achtung_app PUBLIC -Wall -Wpedantic -Wextra -Werror -Wno-narrowing)
target_compile_features(achtung_app PUBLIC cxx_std_17)

add_subdirectory(app)
add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(menu)
add_subdirectory(sql)
