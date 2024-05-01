#include "terminal.h"
#include "io.h"

#include <string.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_scroll(void)
{
    memmove(terminal_buffer, terminal_buffer + VGA_WIDTH, VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(uint16_t));

    size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH;
    for(size_t x = 0; x < VGA_WIDTH; ++x)
        terminal_buffer[index + x] = vga_entry(' ', vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_update_cursor(int32_t x, int32_t y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (uint8_t) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_putchar(char c) 
{
    if (c == '\n')
	{
		terminal_row++;
		terminal_column = 0;

		if (terminal_row >= VGA_HEIGHT)
		{
			terminal_scroll();
			terminal_column = 0;
			terminal_row--;
		}

		return;
	}
	
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}

    if (terminal_row >= VGA_HEIGHT)
    {
        terminal_scroll();
        terminal_column = 0;
        terminal_row--;
    }

	terminal_update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}