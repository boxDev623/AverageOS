#pragma once

#include <stdint.h>

void pit_initialize(uint32_t freq, isr_t callback);