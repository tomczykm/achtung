add_executable(dummy_tests DummyTests.cpp)
target_link_libraries(dummy_tests gtest gtest_main ${SDL2_LIBRARIES})
add_test(DummyTests dummy_tests)
