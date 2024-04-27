#include "ui.h"

void ui_draw_bg()
{
    graphics_fillrect(0, 0, 1024, 768, lfb_rgb(25, 25, 25));
}

void ui_draw_menu()
{
    if (mouse_get_state().right_button == 1)
        graphics_fillrect(0, 0, 100, 400, lfb_rgb(255,255,255));
}

void ui_draw_mouse(bool can_move)
{
    if (can_move)
        graphics_fillrect(mouse_get_x(), mouse_get_y(), 10, 10, lfb_rgb(255,0,255));
}