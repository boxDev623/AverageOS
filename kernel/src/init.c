#include "init.h"

void kernel_initialize(multiboot_info_t* mboot_info)
{
    gdt_initialize();
    idt_initialize();
    bios32_initialize();

    get_kernel_memory_map(&kmap, mboot_info);
    display_kernel_memory_map(&kmap);

    pmm_initialize(kmap.available.start_addr, kmap.available.size);
	pmm_initialize_region(kmap.available.start_addr, kmap.available.size);
}