add_executable(engine_test EngineTests.cpp)
target_link_libraries(engine_test PUBLIC achtung_app gtest gmock gtest_main)
target_include_directories(engine_test PUBLIC mock)
add_test(engine_test engine_test)
