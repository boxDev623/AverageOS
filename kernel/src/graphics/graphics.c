#include "graphics.h"
#include "mm/liballoc.h"

#include <stdint.h>
#include <string.h>

uint32_t *graphics_back_buffer;

void graphics_initialize(uint32_t w, uint32_t h, uint32_t bpp)
{
    lfb_initialize(w, h, bpp);
    graphics_back_buffer = (uint32_t*)kmalloc(lfb_width * lfb_height * sizeof(uint32_t));
}

void graphics_fillrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    for (int i = 0; i <= w; i++)
        for (int j = 0; j <= h; j++)
            graphics_put_pixel(i+x, j+y, color);
}

void graphics_swapbuffers(void)
{
    uint32_t *buffer = lfb_vbe_buffer;
    uint32_t *back_buffer = graphics_back_buffer;
    uint32_t *end_buffer = graphics_back_buffer + lfb_width * lfb_height;
    for (; back_buffer < end_buffer; back_buffer++, buffer++)
        *buffer = *back_buffer;
}

void graphics_shutdown(void)
{
    kfree(graphics_back_buffer);
}

void graphics_put_pixel(int x, int y, int color)
{
    uint32_t i = y * lfb_width + x;
    *(graphics_back_buffer + i) = color;
}

uint32_t graphics_get_pixel(int x, int y)
{
    uint32_t i = y * lfb_width + x;
    uint32_t color = *(graphics_back_buffer + i);
    return color;
}