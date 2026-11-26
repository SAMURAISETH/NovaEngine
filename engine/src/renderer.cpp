#include "renderer.hpp"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace Renderer {
    unsigned int shaderProgram = 0;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    float screenWidth = 800.0f;
    float screenHeight = 600.0f;

    struct Rectangle {
        float x;
        float y;
        float width;
        float height;
    };
    std::vector<Rectangle> rectangles;

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
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

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

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

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

    void draw_rectangle(float x, float y, float width, float height) {
        rectangles.push_back({x, y, width, height});
    }

    void render() {
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for (const Rectangle& rectangle : rectangles) {
            float left = (2.0f * rectangle.x / screenWidth) - 1.0f;
            float right = (2.0f * (rectangle.x + rectangle.width) / screenWidth) - 1.0f;

            float top = 1.0f - (2.0f * rectangle.y / screenHeight);
            float bottom = 1.0f - (2.0f * (rectangle.y + rectangle.height) / screenHeight);

            float vertices[] = {
                left, bottom,
                right, bottom,
                right, top,
            
                left, bottom,
                right, top,
                left, top
            };
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                sizeof(vertices),
                vertices
            );
            glUseProgram(shaderProgram);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
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