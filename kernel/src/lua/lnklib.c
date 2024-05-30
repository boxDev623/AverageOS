#define lnklib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include "graphics/ui.h"
#include "graphics/nuklear.h"

#include <string.h>

static bool last_window_ended = true;

static int lnk_begin_window(lua_State *L)
{
    if (!last_window_ended)
        return 0;

    lua_pushboolean(L, nk_begin(nk_ctx, luaL_checkstring(L, 1),
    nk_rect(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5)),
    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE|NK_WINDOW_SCALABLE));
    
    last_window_ended = false;

    return 1;
}

static int lnk_end_window(lua_State *L)
{
    nk_end(nk_ctx);
    last_window_ended = true;
    return 0;
}

static int lnk_window_is_closed(lua_State *L)
{
    nk_bool closed = nk_window_is_closed(nk_ctx, luaL_checkstring(L, 1));
    lua_pushboolean(L, closed);
    return 1;
}

static int lnk_window_is_collapsed(lua_State *L)
{
    nk_bool collapsed = nk_window_is_collapsed(nk_ctx, luaL_checkstring(L, 1));
    lua_pushboolean(L, collapsed);
    return 1;
}

static int lnk_window_is_active(lua_State *L)
{
    nk_bool active = nk_window_is_active(nk_ctx, luaL_checkstring(L, 1));
    lua_pushboolean(L, active);
    return 1;
}

static int lnk_layout_row_static(lua_State *L)
{
    nk_layout_row_static(nk_ctx, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
    return 0;
}

static int lnk_layout_row_dynamic(lua_State *L)
{
    nk_layout_row_dynamic(nk_ctx, luaL_checknumber(L, 1), luaL_checknumber(L, 2));
    return 0;
}

static int lnk_label(lua_State *L)
{
    nk_label(nk_ctx, luaL_checkstring(L, 1), 0);
    return 0;
}

static int lnk_button(lua_State *L)
{
    nk_bool pressed = nk_button_label(nk_ctx, luaL_checkstring(L, 1));
    lua_pushboolean(L, pressed);
    return pressed;
}

static int lnk_checkbox(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    nk_bool active = lua_toboolean(L, 2);
    nk_checkbox_label(nk_ctx, label, &active);
    lua_pushboolean(L, active);

    return 1;
}

static int lnk_slider(lua_State *L)
{
    float min = luaL_checknumber(L, 1);
    float val = luaL_checknumber(L, 2);
    float max = luaL_checknumber(L, 3);
    float step = luaL_checknumber(L, 4);
    nk_slider_float(nk_ctx, min, &val, max, step);
    lua_pushnumber(L, val);
    return 1;
}

static int lnk_spacer(lua_State *L)
{
    nk_spacer(nk_ctx);
    return 0;
}

static int lnk_edit_string(lua_State *L)
{
    int32_t max = luaL_checknumber(L, 2) + 1;
    char buffer[max];
    strncpy(buffer, luaL_checkstring(L, 1), max);
    nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_FIELD | NK_EDIT_MULTILINE, buffer, max, 0);
    lua_pushstring(L, buffer);
    return 1;
}

static int lnk_window_get_width(lua_State *L)
{
    lua_pushnumber(L, nk_window_get_width(nk_ctx));
    return 1;
}

static int lnk_window_get_height(lua_State *L)
{
    lua_pushnumber(L, nk_window_get_height(nk_ctx));
    return 1;
}

static int lnk_window_get_size(lua_State *L)
{
    struct nk_vec2 size = nk_window_get_size(nk_ctx);
    lua_pushnumber(L, size.x);
    lua_pushnumber(L, size.y);
    return 2;
}

static int lnk_window_get_position(lua_State *L)
{
    struct nk_vec2 position = nk_window_get_position(nk_ctx);
    lua_pushnumber(L, position.x);
    lua_pushnumber(L, position.y);
    return 2;
}

static int lnk_widget_get_width(lua_State *L)
{
    lua_pushnumber(L, nk_widget_width(nk_ctx));
    return 1;
}

static int lnk_widget_get_height(lua_State *L)
{
    lua_pushnumber(L, nk_widget_height(nk_ctx));
    return 1;
}

static int lnk_widget_get_size(lua_State *L)
{
    struct nk_vec2 size = nk_widget_size(nk_ctx);
    lua_pushnumber(L, size.x);
    lua_pushnumber(L, size.y);
    return 2;
}

static int lnk_widget_get_position(lua_State *L)
{
    struct nk_vec2 position = nk_widget_position(nk_ctx);
    lua_pushnumber(L, position.x);
    lua_pushnumber(L, position.y);
    return 2;
}

static int lnk_get_canvas(lua_State *L)
{
    struct nk_command_buffer *cmd_buffer = nk_window_get_canvas(nk_ctx);
    lua_pushnumber(L, (lua_Integer)cmd_buffer);
    return 1;
}

static int lnk_fill_rect(lua_State *L)
{
    struct nk_command_buffer *cmd_buffer = (struct nk_command_buffer*)luaL_checkinteger(L, 1);

    luaL_checktype(L, 2, LUA_TTABLE);
    lua_rawgeti(L, 2, 1);
    int32_t x = luaL_checknumber(L, -1);
    lua_rawgeti(L, 2, 2);
    int32_t y = luaL_checknumber(L, -1);
    lua_rawgeti(L, 2, 3);
    int32_t w = luaL_checknumber(L, -1);
    lua_rawgeti(L, 2, 4);
    int32_t h = luaL_checknumber(L, -1);
    lua_pop(L, 4);

    float rounding = luaL_checknumber(L, 3);

    luaL_checktype(L, 4, LUA_TTABLE);
    lua_rawgeti(L, 4, 1);
    int32_t r = luaL_checknumber(L, -1);
    lua_rawgeti(L, 4, 2);
    int32_t g = luaL_checknumber(L, -1);
    lua_rawgeti(L, 4, 3);
    int32_t b = luaL_checknumber(L, -1);
    lua_rawgeti(L, 4, 4);
    int32_t a = luaL_checknumber(L, -1);
    lua_pop(L, 4);

    nk_fill_rect(cmd_buffer, nk_rect(x, y, w, h), rounding, nk_rgba(r, g, b, a));
    return 0;
}

static const luaL_Reg nklib[] = {
  {"begin_window", lnk_begin_window},
  {"end_window", lnk_end_window},
  {"window_is_closed", lnk_window_is_closed},
  {"window_is_collapsed", lnk_window_is_collapsed},
  {"window_is_active", lnk_window_is_active},
  {"layout_row_static", lnk_layout_row_static},
  {"layout_row_dynamic", lnk_layout_row_dynamic},
  {"label", lnk_label},
  {"button", lnk_button},
  {"checkbox", lnk_checkbox},
  {"slider", lnk_slider},
  {"spacer", lnk_spacer},
  {"edit_string", lnk_edit_string},
  {"window_get_width", lnk_window_get_width},
  {"window_get_height", lnk_window_get_height},
  {"window_get_size", lnk_window_get_size},
  {"window_get_position", lnk_window_get_position},
  {"widget_get_width", lnk_widget_get_width},
  {"widget_get_height", lnk_widget_get_height},
  {"widget_get_size", lnk_widget_get_size},
  {"widget_get_position", lnk_widget_get_position},
  {"get_canvas", lnk_get_canvas},
  {"fill_rect", lnk_fill_rect},
  {NULL, NULL}
};

LUALIB_API int luaopen_nuklear(lua_State *L)
{
    luaL_register(L, LUA_NKLIBNAME, nklib);
    return 1;
}