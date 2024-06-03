#include "text_editor.h"
#include "graphics/nuklear.h"
#include "graphics/ui.h"

#define MAX_TEXT_LENGTH 1024

bool enable_app_text_editor = false;
static char text[MAX_TEXT_LENGTH] = "";
static int32_t len;

void app_text_editor_update(void)
{
    if (nk_begin(nk_ctx, "Text Editor", nk_rect(50, 50, 500, 500), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE))
    {
        int x, y;
        struct nk_rect bounds = nk_window_get_content_region(nk_ctx);
        x = (int)bounds.w;
        y = (int)bounds.h;
        nk_layout_row_static(nk_ctx, y, x, 1);
        nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_FIELD | NK_EDIT_MULTILINE, text, MAX_TEXT_LENGTH, nk_filter_default);
    }
    nk_end(nk_ctx);

    if (nk_window_is_hidden(nk_ctx, "Text Editor"))
        enable_app_text_editor = false;
}