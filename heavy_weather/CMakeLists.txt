# huge single lib for now
add_library(engine)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Filter out test files:
list(FILTER SOURCES EXCLUDE REGEX ".*_test\\.cpp$")

target_sources(engine PRIVATE ${SOURCES})
target_link_libraries(engine PUBLIC cxx_setup spdlog glfw glad imgui stb::stb)

if(BUILD_TESTING)
  add_executable(sparse_test "${CMAKE_CURRENT_SOURCE_DIR}/ecs/sparse_test.cpp")
  target_link_libraries(sparse_test PRIVATE GTest::gtest_main engine)
  add_executable(ecs_test "${CMAKE_CURRENT_SOURCE_DIR}/ecs/ecs_test.cpp")
  target_link_libraries(ecs_test PRIVATE GTest::gtest_main engine)

  include(GoogleTest)
  gtest_discover_tests(sparse_test
                       WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  gtest_discover_tests(ecs_test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()
