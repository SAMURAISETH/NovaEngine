
cmake_minimum_required(VERSION 3.20)

project(NovaEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.5 REQUIRED)
find_package(Lua REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(novaengine
    engine/src/main.cpp
    engine/src/engine.cpp
    engine/src/window.cpp
    engine/src/renderer.cpp
)

target_include_directories(novaengine PRIVATE
    engine/include
    ${LUA_INCLUDE_DIR}
)

target_link_libraries(novaengine PRIVATE
    glfw
    OpenGL::GL
    GLEW::GLEW
    ${LUA_LIBRARIES}
)
