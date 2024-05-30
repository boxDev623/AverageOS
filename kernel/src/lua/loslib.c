#define loslib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include "appman.h"
#include "devices/ps2/keyboard.h"
#include "devices/ps2/mouse.h"

#include <string.h>

static int los_get_current_proc(lua_State *L)
{
    lua_pushinteger(L, (lua_Integer)appman_get_current());
    return 1;
}

static int los_new_proc(lua_State *L)
{
    const char *name = luaL_checkstring(L, 1);
    const char *code = luaL_checkstring(L, 2);
    lua_pushinteger(L, (lua_Integer)appman_new_proc(name, code));
    return 1;
}

static int los_close_proc(lua_State *L)
{
    lua_Integer proc = luaL_checkinteger(L, 1);
    appman_close_proc((appproc_t*)proc);
    return 0;
}

static int los_is_key_pressed(lua_State *L)
{
    lua_Integer scancode = luaL_checkinteger(L, 1);
    lua_pushboolean(L, keyboard_get_scancode(scancode));
    return 1;
}

static int los_is_mouse_button_pressed(lua_State *L)
{
    lua_Integer button = luaL_checkinteger(L, 1);
    bool result = false;
    switch (button)
    {
    case 1: if (mouse_get_status().left_button) result = true; break;
    case 2: if (mouse_get_status().right_button) result = true; break;
    case 3: if (mouse_get_status().middle_button) result = true; break;
    }
    lua_pushboolean(L, result);
    return 1;
}

static const luaL_Reg oslib[] = {
  {"get_current_proc", los_get_current_proc},
  {"new_proc", los_new_proc},
  {"close_proc", los_close_proc},
  {"is_key_pressed", los_is_key_pressed},
  {"is_mouse_button_pressed", los_is_mouse_button_pressed},
  {NULL, NULL}
};

LUALIB_API int luaopen_os(lua_State *L)
{
    luaL_register(L, LUA_OSLIBNAME, oslib);
    return 1;
}