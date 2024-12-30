# Code

- [x] event system

- [x] replace app polling for its window status by event
- [x] GLFW callbacks fire events
- [x] App listens for events fired by its window
- [ ] App has virtual methods to derivate it in demo
- [ ] Find a solution for libengine having undefined reference to extern hook
- [ ] make a static logger class that just wraps an internal spdlog instance
      (or just forwards calls to spdlog::[info/error/...] ?)

- [ ] elegant way to manage OpenGL 'platform' part (API agnostic context
      abstraction is overkill please dont waste hours on it)

# Misc

- [ ] Find a name
- [ ] CMake presets and launch.json
- [ ] Fix spdlog not building with Clang 18
