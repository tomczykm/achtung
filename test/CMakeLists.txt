add_subdirectory(preview_test)
add_subdirectory(testable)

add_executable(engine_test EngineTests.cpp EngineTestParams.cpp)
target_link_libraries(engine_test PUBLIC freeachtung_app testable gtest gmock gtest_main)
add_test(engine_test engine_test)

target_include_directories(preview_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(engine_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(testable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
