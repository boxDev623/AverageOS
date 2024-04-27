#include "ui.h"

void ui_draw_bg()
{
    graphics_fillrect(0, 0, 1024, 768, lfb_rgb(25, 25, 25));
}

void ui_draw_menu()
{
    if (mouse_get_state().left_button)
    {
        graphics_fillrect(mouse_get_x(), mouse_get_y(), 100, 300, lfb_rgb(255,255,255));
        graphics_drawstring("test", mouse_get_x()+10, mouse_get_y()+1, lfb_rgb(0,0,0));
    }
}

void ui_draw_mouse(bool can_move)
{
    if (can_move)
    {
        for (int y = 0; y < 15; y++)
            for (int x = 0; x < 11; x++)
                if (g_gui_cursor[y * 11 + x] != 16711680)
                    lfb_put_pixel(mouse_get_x() + x, mouse_get_y() + y, g_gui_cursor[y * 11 + x]);
    }
}