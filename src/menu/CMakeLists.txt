set (MENU_SOURCE
    StateMenu.cpp
)

add_library(menu OBJECT ${MENU_SOURCE})
target_link_libraries(menu PRIVATE fmt::fmt-header-only)