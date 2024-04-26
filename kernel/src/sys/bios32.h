#pragma once

#include "intr/isr.h"

void bios32_initialize(void);
void int86(uint8_t interrupt, regs16_t *in, regs16_t *out);