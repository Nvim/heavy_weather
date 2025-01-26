cmake_minimum_required(VERSION 3.12)
project(glad)

add_library(glad)

if(GL_VERSION_4 STREQUAL "1")
  target_sources(glad PRIVATE src/glad4_6.c)
  target_include_directories(glad PUBLIC include/4_6)
else()
  target_sources(glad PRIVATE src/glad3_3.c)
  target_include_directories(glad PUBLIC include/3_3)
endif()
