#include "vesa.h"
#include "sys/bios32.h"
#include "io.h"
#include "intr/isr.h"
#include "string.h"
#include <stdint.h>

vbe20_infoblock_t g_vbe_infoblock;
vbe20_modeinfoblock_t g_vbe_modeinfoblock;
int g_selected_mode = -1;
uint32_t g_width = 0, g_height = 0;
uint32_t *g_vbe_buffer = NULL;
uint32_t *prev_buffer = NULL;

uint32_t vbe_get_width()
{
    return g_width;
}

uint32_t vbe_get_height()
{
    return g_height;
}

int get_vbe_info()
{
    regs16_t in = {0}, out = {0};
    in.ax = 0x4F00;
    in.di = BIOS_CONVENTIONAL_MEMORY;
    int86(0x10, &in, &out);
    memcpy(&g_vbe_infoblock, (void *)BIOS_CONVENTIONAL_MEMORY, sizeof(vbe20_infoblock_t));
    return (out.ax == 0x4F);
}

void get_vbe_mode_info(uint16_t mode, vbe20_modeinfoblock_t *mode_info)
{
    regs16_t in = {0}, out = {0};
    in.ax = 0x4F01;
    in.cx = mode;
    in.di = BIOS_CONVENTIONAL_MEMORY + 1024;
    int86(0x10, &in, &out);
    memcpy(mode_info, (void *)BIOS_CONVENTIONAL_MEMORY + 1024, sizeof(vbe20_modeinfoblock_t));
}

void vbe_set_mode(uint32_t mode)
{
    regs16_t in = {0}, out = {0};
    in.ax = 0x4F02;
    in.bx = mode;
    int86(0x10, &in, &out);
}

uint32_t vbe_find_mode(uint32_t width, uint32_t height, uint32_t bpp)
{
    uint16_t *mode_list = (uint16_t *)g_vbe_infoblock.VideoModePtr;
    uint16_t mode = *mode_list++;
    while (mode != 0xffff)
    {
        get_vbe_mode_info(mode, &g_vbe_modeinfoblock);
        if (g_vbe_modeinfoblock.XResolution == width && g_vbe_modeinfoblock.YResolution == height && g_vbe_modeinfoblock.BitsPerPixel == bpp) {
            return mode;
        }
        mode = *mode_list++;
    }
    return -1;
}

uint32_t vbe_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t color = red;
    color <<= 16;
    color |= (green << 8);
    color |= blue;
    return color;
}

void vbe_putpixel(int x, int y, int color)
{
    uint32_t i = y * g_width + x;
    *(g_vbe_buffer + i) = color;
}

uint32_t vbe_getpixel(int x, int y)
{
    uint32_t i = y * g_width + x;
    uint32_t color = *(g_vbe_buffer + i);
    return color;
}

int vesa_initialize(uint32_t width, uint32_t height, uint32_t bpp)
{
    g_selected_mode = vbe_find_mode(width, height, bpp);
    if (g_selected_mode == -1) {
        return -1;
    }

    g_width = g_vbe_modeinfoblock.XResolution;
    g_height = g_vbe_modeinfoblock.YResolution;
    g_vbe_buffer = (uint32_t *)g_vbe_modeinfoblock.PhysBasePtr;

    vbe_set_mode(g_selected_mode);
    
    return 0;
}