#include "bios32.h"

#include "intr/gdt.h"
#include "intr/idt.h"

#include <string.h>
#include <printk.h>

extern void BIOS32_START(void);
extern void BIOS32_END(void);
extern void *bios32_gdt_ptr;
extern void *bios32_gdt_entries;
extern void *bios32_idt_ptr;
extern void *bios32_in_reg16_ptr;
extern void *bios32_out_reg16_ptr;
extern void *bios32_int_number_ptr;

#define REBASE_ADDRESS(x)  (void*)(0x7c00 + (void*)x - (uint32_t)BIOS32_START)

#define BIOS_CONVENTIONAL_MEMORY 0x7E00

idt_ptr_t g_real_mode_gdt;
idt_ptr_t g_real_mode_idt;

extern gdt_t g_gdt[NO_GDT_DESCRIPTORS];

void (*bios32_function)() = (void *)0x7c00;

void bios32_initialize(void)
{
    gdt_set_entry(6, 0, 0xffffffff, 0x9A, 0x0f);
    gdt_set_entry(7, 0, 0xffffffff, 0x92, 0x0f);
    g_real_mode_gdt.base_address = (uint32_t)g_gdt;
    g_real_mode_gdt.limit = sizeof(g_gdt) - 1;
    g_real_mode_idt.base_address = 0;
    g_real_mode_idt.limit = 0x3ff;
}

void bios32_service(uint8_t interrupt, regs16_t *in, regs16_t *out)
{
    void *new_code_base = (void *)0x7c00;

    memcpy(&bios32_gdt_entries, g_gdt, sizeof(g_gdt));
    g_real_mode_gdt.base_address = (uint32_t)REBASE_ADDRESS((&bios32_gdt_entries));
    memcpy(&bios32_gdt_ptr, &g_real_mode_gdt, sizeof(idt_ptr_t));
    memcpy(&bios32_idt_ptr, &g_real_mode_idt, sizeof(idt_ptr_t));
    memcpy(&bios32_in_reg16_ptr, in, sizeof(regs16_t));
    void *in_reg16_address = REBASE_ADDRESS(&bios32_in_reg16_ptr);
    memcpy(&bios32_int_number_ptr, &interrupt, sizeof(uint8_t));

    uint32_t size = (uint32_t)BIOS32_END - (uint32_t)BIOS32_START;
    memcpy(new_code_base, BIOS32_START, size);
    bios32_function();
    in_reg16_address = REBASE_ADDRESS(&bios32_out_reg16_ptr);
    memcpy(out, in_reg16_address, sizeof(regs16_t));

    gdt_initialize();
    idt_initialize();
}

void int86(uint8_t interrupt, regs16_t *in, regs16_t *out)
{
    bios32_service(interrupt, in, out);
}