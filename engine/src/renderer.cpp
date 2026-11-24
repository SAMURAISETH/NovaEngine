#include "renderer.hpp"
#include <GL/glew.h>
#include <iostream>

namespace Renderer {
    std::string EngineName = "Nova Engine";
    bool initialize() {
        const GLubyte* version = glGetString(GL_VERSION);

        if (!version) {
            std::cerr << "Failed to get OpenGL version\n";
            return false;
        }

        std::cout << "OpenGL Version: " << version << '\n';

        return true;
    }

    void render() {
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void shutdown() {
        // Renderer cleanup will go here later.
    }
}