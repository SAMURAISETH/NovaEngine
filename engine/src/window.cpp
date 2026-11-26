
#include "window.hpp"
#include "renderer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace Window {
    char EngineName[] = "Nova Engine";
    GLFWwindow* window = nullptr;

    bool create() {
        glfwSetErrorCallback([](int error, const char* description) {
            std::cerr << "GLFW error " << error
                      << ": " << description << '\n';
        });

        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

        if (!glfwInit()) {
            std::cerr << "GLFW initialization failed\n";
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        

        window = glfwCreateWindow(
            800,
            600,
            EngineName,
            nullptr,
            nullptr
        );

        if (!window) {
            std::cerr << "Window creation failed\n";
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        glewExperimental = GL_TRUE;
        GLenum result = glewInit();

        if (result != GLEW_OK) {
            std::cerr << "GLEW initialization failed: "
                      << glewGetErrorString(result) << '\n';
        
            close();
            return false;
        }
        
        glfwSwapInterval(1);

        if (!Renderer::initialize()) {
            close();
            return false;
        }

        std::cout << EngineName << " OpenGL window created!\n";

        return true;
    }

    void update() {
        glfwPollEvents();
    
        Renderer::render();
    
        glfwSwapBuffers(window);
    }

    bool is_open() {
        return window && !glfwWindowShouldClose(window);
    }

    void close() {
        if (window) {
            Renderer::shutdown();
            glfwDestroyWindow(window);
            window = nullptr;
        }
    
        glfwTerminate();
    }
}
