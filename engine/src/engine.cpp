#include "engine.hpp"
#include "renderer.hpp"
#include <iostream>

namespace Engine {
    std::string EngineName = "Nova Engine";
    int print(lua_State* lua) {
        const char* message = luaL_checkstring(lua, 1);

        std::cout << "[" << EngineName << "]: " << message << '\n';

        return 0;
    }
    int Add(lua_State* lua){
        lua_Integer a = luaL_checkinteger(lua, 1);
        lua_Integer b = luaL_checkinteger(lua, 2);

        lua_pushinteger(lua, a + b);
        
        return 1;
    }
    
    int draw_rectangle(lua_State* lua) {
        float x = luaL_checknumber(lua, 1);
        float y = luaL_checknumber(lua, 2);
        float width = luaL_checknumber(lua, 3);
        float height = luaL_checknumber(lua, 4);
    
        Renderer::draw_rectangle(x, y, width, height);
    
        return 0;
    }

    int get_version(lua_State* lua) {
        lua_pushstring(lua, "v0.1.0");

        return 1;
    }

    void register_functions(lua_State* lua) {
        lua_newtable(lua);

        lua_pushcfunction(lua, print);
        lua_setfield(lua, -2, "print");

        lua_pushcfunction(lua, get_version);
        lua_setfield(lua, -2, "get_version");

        lua_pushcfunction(lua, Add);
        lua_setfield(lua, -2, "Add");

        lua_pushcfunction(lua, draw_rectangle);
        lua_setfield(lua, -2, "draw_rectangle");

        lua_setglobal(lua, "Engine");
    }

}