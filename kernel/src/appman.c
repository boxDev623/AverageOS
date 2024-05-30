#include "appman.h"
#include "mm/liballoc.h"
#include "graphics/ui.h"

#include "graphics/nuklear.h"

#include <string.h>

appproc_t *head = NULL;
appproc_t *appproc_current;

static void appman_update_single_proc(appproc_t *proc)
{
    lua_getglobal(proc->L, "__averageos_update_event__");
    if (lua_isfunction(proc->L, -1))
        lua_pcall(proc->L, 0, 0, 0);
}

appproc_t *appman_new_proc(const char *name, const char *code)
{
    appproc_t *proc = (appproc_t *)kmalloc(sizeof(appproc_t));

    strcpy(proc->name, name);

    proc->L = luaL_newstate();
    proc->next = NULL;
    proc->prev = NULL;

    luaL_openlibs(proc->L);
    luaL_dostring(proc->L, code);

    lua_getglobal(proc->L, "__averageos_open_event__");
    if (lua_isfunction(proc->L, -1))
        lua_pcall(proc->L, 0, 0, 0);

    if (head == NULL)
        head = proc;
    else
    {
        appproc_t *current = head;
        while (current->next != NULL)
            current = current->next;
        current->next = proc;
        proc->prev = current;
    }
}

void appman_close_proc(appproc_t *proc)
{
    if (head == NULL || proc == NULL)
        return;

    lua_getglobal(proc->L, "__averageos_close_event__");
    if (lua_isfunction(proc->L, -1))
        lua_pcall(proc->L, 0, 0, 0);

    lua_close(proc->L);

    if (head == proc)
        head = proc->next;

    if (proc->next != NULL)
        proc->next->prev = proc->prev;

    if (proc->prev != NULL)
        proc->prev->next = proc->next;

    kfree(proc);
}

void appman_update(void)
{
    appproc_t *current = head;
    while (current != NULL)
    {
        appproc_current = current;
        appman_update_single_proc(current);
        current = current->next;
    }
}

void appman_close_all(void)
{
    appproc_t *current = head;
    while (current != NULL)
    {
        appproc_t *next = current->next;
        appman_close_proc(current);
        current = next;
    }
}

appproc_t *appman_get_current(void)
{
    return appproc_current;
}