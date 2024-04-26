#include "gdt.h"

gdt_t g_gdt[NO_GDT_DESCRIPTORS];
gdt_ptr_t g_gdt_ptr;

extern void gdt_load(uint32_t gdt_ptr);

void gdt_set_entry(int32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_t *this = &g_gdt[index];

    this->segment_limit = limit & 0xFFFF;
    this->base_low = base & 0xFFFF;
    this->base_middle = (base >> 16) & 0xFF;
    this->access = access;

    this->granularity = (limit >> 16) & 0x0F;
    this->granularity = this->granularity | (gran & 0xF0);

    this->base_high = (base >> 24 & 0xFF);
}

void gdt_initialize(void)
{
    g_gdt_ptr.limit = sizeof(g_gdt) - 1;
    g_gdt_ptr.base_address = (uint32_t)g_gdt;

    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_load((uint32_t)&g_gdt_ptr);
}