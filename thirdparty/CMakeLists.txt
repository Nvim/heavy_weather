include(spdlog.cmake)
include(glad.cmake)
include(imgui.cmake)
include(googletest.cmake)
include(stb.cmake)
include(tinygltf.cmake)
include(json.cmake)

set(BUILD_GLFW
    OFF
    CACHE BOOL "Build GLFW with project or use system install")
if(BUILD_GLFW)
  include(glfw.cmake)
else()
  find_package(glfw3 3.4 REQUIRED)
endif()
