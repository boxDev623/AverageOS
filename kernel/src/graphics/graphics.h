#pragma once

#include "devices/lfb.h"

#include <stdint.h>

void graphics_initialize(uint32_t w, uint32_t h, uint32_t bpp);
void graphics_fillrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);