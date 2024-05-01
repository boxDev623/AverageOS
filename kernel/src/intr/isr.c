#include "isr.h"

#include <stddef.h>
#include <printk.h>

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

static void isr_print_registers(regs_t *reg)
{
    printk("EXCEPTION: %s\n", exception_messages[reg->int_no]);
    printk("Registers:\n");
    printk("err_code=%d\n", reg->err_code);
    printk("eax=0x%x, ebx=0x%x, ecx=0x%x, edx=0x%x\n", reg->eax, reg->ebx, reg->ecx, reg->edx);
    printk("edi=0x%x, esi=0x%x, ebp=0x%x, esp=0x%x\n", reg->edi, reg->esi, reg->ebp, reg->esp);
    printk("eip=0x%x, cs=0x%x, ss=0x%x, eflags=0x%x, useresp=0x%x\n", reg->eip, reg->ss, reg->eflags, reg->useresp);
}

void isr_exception_handler(regs_t reg)
{
    if (reg.int_no < 32)
        isr_print_registers(&reg);

    if (g_interrupt_handlers[reg.int_no] != NULL)
    {
        isr_t handler = g_interrupt_handlers[reg.int_no];
        handler(&reg);
    }
}