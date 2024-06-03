#include "appman.h"
#include "mm/liballoc.h"
#include "graphics/ui.h"

#include "graphics/nuklear.h"

#include "apps/snake.h"
#include "apps/tetris.h"
#include "apps/text_editor.h"

#include <string.h>

static appproc_t appprocs[16];
static appproc_t *appproc_current;

static void appman_update_single_proc(appproc_t *proc)
{
    if (proc->closed)
    {
        lua_close(proc->L);
    }

    lua_getglobal(proc->L, "__averageos_update_event__");
    if (lua_isfunction(proc->L, -1))
        lua_pcall(proc->L, 0, 0, 0);

}

appproc_t *appman_new_proc(const char *name, const char *code)
{
    appproc_t *proc;
    for (uint32_t i = 0; i < 16; i++)
        if (appprocs[i].closed)
        {
            proc = &appprocs[i];
            break;
        }

    strcpy(proc->name, name);

    proc->L = luaL_newstate();
    proc->next = NULL;
    proc->prev = NULL;
    proc->closed = false;

    luaL_openlibs(proc->L);
    luaL_dostring(proc->L, code);

    /*lua_getglobal(proc->L, "__averageos_open_event__");
    if (lua_isfunction(proc->L, -1))
        lua_pcall(proc->L, 0, 0, 0);*/

    return proc;
}

void appman_close_proc(appproc_t *proc)
{
    if (!strcmp(proc->name, "SNAKE"))
        enable_app_snake = false;
    if (!strcmp(proc->name, "TETRIS"))
        enable_app_tetris = false;
    if (!strcmp(proc->name, "TEXT_EDITOR"))
        enable_app_text_editor = false;

    /*lua_getglobal(proc->L, "__averageos_close_event__");
    if (lua_isfunction(proc->L, -1))
        lua_pcall(proc->L, 0, 0, 0);*/

    proc->closed = true;
}

void appman_update(void)
{
    for (uint32_t i = 0; i < 16; i++)
        if (!appprocs[i].closed)
        {
            appproc_current = &appprocs[i];
            appman_update_single_proc(appproc_current);
        }
}

void appman_close_all(void)
{

}

void appman_initialize(void)
{
    for (uint32_t i = 0; i < 16; i++)
        appprocs[i].closed = true;
}

appproc_t *appman_get_current(void)
{
    return appproc_current;
}