#include "menu.h"

#include "snake.h"
#include "tetris.h"

bool enable_app_menu = false;

void app_menu(struct nk_context* ctx)
{
    if (enable_app_menu)
    {
        nk_begin(ctx, "Menu", nk_rect(0.0f, lfb_height - 362.0f, 250.0f, 400.0f),
        NK_WINDOW_NO_SCROLLBAR);
        nk_layout_row_static(ctx, 20.0f, 225, 1);

        if (nk_button_label(ctx, "Snake") && !enable_app_snake){
            enable_app_snake = true;
            enable_app_menu = false;
        } else if (nk_button_label(ctx, "Tetris") && !enable_app_tetris){
            enable_app_tetris = true;
            enable_app_menu = false;
        }

        nk_end(ctx);
    }
}