#pragma once

#include "graphics/graphics.h"
#include "devices/ps2/mouse.h"

#include <stdint.h>

void ui_draw_bg();
void ui_draw_menu();
void ui_draw_mouse(bool can_move);