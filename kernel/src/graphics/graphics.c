#include "graphics.h"
#include "fonts.h"

uint32_t _width = 0;

void graphics_initialize(uint32_t w, uint32_t h, uint32_t bpp)
{
    lfb_initialize(w, h, bpp);
}

void graphics_fillrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    for (int i = 0; i <= w; i++)
        for (int j = 0; j <= h; j++)
            lfb_put_pixel(i+x, j+y, color);
}

void graphics_drawchar(char ch, int x, int y, int color)
{
    int temp = 0, pix_data = 0;

    for (int i = 0; i < 16; i++)
    {
        temp = x;
        x = 0;
        pix_data = g_8x16_font[(int)ch * 16 + i];
        while (pix_data > 0)
        {
            if (pix_data & 1)
            {
                lfb_put_pixel(temp + 8 - x, y, color);
            }
            pix_data >>= 1;
            x++;
        }
        x = temp;
        y++;
    }
}

void graphics_drawstring(const char *str, int x, int y, int color)
{
    int new_x = x;
    int new_y = y;
    while (*str)
    {
        graphics_drawchar(*str, new_x, new_y, color);
        str++;
        new_x += 9;
        if (new_x > (int)_width)
        {
            new_y += 17;
            new_x = x;
        }
    }
}