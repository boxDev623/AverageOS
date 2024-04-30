#pragma once

#include "graphics/graphics.h"
#include "devices/ps2/mouse.h"

#include <stdint.h>

void ui_initialize(void);
void ui_shutdown(void);
void ui_render(void);

void ui_draw_menu();