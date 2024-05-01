#include "lfb.h"
#include "intr/isr.h"
#include "sys/bios32.h"

#include <stdint.h>
#include <string.h>

vbe20_info_block_t g_vbe_info_block;
vbe20_mode_info_block_t g_vbe_mode_info_block;

int g_selected_mode = -1;

uint32_t g_width = 0, g_height = 0;

uint32_t *g_vbe_buffer = NULL;

uint32_t lfb_get_width(void) 
{
    return g_width;
}

uint32_t lfb_get_height(void) 
{
    return g_height;
}

int vbe_get_info(void)
{
    regs16_t in = {0}, out = {0};
    
    in.ax = 0x4F00;
    
    in.di = BIOS_CONVENTIONAL_MEMORY;
    int86(0x10, &in, &out);
    
    memcpy(&g_vbe_info_block, (void *)BIOS_CONVENTIONAL_MEMORY, sizeof(vbe20_info_block_t));
    return (out.ax == 0x4F);
}

void vbe_get_mode_info(uint16_t mode, vbe20_mode_info_block_t *mode_info)
{
    regs16_t in = {0}, out = {0};
    
    in.ax = 0x4F01;
    in.cx = mode; 
    in.di = BIOS_CONVENTIONAL_MEMORY + 1024;  
    int86(0x10, &in, &out);  
    
    memcpy(mode_info, (void *)BIOS_CONVENTIONAL_MEMORY + 1024, sizeof(vbe20_mode_info_block_t));
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
    uint16_t *mode_list = (uint16_t *)g_vbe_info_block.VideoModePtr;
    uint16_t mode = *mode_list++;
    while (mode != 0xffff)
    {
        vbe_get_mode_info(mode, &g_vbe_mode_info_block);
        if (g_vbe_mode_info_block.XResolution == width && g_vbe_mode_info_block.YResolution == height && g_vbe_mode_info_block.BitsPerPixel == bpp)
            return mode;
        mode = *mode_list++;
    }
    return -1;
}

void vbe_print_available_modes(void)
{
    vbe20_mode_info_block_t modeinfoblock;

    uint16_t *mode_list = (uint16_t *)g_vbe_info_block.VideoModePtr;
    uint16_t mode = *mode_list++;
    while (mode != 0xffff) {
        vbe_get_mode_info(mode, &modeinfoblock);
        
        mode = *mode_list++;
    }
}

uint32_t lfb_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t color = red;
    color <<= 16;
    color |= (green << 8);
    color |= blue;
    return color;
}

int lfb_initialize(uint32_t width, uint32_t height, uint32_t bpp)
{    
    if (!vbe_get_info())
        return -1;

    g_selected_mode = vbe_find_mode(width, height, bpp);
        
    if (g_selected_mode == -1)
        return -1;

    g_width = g_vbe_mode_info_block.XResolution;
    g_height = g_vbe_mode_info_block.YResolution;

    g_vbe_buffer = (uint32_t *)g_vbe_mode_info_block.PhysBasePtr;

    vbe_set_mode(g_selected_mode);

    return 0;
}

uint32_t *lfb_get_vbebuffer(void)
{
    return g_vbe_buffer;
}

void lfb_clear_vbebuffer(void)
{
    memset(g_vbe_buffer, lfb_rgb(25,25,25), g_width * g_height * sizeof(uint32_t));
}