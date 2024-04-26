#include "printk.h"
#include "printf.h"

const char printk_buffer[5120];

extern void terminal_writestring(const char* data);

void printk(char *fmt, ...)
{
    va_list ptr;
    va_start(ptr, fmt);
    vsnprintf((char *)&printk_buffer, -1, fmt, ptr);

    terminal_writestring((char*)printk_buffer);

    va_end(ptr);
}