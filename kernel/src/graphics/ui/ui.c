#include "ui.h"

void ui_draw_bg()
{
    graphics_fillrect(0, 0, 1024, 768, lfb_rgb(25, 25, 25));
}

void ui_draw_menu(bool can_move)
{
    while (can_move)
    {
        graphics_fillrect(mouse_get_x(), mouse_get_y(), 10, 10, lfb_rgb(255, 255, 255));
    }   
}