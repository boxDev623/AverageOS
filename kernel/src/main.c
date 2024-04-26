#include "intr/idt.h"
#include "intr/gdt.h"
#include "intr/isr.h"

#include "terminal.h"

#include "mm/pmm.h"

#include "multiboot.h"

#include <printk.h>
#include <string.h>

extern uint8_t __kernel_section_start;
extern uint8_t __kernel_section_end;
extern uint8_t __kernel_text_section_start;
extern uint8_t __kernel_text_section_end;
extern uint8_t __kernel_data_section_start;
extern uint8_t __kernel_data_section_end;
extern uint8_t __kernel_rodata_section_start;
extern uint8_t __kernel_rodata_section_end;
extern uint8_t __kernel_bss_section_start;
extern uint8_t __kernel_bss_section_end;

typedef struct {
    struct {
        uint32_t k_start_addr;
        uint32_t k_end_addr;
        uint32_t k_len;
        uint32_t text_start_addr;
        uint32_t text_end_addr;
        uint32_t text_len;
        uint32_t data_start_addr;
        uint32_t data_end_addr;
        uint32_t data_len;
        uint32_t rodata_start_addr;
        uint32_t rodata_end_addr;
        uint32_t rodata_len;
        uint32_t bss_start_addr;
        uint32_t bss_end_addr;
        uint32_t bss_len;
    } kernel;

    struct {
        uint32_t total_memory;
    } system;

    struct {
        uint32_t start_addr;
        uint32_t end_addr;
        uint32_t size;
    } available;
} kernel_memory_map_t;

kernel_memory_map_t kmap;

int32_t get_kernel_memory_map(kernel_memory_map_t *kmap, multiboot_info_t *mboot_info)
{
    uint32_t i;

    if (kmap == NULL) return -1;
    kmap->kernel.k_start_addr = (uint32_t)&__kernel_section_start;
    kmap->kernel.k_end_addr = (uint32_t)&__kernel_section_end;
    kmap->kernel.k_len = ((uint32_t)&__kernel_section_end - (uint32_t)&__kernel_section_start);

    kmap->kernel.text_start_addr = (uint32_t)&__kernel_text_section_start;
    kmap->kernel.text_end_addr = (uint32_t)&__kernel_text_section_end;
    kmap->kernel.text_len = ((uint32_t)&__kernel_text_section_end - (uint32_t)&__kernel_text_section_start);

    kmap->kernel.data_start_addr = (uint32_t)&__kernel_data_section_start;
    kmap->kernel.data_end_addr = (uint32_t)&__kernel_data_section_end;
    kmap->kernel.data_len = ((uint32_t)&__kernel_data_section_end - (uint32_t)&__kernel_data_section_start);

    kmap->kernel.rodata_start_addr = (uint32_t)&__kernel_rodata_section_start;
    kmap->kernel.rodata_end_addr = (uint32_t)&__kernel_rodata_section_end;
    kmap->kernel.rodata_len = ((uint32_t)&__kernel_rodata_section_end - (uint32_t)&__kernel_rodata_section_start);

    kmap->kernel.bss_start_addr = (uint32_t)&__kernel_bss_section_start;
    kmap->kernel.bss_end_addr = (uint32_t)&__kernel_bss_section_end;
    kmap->kernel.bss_len = ((uint32_t)&__kernel_bss_section_end - (uint32_t)&__kernel_bss_section_start);

    kmap->system.total_memory = mboot_info->mem_low + mboot_info->mem_high;

    for (i = 0; i < mboot_info->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)(mboot_info->mmap_addr + i);
        if (mmap->type != MULTIBOOT_MEMORY_AVAILABLE) continue;
        if (mmap->addr_low == kmap->kernel.text_start_addr)
        {
            kmap->available.start_addr = kmap->kernel.k_end_addr + 1024 * 1024;
            kmap->available.end_addr = mmap->addr_low + mmap->len_low;
            kmap->available.size = kmap->available.end_addr - kmap->available.start_addr;
            return 0;
        }
    }

    return -1;
}

void display_kernel_memory_map(kernel_memory_map_t *kmap)
{
    printk("kernel:\n");
    printk("  kernel-start: 0x%x, kernel-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.k_start_addr, kmap->kernel.k_end_addr, kmap->kernel.k_len);
    printk("  text-start: 0x%x, text-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.text_start_addr, kmap->kernel.text_end_addr, kmap->kernel.text_len);
    printk("  data-start: 0x%x, data-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.data_start_addr, kmap->kernel.data_end_addr, kmap->kernel.data_len);
    printk("  rodata-start: 0x%x, rodata-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.rodata_start_addr, kmap->kernel.rodata_end_addr, kmap->kernel.rodata_len);
    printk("  bss-start: 0x%x, bss-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.bss_start_addr, kmap->kernel.bss_end_addr, kmap->kernel.bss_len);

    printk("total_memory: %d KB\n", kmap->system.total_memory);
    printk("available:\n");
    printk("  start_adddr: 0x%x\n  end_addr: 0x%x\n  size: %d\n",
           kmap->available.start_addr, kmap->available.end_addr, kmap->available.size);
}

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