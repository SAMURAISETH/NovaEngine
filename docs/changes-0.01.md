# Nova Engine — Changelog

## Version 0.1.0 — Initial Engine Foundation

### Project setup

- Created the initial C++20 game engine project.
- Set up CMake as the build system.
- Added the initial engine source and header structure.
- Linked GLFW, OpenGL, GLEW and Lua.

### Window system

- Implemented GLFW window creation.
- Added window update, close and open-state functions.
- Added GLFW error reporting.
- Configured GLFW to use the X11 platform backend.

**Reason for X11 backend:** GLEW initially failed to initialise with the native Wayland backend, reporting `No GLX display`. Using GLFW's X11 backend through XWayland resolved this on the development system.

### Renderer

- Created a separate renderer subsystem.
- Implemented GLEW initialisation.
- Added OpenGL version detection.
- Implemented frame clearing with `glClearColor` and `glClear`.
- Added buffer swapping through GLFW.

### Lua integration

- Added Lua headers and libraries to the build.
- Created the initial Lua engine integration.
- Added Lua script loading using `luaL_dofile`.
- Added initial engine function registration.

### Verified environment

- Operating system: Arch Linux
- Window environment: Hyprland
- Graphics API: OpenGL 4.6
- GPU: NVIDIA RTX 3060
- NVIDIA driver: 580.196.07
- Windowing library: GLFW
- OpenGL loader: GLEW
- Build system: CMake
- Language standard: C++20

### Current limitations

- Lua execution is not yet integrated into the active window/game loop.
- No shader management system yet.
- No vertex or index buffer abstraction yet.
- No texture management yet.
- Vulkan support is not implemented.

---

## Development rules

- Record completed changes after each development stage.
- Document architectural decisions and the reasons behind them.
- Keep implemented features separate from planned features.
- Update documentation whenever project structure or behaviour changes.
