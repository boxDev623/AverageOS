#pragma once

#include "devices/lfb.h"

#include <stdint.h>

extern uint32_t *graphics_back_buffer;

void graphics_initialize(uint32_t w, uint32_t h, uint32_t bpp);
void graphics_shutdown(void);

void graphics_put_pixel(int x, int y, int color);
uint32_t graphics_get_pixel(int x, int y);

void graphics_swapbuffers(void);