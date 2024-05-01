#include "graphics.h"

#include <stdint.h>
#include <string.h>

uint32_t _width = 0;

uint32_t* g_back_buffer = NULL;

void graphics_initialize(uint32_t w, uint32_t h, uint32_t bpp)
{
    lfb_initialize(w, h, bpp);
    g_back_buffer = kmalloc(lfb_get_width() * lfb_get_height() * sizeof(uint32_t));
}

void graphics_fillrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    for (int i = 0; i <= w; i++)
        for (int j = 0; j <= h; j++)
            graphics_put_pixel(i+x, j+y, color);
}

uint32_t* graphics_get_backbuffer(void)
{
    return g_back_buffer;
}

void graphics_swapbuffers(void)
{
    uint32_t *buffer = lfb_get_vbebuffer();
    uint32_t *back_buffer = g_back_buffer;
    uint32_t end_buffer = g_back_buffer + lfb_get_width() * lfb_get_height();
    for (; back_buffer < end_buffer; back_buffer++, buffer++)
        *buffer = *back_buffer;
}

void graphics_shutdown(void)
{
    kfree(g_back_buffer);
}

void graphics_put_pixel(int x, int y, int color)
{
    uint32_t i = y * lfb_get_width() + x;
    *(g_back_buffer + i) = color;
}

uint32_t graphics_get_pixel(int x, int y)
{
    uint32_t i = y * lfb_get_width() + x;
    uint32_t color = *(g_back_buffer + i);
    return color;
}