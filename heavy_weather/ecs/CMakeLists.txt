if(BUILD_TESTING)
  add_executable(sparse_test sparse_test.cpp)
  target_link_libraries(sparse_test PRIVATE GTest::gtest_main)
  add_executable(ecs_test ecs_test.cpp)
  target_link_libraries(ecs_test PRIVATE GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(sparse_test 
                       WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  gtest_discover_tests(ecs_test 
                       WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()
