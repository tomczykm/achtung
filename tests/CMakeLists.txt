add_executable(tests DummyTests.cpp)
set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries(tests gtest gtest_main)
add_test(Tests tests)
