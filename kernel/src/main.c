#include "intr/idt.h"
#include "intr/gdt.h"
#include "intr/isr.h"

#include "terminal.h"

#include "mm/pmm.h"
#include "mm/memmap.h"

#include "sys/bios32.h"

#include "multiboot.h"

#include <printk.h>
#include <string.h>

#include <mm/liballoc.h>

#include "devices/lfb.h"

#include "io.h"

#include <openlibm.h>

kernel_memory_map_t kmap;

void __stack_chk_fail(void)
{

}

void __attribute__ ((noreturn))
__stack_chk_fail_local (void)
{
  __stack_chk_fail ();
}

float i = 0;

void timer(void)
{
    i++;
    lfb_put_pixel(i, sinf(i / 10.0f) * (i / 10.0f) + 16.0f, lfb_rgb(255, 0, 0));
}

void kmain(unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mboot_info = (multiboot_info_t*)addr;

    gdt_initialize();
    idt_initialize();
    bios32_initialize();

    terminal_initialize();

    isr_register_interrupt_handler(IRQ_BASE + IRQ0_TIMER, timer);
    uint32_t divisor = 1193180;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    outportb(0x43, 0x36);
    outportb(0x40, low);
    outportb(0x40, high);

    get_kernel_memory_map(&kmap, mboot_info);
    display_kernel_memory_map(&kmap);

    pmm_initialize(kmap.available.start_addr, kmap.available.size);
	pmm_initialize_region(kmap.available.start_addr, kmap.available.size);

    lfb_initialize(640, 480, 32);
}