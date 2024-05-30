#pragma once

#include "graphics/graphics.h"
#include "devices/ps2/mouse.h"

#include <stdint.h>

extern struct nk_context *nk_ctx;

void ui_initialize(void);
void ui_shutdown(void);

void ui_begin(void);
void ui_end(void);

void ui_key_event(int32_t scancode);
void ui_char_event(int32_t ch);