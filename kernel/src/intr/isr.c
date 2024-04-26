#include "isr.h"

#include <stddef.h>

isr_t g_interrupt_handlers[NO_INTERRUPT_HANDLERS];

char *exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available (No Math Coprocessor)",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "Unknown Interrupt (intel reserved)",
    "x87 FPU Floating-Point Error (Math Fault)",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_register_interrupt_handler(int32_t num, isr_t handler)
{
    if (num < NO_INTERRUPT_HANDLERS)
        g_interrupt_handlers[num] = handler;
}

void isr_end_interrupt(int32_t num)
{
    pic8259_eoi(num);
}

void isr_irq_handler(regs_t *reg)
{
    if (g_interrupt_handlers[reg->int_no] != NULL)
    {
        isr_t handler = g_interrupt_handlers[reg->int_no];
        handler(reg);
    }
    pic8259_eoi(reg->int_no);
}

void isr_exception_handler(regs_t reg)
{
    if (reg.int_no < 32)
    {
        // log exception somehow
    }

    if (g_interrupt_handlers[reg.int_no] != NULL)
    {
        isr_t handler = g_interrupt_handlers[reg.int_no];
        handler(&reg);
    }
}