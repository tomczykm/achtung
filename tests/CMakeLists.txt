add_executable(collision_tests CollisionDetectionTests.cpp $<TARGET_OBJECTS:libCollisions>)
target_link_libraries(collision_tests gtest gtest_main ${SDL2_LIBRARIES})
add_test(CollisionDetectionTests collision_tests)
