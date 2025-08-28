# Heavy Weather

Work in progress graphics engine.

# Features

- Event system for communication between components
- Multiple client applications can be created independently
- Scene editor program 
- Entity Component System
- GUI for controlling the scene and viewing loaded assets
- Forward rendering pipeline with Phong lighting
- Dynamic light sources
- Hot-reloadable custom shaders, materials and textures
- Multiple render targets, draw to texture

# Technical details

## Graphics API support

Only OpenGL backend is implemented for now, but the rendering API is designed
to be independent of any specific backend. 

OpenGL is split into two backends as there are big difference between regular
OpenGL 3.3 and modern OpenGL versions. Ideally, I'd only use 4.6, but it isn't
supported on all devices or by WebGL.

## Dependencies

The project uses the following third-party libraries:

- [GLFW](https://github.com/glfw/glfw) for windowing and input
- [GLAD](https://github.com/premake-libs/glad) for loading OpenGL headers
- [ImGUI](https://github.com/ocornut/imgui) for the graphical user interface
- [spdlog](https://github.com/gabime/spdlog) for logging
- [stb_image](https://github.com/nothings/stb) for loading textures
- [TinyGLTF](https://github.com/syoyo/tinygltf) for loading GLTF models and resources
- [nlohmann/json](https://github.com/nlohmann/json) for parsing data files
- [GoogleTest](https://github.com/google/googletest) testing framework

# Building

Building the engine has only been tested on Linux x86, with Clang 20 and the
C++17 standard. Building with GCC or other Clang versions should be possible
on Linux, but other platforms aren't supported yet.

All dependencies are included as submodules and are built from source. The only
exception is GLFW, which is expected to be installed system-wide. If you prefer
building it from source with the other dependencies, define `BUILD_GLFW`.

## Configurations

There are two CMake presets for build configurations: debug and release.
Use one of the following commands to launch the build:

```bash
cmake --build --preset debug
cmake --build --preset debug-clean
cmake --build --preset release
cmake --build --preset release-clean
```

### Defines

- `CMAKE_BUILD_TYPE`: Debug mode enables assertions, higher logging levels and debug symbols. It also disables the `-Werror` flag.
- `PLATFORM_LINUX`: Linux is the only OS supported for now,  leave enabled
- `GL_VERSION_4`: Toggle to use OpenGL 4.6 backend instead of 3.3

## Nix

A Nix devshell with all the necessary dependencies is available. Run `nix
develop .` then build the project with CMake.
