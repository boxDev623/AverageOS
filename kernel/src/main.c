#include "init.h"
#include "appman.h"
#include "devices/lfb.h"
#include "devices/rtc.h"
#include "graphics/graphics.h"
#define NK_ASSERT
#include "graphics/ui.h"

#include <printk.h>
#include <string.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#include "graphics/nuklear.h"
#include "graphics/images/logo.h"

#include "apps/menu.h"
#include "apps/snake.h"
#include "apps/tetris.h"

kernel_memory_map_t kmap;
void __stack_chk_fail(void){}
void __attribute__ ((noreturn))
__stack_chk_fail_local (void){__stack_chk_fail ();}

void kmain(unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mboot_info = (multiboot_info_t*)addr;
    terminal_initialize();

    kernel_initialize(mboot_info);

    keyboard_initialize();
    mouse_initialize();

    graphics_initialize(1440, 900, 32);

    ui_initialize();

    date_time_t dt = { 0 };
    char dt1_str[8];
    char dt2_str[16];

	/*struct nk_color background = nk_rgb(31,31,31);
	
	nk_style_push_color(nk_ctx, &nk_ctx->style.window.header.normal.data.color, background); 
	nk_style_push_color(nk_ctx, &nk_ctx->style.window.header.hover.data.color, background); 
	nk_style_push_color(nk_ctx, &nk_ctx->style.window.header.active.data.color, background); 
	nk_style_push_color(nk_ctx, &nk_ctx->style.window.fixed_background.data.color, background);
	nk_style_push_color(nk_ctx, &nk_ctx->style.button.normal.data.color, nk_rgb(57,103,255));
	nk_style_push_color(nk_ctx, &nk_ctx->style.button.hover.data.color, nk_rgb(57,103,255));
	nk_style_push_color(nk_ctx, &nk_ctx->style.button.active.data.color, nk_rgb(57,103,255));
	nk_style_push_color(nk_ctx, &nk_ctx->style.window.background, nk_rgba(0, 0, 0, 0));


	nk_style_push_float(nk_ctx, &nk_ctx->style.window.rounding, 32.0f); */

    while (true)
    {
        ui_begin();
        appman_update();
        rtc_get_time(&dt);
        sprintf(dt1_str, "%i:%i:%i", dt.hour, dt.min, dt.sec);
        sprintf(dt2_str, "%i/%i/%i", dt.month, dt.day, dt.year);
        nk_begin(nk_ctx, "__Taskbar__", nk_rect(0.0f, 0.0f, lfb_width, 38.0f), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER);
        nk_layout_row_static(nk_ctx, 32.0f, 64.0f, 3);
        if (nk_button_label(nk_ctx, "Menu"))
            enable_app_menu = true;
        nk_label(nk_ctx, dt1_str, NK_TEXT_RIGHT);
        nk_label(nk_ctx, dt2_str, NK_TEXT_RIGHT);
        nk_end(nk_ctx);
        
        app_menu(nk_ctx);
        app_snake();
        app_tetris();
        
        ui_end();
    }

    ui_shutdown();

    appman_close_all();

    graphics_shutdown();
}