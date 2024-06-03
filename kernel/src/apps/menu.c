#include "menu.h"

#include "settings.h"
#include "snake.h"
#include "tetris.h"
#include "text_editor.h"

bool enable_app_menu = false;

void app_menu(struct nk_context* ctx)
{
    if (enable_app_menu)
    {
        nk_begin(ctx, "Menu", nk_rect(0.0f, lfb_height - 362.0f, 250.0f, 400.0f),
        NK_WINDOW_NO_SCROLLBAR);
        nk_layout_row_static(ctx, 20.0f, 225, 1);

        if (nk_button_label(ctx, "Settings")){
            if (!enable_app_settings)
            {
                app_settings_initialize();
                enable_app_settings = true;
            }
            enable_app_menu = false;
        }
        if (nk_button_label(ctx, "Snake")){
            if (!enable_app_snake)
            {
                app_snake_initialize();
                enable_app_snake = true;
            }
            enable_app_menu = false;
        }
        if (nk_button_label(ctx, "Tetris")){
            if (!enable_app_tetris)
            {
                app_tetris_intialize();
                enable_app_tetris = true;
            }
            enable_app_menu = false;
        }
        if (nk_button_label(ctx, "Text Editor")){
            if (!enable_app_text_editor)
            {
                enable_app_text_editor = true;
            }
            enable_app_menu = false;
        }

        nk_end(ctx);
    }

    if (enable_app_settings)
        app_settings_update();

    if (enable_app_snake)
        app_snake_update();

    if (enable_app_text_editor)
        app_text_editor_update();

    if (enable_app_tetris)
        app_tetris_update();
}