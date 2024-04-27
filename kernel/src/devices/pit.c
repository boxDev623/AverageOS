#include "pit.h"

#include "intr/isr.h"
#include "io.h"

void pit_initialize(uint32_t freq, isr_t callback)
{
    isr_register_interrupt_handler(IRQ_BASE + IRQ0_TIMER, callback);

    uint32_t divisor = 1193180 / freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    outportb(0x43, 0x36);
    outportb(0x40, low);
    outportb(0x40, high);
}