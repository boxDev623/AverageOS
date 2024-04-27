#include "init.h"
#include "graphics/graphics.h"
#include "graphics/ui/ui.h"

#include <printk.h>
#include <string.h>
#include <openlibm.h>

// -----------a random fix for something mysterious my brain cant comprehend-------------
kernel_memory_map_t kmap;
void __stack_chk_fail(void){}
void __attribute__ ((noreturn))
__stack_chk_fail_local (void){__stack_chk_fail ();}
// ---------------------------------------------

void kmain(unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mboot_info = (multiboot_info_t*)addr;
    kernel_initialize(mboot_info);

    graphics_initialize(1024, 768, 32);

    bool can_move = true;
    while (true)
    {
        ui_draw_bg();

        

        ui_draw_mouse(can_move);
    }
}