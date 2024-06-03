#include "text_editor.h"
#include "graphics/nuklear.h"
#include "graphics/ui.h"

#include <string.h>

#include "graphics/images/wallpaper.h"

extern uint32_t *wallpaper_current_ptr;

struct Settings {
    char wallpaper_type[32];
    float volume;
};

static struct Settings settings;

bool enable_app_settings = false;

void app_settings_initialize(void)
{
    strcpy(settings.wallpaper_type, "AverageOS");
}

void app_settings_update(void)
{
    if (nk_begin(nk_ctx, "Settings", nk_rect(50, 50, 400, 400), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE)) {
        if (nk_tree_push(nk_ctx, NK_TREE_TAB, "Wallpaper", NK_MINIMIZED)) {
            nk_layout_row_dynamic(nk_ctx, 30, 1);
            nk_label(nk_ctx, "Select Wallpaper Type:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(nk_ctx, 30, 1);
            if (nk_combo_begin_label(nk_ctx, settings.wallpaper_type, nk_vec2(200,200))) {
                nk_layout_row_dynamic(nk_ctx, 25, 1);
                if (nk_combo_item_label(nk_ctx, "AverageOS", NK_TEXT_LEFT))
                {
                    wallpaper_current_ptr = wallpaper_averageos_01;
                    strcpy(settings.wallpaper_type, "AverageOS");
                }
                if (nk_combo_item_label(nk_ctx, "Mountains", NK_TEXT_LEFT))
                {
                    wallpaper_current_ptr = wallpaper_mountains_01;
                    strcpy(settings.wallpaper_type, "Mountains");
                }
                if (nk_combo_item_label(nk_ctx, "Waves", NK_TEXT_LEFT))
                {
                    wallpaper_current_ptr = wallpaper_waves_01;
                    strcpy(settings.wallpaper_type, "Waves");
                }
                if (nk_combo_item_label(nk_ctx, "Bliss", NK_TEXT_LEFT))
                {
                    wallpaper_current_ptr = wallpaper_bliss_01;
                    strcpy(settings.wallpaper_type, "Bliss");
                }
                nk_combo_end(nk_ctx);
            }
            nk_tree_pop(nk_ctx);
        }

        // Sound tab
        if (nk_tree_push(nk_ctx, NK_TREE_TAB, "Sound", NK_MINIMIZED)) {
            nk_layout_row_dynamic(nk_ctx, 30, 1);
            nk_label(nk_ctx, "Volume:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(nk_ctx, 30, 1);
            nk_slider_float(nk_ctx, 0, &(settings.volume), 100, 1);
            nk_tree_pop(nk_ctx);
        }
    }
    nk_end(nk_ctx);

    if (nk_window_is_hidden(nk_ctx, "Settings"))
        enable_app_settings = false;
}