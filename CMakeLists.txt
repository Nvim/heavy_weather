cmake_minimum_required(VERSION 3.25)

project(heavy_weather LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMAKE_BUILD_TYPE is not defined!")
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "" FORCE)
endif()

add_library(cxx_setup INTERFACE)
target_compile_features(cxx_setup INTERFACE cxx_std_17)
target_include_directories(cxx_setup INTERFACE ${PROJECT_SOURCE_DIR})

message(STATUS "BUILD_TESTING: ${BUILD_TESTING}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Defining HW_DEBUG and ASSERT")
  target_compile_definitions(cxx_setup INTERFACE HW_DEBUG=1)
  target_compile_definitions(cxx_setup INTERFACE HW_ENABLE_ASSERTS=1)
  target_compile_options(cxx_setup INTERFACE -Wall -Wpedantic -Wextra)
else()
  target_compile_options(cxx_setup INTERFACE -Wall -Wpedantic -Wextra -Werror)
endif()

if(PLATFORM_LINUX STREQUAL "1")
  message(STATUS "Building for Linux platform")
  target_compile_definitions(cxx_setup INTERFACE PLATFORM_LINUX=1)
endif()

if(GL_VERSION_4 STREQUAL "1")
  message(STATUS "OpenGL 4+ support enabled")
  target_compile_definitions(cxx_setup INTERFACE GL_VERSION_4=1)
endif()

set(BUILD_GMOCK
    OFF
    CACHE BOOL "" FORCE)
include(CTest)

add_subdirectory(${PROJECT_NAME})
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
