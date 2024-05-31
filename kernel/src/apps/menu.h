#pragma once

#include "devices/lfb.h"
#include "graphics/nuklear.h"
#include <stdbool.h>

extern bool enable_app_menu;
void app_menu(struct nk_context* ctx);