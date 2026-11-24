#include "engine.hpp"

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
        lua_Integer b = luaL_checkinteger(lua, 1);

        lua_pushinteger(lua, a + b);
        
        return 1;
    }

    int get_version(lua_State* lua) {
        lua_pushstring(lua, "0.1.0");

        return 1;
    }

    void register_functions(lua_State* lua) {
        lua_newtable(lua);

        lua_pushcfunction(lua, print);
        lua_setfield(lua, -2, "print");

        lua_pushcfunction(lua, get_version);
        lua_setfield(lua, -2, "get_version");

        lua_setglobal(lua, "Engine");
    }

}