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

kernel_memory_map_t kmap;

void pit_event(regs_t *regs)
{
    inportb(0x60);
    //terminal_writestring("never gonna give you up, never gonna let you down, ");
    terminal_writestring("a");
}
 
void kmain(unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mboot_info = (multiboot_info_t*)addr;

    gdt_initialize();
    idt_initialize();

    bios32_initialize();

    terminal_initialize();
    terminal_writestring("Hello, kernel World!\n");

    isr_register_interrupt_handler(IRQ_BASE + IRQ1_KEYBOARD, pit_event);

    get_kernel_memory_map(&kmap, mboot_info);
    display_kernel_memory_map(&kmap);

    pmm_initialize(kmap.available.start_addr, kmap.available.size);
	pmm_initialize_region(kmap.available.start_addr, kmap.available.size);

    char *name1 = kmalloc(64);
    strcpy(name1, "Hello");
    kfree(name1);
    char *name2 = kmalloc(64);
    strcpy(name2, "fgadfadsf");
    terminal_writestring(name1);
}