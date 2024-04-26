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

kernel_memory_map_t kmap;
 
void kmain(unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mboot_info = (multiboot_info_t*)addr;

    gdt_initialize();
    idt_initialize();
    bios32_initialize();

    terminal_initialize();

    get_kernel_memory_map(&kmap, mboot_info);
    display_kernel_memory_map(&kmap);

    pmm_initialize(kmap.available.start_addr, kmap.available.size);
	pmm_initialize_region(kmap.available.start_addr, kmap.available.size);

    lfb_initialize(640, 480, 32);
    for (int i = 0; i < 100; i++)
    {
        lfb_put_pixel(i, i, lfb_rgb(255, 0, 0));
    }
}