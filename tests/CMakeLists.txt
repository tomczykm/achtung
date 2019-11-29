add_executable(dummy_tests DummyTests.cpp)
set_target_properties(dummy_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries(dummy_tests gtest gtest_main ${SDL2_LIBRARIES})
add_test(DummyTests dummy_tests)
