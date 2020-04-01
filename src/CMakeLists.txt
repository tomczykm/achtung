target_sources(${PROJECT_NAME} PRIVATE
    Main.cpp
)

add_library(freeachtung_app STATIC)
target_link_libraries(freeachtung_app PUBLIC sfml-graphics tgui fmt toml11 sqlite3)
target_include_directories(freeachtung_app PUBLIC ${CMAKE_BINARY_DIR}/generated_include ../include)
target_compile_options(freeachtung_app PUBLIC -Wall -Wpedantic -Wextra -Werror -Wno-narrowing)
target_compile_features(freeachtung_app PUBLIC cxx_std_17)

add_subdirectory(app)
add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(menu)
add_subdirectory(sql)
