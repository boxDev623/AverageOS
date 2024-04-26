#include "intr/idt.h"
#include "intr/gdt.h"
#include "intr/isr.h"

#include "terminal.h"

#include "mm/pmm.h"
#include "mm/memmap.h"

#include "multiboot.h"

#include <printk.h>
#include <string.h>

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

    terminal_initialize();
    terminal_writestring("Hello, kernel World!\n");

    gdt_initialize();
    idt_initialize();

    isr_register_interrupt_handler(IRQ_BASE + IRQ1_KEYBOARD, pit_event);

    get_kernel_memory_map(&kmap, mboot_info);
    display_kernel_memory_map(&kmap);

    pmm_initialize(kmap.available.start_addr, kmap.available.size);
	pmm_initialize_region(kmap.available.start_addr, 4 * PMM_BLOCK_SIZE);

    char *name1 = (char*)pmm_alloc_block();
    strcpy(name1, "Hello");
    pmm_free_block((void*)name1);
    char *name2 = (char*)pmm_alloc_block();
    strcpy(name2, "fgadfadsf");
    terminal_writestring(name1);
}