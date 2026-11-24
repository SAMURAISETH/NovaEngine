#include "renderer.hpp"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace Renderer {
    unsigned int shaderProgram = 0;
    unsigned int VAO = 0;
    unsigned int VBO = 0;

    unsigned int compile_shader(unsigned int type, const std::string& source) {
        unsigned int shader = glCreateShader(type);
    
        const char* sourceCode = source.c_str();
        glShaderSource(shader, 1, &sourceCode, nullptr);
        glCompileShader(shader);
    
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    
            std::cerr << "Shader compilation failed:\n"
                      << infoLog << '\n';
    
            glDeleteShader(shader);
            return 0;
        }
    
        return shader;
    }
    std::string load_shader(const std::string& path) {
        std::ifstream file(path);
    
        if (!file.is_open()) {
            std::cerr << "Failed to open shader: " << path << '\n';
            return "";
        }
    
        std::stringstream buffer;
        buffer << file.rdbuf();
    
        return buffer.str();
    }
    std::string EngineName = "Nova Engine";
    bool initialize() {
        const GLubyte* version = glGetString(GL_VERSION);
        

        if (!version) {
            std::cerr << "Failed to get OpenGL version\n";
            return false;
        }

        std::cout << "OpenGL Version: " << version << '\n';

            std::string vertexSource = load_shader("engine/assets/shaders/rectangle.vert");
    std::string fragmentSource = load_shader("engine/assets/shaders/rectangle.frag");

    if (vertexSource.empty() || fragmentSource.empty()) {
        return false;
    }

    unsigned int vertexShader = compile_shader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentSource);

    if (!vertexShader || !fragmentShader) {
        return false;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

        std::cerr << "Shader linking failed:\n"
                << infoLog << '\n';

        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        // First triangle
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
    
        // Second triangle
        -0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

        return true;
    }

    void render() {
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        glUseProgram(shaderProgram);
    
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    
        glBindVertexArray(0);
    }

    void shutdown() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    
        VAO = 0;
        VBO = 0;
        shaderProgram = 0;
    }
}