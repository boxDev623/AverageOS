#pragma once

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#include <stdbool.h>

typedef struct appproc
{
    char name[32];
    bool closed;

    lua_State *L;
    struct appproc *next;
    struct appproc *prev;
}
appproc_t;

appproc_t  *appman_new_proc(const char *name, const char *code);
void        appman_close_proc(appproc_t *proc);

void        appman_update(void);
void        appman_close_all(void);

void        appman_initialize(void);

appproc_t  *appman_get_current(void);