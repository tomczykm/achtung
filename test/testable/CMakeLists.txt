add_library(testable STATIC)
target_sources(testable PUBLIC
    EngineTestable.cpp
    InputSequence.cpp)

target_link_libraries(testable PUBLIC freeachtung_app)
