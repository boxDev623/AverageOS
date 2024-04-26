#pragma once

#include <stdint.h>

#define NO_GDT_DESCRIPTORS     8

typedef struct
{
    uint16_t segment_limit;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
}
__attribute__((packed)) gdt_t;

typedef struct
{
    uint16_t limit;
    uint32_t base_address;
}
__attribute__((packed)) gdt_ptr_t;

void gdt_initialize(void);