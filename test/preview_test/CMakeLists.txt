target_sources(preview_test PUBLIC
    Main.cpp
    StateViewer.cpp
    ../EngineTestParams.cpp
)

target_link_libraries(preview_test PUBLIC freeachtung_app testable)
