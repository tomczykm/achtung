add_subdirectory(testcase_preview)
add_subdirectory(testable)

add_executable(engine_test EngineTests.cpp)
target_link_libraries(engine_test PUBLIC freeachtung_app testable gtest gmock gtest_main)
add_test(engine_test engine_test)

target_include_directories(testcase_preview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(engine_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(testable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
