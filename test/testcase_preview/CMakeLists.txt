target_sources(testcase_preview PUBLIC
    Main.cpp
    StateViewer.cpp
)

target_link_libraries(testcase_preview PUBLIC freeachtung_app testable)
