# Code

- [x] event system
- [x] replace app polling for its window status by event
- [x] GLFW callbacks fire events
- [x] App listens for events fired by its window
- [ ] App has virtual methods to derivate it in demo
- [ ] Find a solution for libengine having undefined reference to extern hook
- [x] make a static logger class that just wraps an internal spdlog instance
      (or just forwards calls to spdlog::[info/error/...] ?)

- [ ] elegant way to manage OpenGL 'platform' part (API agnostic context
      abstraction is overkill please dont waste hours on it)
- [ ] libengine is private by default. Exported symbols defined in a specific header

## Event system rework

I already feel like this could be improved. I don't like it being global.
Alternative solution:

Leverage the fact that app is guaranteed to be global, so it should hold the
EventSystem. Instead of having listeners for all kinds of events, Application
should just have an `OnEvent(const Event& e)` method where the dispatching
will happen.

Every component/subsystem is created and owned by Application, so add to
each a callback in the constructor so they can raise their respective events
to their owning Application.

Instead of holding private `EventCallback<XXXEvent>` members, application
dispatches with an `std::bind` call to its `OnXXX` method.

Events should keep state in the `handled_` flag, and event handlers should
return a boolean. Thus, we know when to stop propagating an event.

## Event propagation/layers

Layer = section of app that renders it's own stuff and listens to specific
events. Need multiple layers to separate UI from 3D rendering and draw in the
correct order, and to dispatch events from 'closest' to 'deepest'.

# Misc

- [x] Find a name
- [x] CMake presets and launch.json
- [x] Fix spdlog not building with Clang 18
