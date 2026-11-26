# Nova Engine — Rectangle Rendering Documentation

**Version:** 0.1.0  
**System:** 2D Rectangle Renderer  
**Language:** C++20, Lua, GLSL  
**Graphics API:** OpenGL 3.3 Core

---

## 1. Overview

Nova Engine uses OpenGL to render 2D rectangles. Lua scripts can request rectangles through the engine API, while the C++ renderer handles storing and drawing them.

The rendering system is split into two main responsibilities:

- **Rectangle submission:** Store rectangle positions and dimensions.
- **Rendering:** Convert stored rectangles into OpenGL vertices and draw them every frame.

This separation ensures that rectangles remain visible instead of disappearing when the screen is cleared.

## 2. Project files

The rectangle renderer uses these files:

```text
engine/
├── assets/
│   └── shaders/
│       ├── rectangle.vert
│       └── rectangle.frag
├── include/
│   └── renderer.hpp
└── src/
    ├── engine.cpp
    ├── renderer.cpp
    └── window.cpp

examples/
└── game.lua
```

### File responsibilities

| File | Responsibility |
|---|---|
| `renderer.hpp` | Declares renderer functions |
| `renderer.cpp` | Handles rectangle storage and OpenGL rendering |
| `engine.cpp` | Exposes C++ functions to Lua |
| `window.cpp` | Creates the window and runs the render loop |
| `rectangle.vert` | Vertex shader |
| `rectangle.frag` | Fragment shader |
| `game.lua` | Contains game-side Lua instructions |

## 3. Rectangle data structure

The renderer defines a structure to represent a rectangle:

```cpp
struct Rectangle {
    float x;
    float y;
    float width;
    float height;
};
```

Each rectangle contains four properties:

- `x`: Horizontal position.
- `y`: Vertical position.
- `width`: Rectangle width.
- `height`: Rectangle height.

The renderer stores rectangles in a vector:

```cpp
std::vector<Rectangle> rectangles;
```

A vector is a dynamic array, allowing the engine to store multiple rectangles.

## 4. Submitting rectangles

The renderer exposes this function:

```cpp
void draw_rectangle(float x, float y, float width, float height) {
    rectangles.push_back({x, y, width, height});
}
```

Rather than immediately drawing with OpenGL, this function stores the rectangle.

`push_back()` adds a new rectangle to the vector.

This allows the renderer to redraw every stored rectangle during subsequent frames.

## 5. Lua integration

The engine exposes the rectangle function to Lua through a wrapper:

```cpp
int draw_rectangle(lua_State* lua) {
    float x = luaL_checknumber(lua, 1);
    float y = luaL_checknumber(lua, 2);
    float width = luaL_checknumber(lua, 3);
    float height = luaL_checknumber(lua, 4);

    Renderer::draw_rectangle(x, y, width, height);

    return 0;
}
```

`luaL_checknumber()` retrieves and validates numeric arguments passed from Lua.

The function is registered in `register_functions()`:

```cpp
lua_pushcfunction(lua, draw_rectangle);
lua_setfield(lua, -2, "draw_rectangle");
```

This makes the function accessible through the global Lua table `Engine`.

### Lua example

```lua
Engine.draw_rectangle(300, 200, 200, 100)
```

This creates a rectangle with:

| Property | Value |
|---|---:|
| X position | 300 |
| Y position | 200 |
| Width | 200 |
| Height | 100 |

## 6. OpenGL initialization

Before rendering, the engine initializes OpenGL resources.

The renderer creates:

- A shader program.
- A Vertex Array Object (VAO).
- A Vertex Buffer Object (VBO).

The VBO is allocated to hold six vertices, with two floating-point coordinates per vertex:

```cpp
glBufferData(
    GL_ARRAY_BUFFER,
    12 * sizeof(float),
    nullptr,
    GL_DYNAMIC_DRAW
);
```

The VAO stores the vertex attribute configuration.

The renderer configures attribute location `0` to read two floating-point values per vertex:

```cpp
glVertexAttribPointer(
    0,
    2,
    GL_FLOAT,
    GL_FALSE,
    2 * sizeof(float),
    (void*)0
);

glEnableVertexAttribArray(0);
```

## 7. Coordinate conversion

OpenGL uses normalized device coordinates rather than pixel coordinates.

The renderer converts pixel positions into OpenGL coordinates.

Horizontal conversion:

```cpp
float left = (2.0f * rectangle.x / screenWidth) - 1.0f;

float right = (2.0f * (rectangle.x + rectangle.width) / screenWidth) - 1.0f;
```

Vertical conversion:

```cpp
float top = 1.0f - (2.0f * rectangle.y / screenHeight);

float bottom = 1.0f - (2.0f * (rectangle.y + rectangle.height) / screenHeight);
```

For an 800 × 600 window:

- Left edge: `-1.0`
- Right edge: `1.0`
- Top edge: `1.0`
- Bottom edge: `-1.0`

This allows Lua scripts to use familiar pixel-based coordinates.

## 8. Rectangle vertices

A rectangle is rendered using two triangles.

The renderer creates six vertices:

```cpp
float vertices[] = {
    left, bottom,
    right, bottom,
    right, top,

    left, bottom,
    right, top,
    left, top
};
```

Each vertex contains an X and Y coordinate.

Together, the six vertices form a rectangle.

## 9. Rendering each frame

The `render()` function clears the screen and redraws every stored rectangle:

```cpp
void render() {
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const Rectangle& rectangle : rectangles) {
        // Convert coordinates.
        // Create vertices.
        // Upload vertices to the VBO.
        // Draw the rectangle.
    }
}
```

The rendering loop performs these operations:

1. Clear the previous frame.
2. Iterate through the rectangle vector.
3. Convert rectangle coordinates.
4. Upload vertex data to the GPU.
5. Activate the shader.
6. Draw six vertices.

The window calls `Renderer::render()` every frame through `Window::update()`.

## 10. Shaders

### Vertex shader

`rectangle.vert`:

```glsl
#version 330 core

layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}
```

The vertex shader receives each vertex position and converts it into a position used by OpenGL.

### Fragment shader

`rectangle.frag`:

```glsl
#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
```

The fragment shader determines the rectangle's colour.

The current colour is red, represented by RGBA values:

- Red: `1.0`
- Green: `0.0`
- Blue: `0.0`
- Alpha: `1.0`

## 11. Resource cleanup

When the engine shuts down, it releases its OpenGL resources:

```cpp
void shutdown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    VAO = 0;
    VBO = 0;
    shaderProgram = 0;
}
```

This prevents the renderer from leaving its GPU resources allocated after shutdown.

The window must still have an active OpenGL context when this cleanup happens.

## 12. Current limitations

The current rectangle renderer is an early implementation.

Known limitations include:

- All rectangles use the same shader and colour.
- Screen dimensions are currently fixed at 800 × 600.
- Rectangles are stored persistently in a vector.
- There is no rectangle removal or clearing API yet.
- There is no texture or sprite support.
- Each rectangle is submitted through an individual draw call.

## 13. Future improvements

Possible future additions include:

1. Rectangle colour support.
2. Rectangle rotation.
3. Rectangle removal and clearing.
4. Dynamic window resizing.
5. Texture rendering.
6. Sprite rendering.
7. Batching multiple rectangles into fewer draw calls.
8. Lua update and draw callbacks.

---

**Nova Engine — Rectangle Rendering System**

This implementation establishes the foundation for Lua-controlled 2D rendering using C++ and OpenGL.