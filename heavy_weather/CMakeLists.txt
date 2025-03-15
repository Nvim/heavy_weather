# huge single lib for now
add_library(engine)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Filter out test files:
list(FILTER SOURCES EXCLUDE REGEX ".*_test\\.cpp$")

add_subdirectory(ecs)

# Header-only event system:
add_library(events INTERFACE)
target_include_directories(events
                           INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/events")
target_link_libraries(engine PRIVATE events)

target_sources(engine PRIVATE ${SOURCES})
target_link_libraries(engine PUBLIC cxx_setup spdlog glfw glad imgui)
