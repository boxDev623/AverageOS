#include "graphics.h"

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