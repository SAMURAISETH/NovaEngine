#include <iostream>
#include <lua.hpp>
#include "engine.hpp"
#include "window.hpp"
#include <string>

int main() {
    lua_State* lua = luaL_newstate();
    Engine::register_functions(lua);
    luaL_openlibs(lua);

    if (luaL_dofile(lua, "examples/game.lua") != LUA_OK) {
        std::cerr << "Lua Error: "
                  << lua_tostring(lua, -1)
                  << '\n';

        lua_close(lua);
        return 1;
    }

    lua_close(lua);

    std::string EngineName = "Nova Engine";
    
    if (!Window::create()) {
        std::cerr << "Failed to create window!\n";
        return 1;
    }
    

    
    while (Window::is_open()) {
        Window::update();
    }
    Window::close();

    

    return 0;
}