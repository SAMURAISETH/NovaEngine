Engine.print("Hello from LuaForge!")

local version = Engine.get_version()

print("Engine version: " .. version .. "\n")
local result = Engine.Add(10, 20)
print(result)

Engine.draw_rectangle(300, 200, 200, 100)
