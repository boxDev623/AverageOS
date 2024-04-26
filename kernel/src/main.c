#include "intr/idt.h"
#include "intr/gdt.h"
#include "intr/isr.h"

#include "terminal.h"

#include "io.h"

void pit_event(regs_t *regs)
{
    inportb(0x60);
    //terminal_writestring("never gonna give you up, never gonna let you down, ");
    terminal_writestring("a");
}
 
void kmain(uint64_t magic, uint64_t addr)
{
    terminal_initialize();
    terminal_writestring("Hello, kernel World!\n");

    gdt_initialize();
    idt_initialize();

    isr_register_interrupt_handler(IRQ_BASE + IRQ1_KEYBOARD, pit_event);
}