#pragma once

namespace Renderer {
    bool initialize();
    void render();
    void shutdown();
    void draw_rectangle(float x, float y, float width, float height);
}