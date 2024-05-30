#include "keyboard.h"
#include "intr/isr.h"
#include "io.h"

#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>

#define KEYBOARD_USE_CAPS ((g_shift_pressed && !g_capslocked) || (!g_shift_pressed && g_capslocked))

keyboard_event_data_t keyboard_event_data[KEYBOARD_MAX_EVENTS];
uint32_t keyboard_event_data_length;

bool g_shift_pressed, g_capslocked;
bool scancode_map[89];

char keyboard_scancode_to_char(uint8_t scancode)
{
    switch (scancode)
    {
        case SCAN_CODE_KEY_1: return g_shift_pressed ? '!' : '1';
        case SCAN_CODE_KEY_2: return g_shift_pressed ? '@' : '2';
        case SCAN_CODE_KEY_3: return g_shift_pressed ? '#' : '3';
        case SCAN_CODE_KEY_4: return g_shift_pressed ? '$' : '4';
        case SCAN_CODE_KEY_5: return g_shift_pressed ? '%' : '5';
        case SCAN_CODE_KEY_6: return g_shift_pressed ? '^' : '6';
        case SCAN_CODE_KEY_7: return g_shift_pressed ? '&' : '7';
        case SCAN_CODE_KEY_8: return g_shift_pressed ? '*' : '8';
        case SCAN_CODE_KEY_9: return g_shift_pressed ? '(' : '9';
        case SCAN_CODE_KEY_0: return g_shift_pressed ? ')' : '0';
        case SCAN_CODE_KEY_MINUS: return g_shift_pressed ? '_' : '-';
        case SCAN_CODE_KEY_EQUAL: return g_shift_pressed ? '+' : '=';
        case SCAN_CODE_KEY_Q: return KEYBOARD_USE_CAPS ? 'Q' : 'q';
        case SCAN_CODE_KEY_W: return KEYBOARD_USE_CAPS ? 'W' : 'w';
        case SCAN_CODE_KEY_E: return KEYBOARD_USE_CAPS ? 'E' : 'e';
        case SCAN_CODE_KEY_R: return KEYBOARD_USE_CAPS ? 'R' : 'r';
        case SCAN_CODE_KEY_T: return KEYBOARD_USE_CAPS ? 'T' : 't';
        case SCAN_CODE_KEY_Y: return KEYBOARD_USE_CAPS ? 'Y' : 'y';
        case SCAN_CODE_KEY_U: return KEYBOARD_USE_CAPS ? 'U' : 'u';
        case SCAN_CODE_KEY_I: return KEYBOARD_USE_CAPS ? 'I' : 'i';
        case SCAN_CODE_KEY_O: return KEYBOARD_USE_CAPS ? 'O' : 'o';
        case SCAN_CODE_KEY_P: return KEYBOARD_USE_CAPS ? 'P' : 'p';
        case SCAN_CODE_KEY_SQUARE_OPEN_BRACKET: return g_shift_pressed ? '{' : '[';
        case SCAN_CODE_KEY_SQUARE_CLOSE_BRACKET: return g_shift_pressed ? '}' : ']';
        case SCAN_CODE_KEY_A: return KEYBOARD_USE_CAPS ? 'A' : 'a';
        case SCAN_CODE_KEY_S: return KEYBOARD_USE_CAPS ? 'S' : 's';
        case SCAN_CODE_KEY_D: return KEYBOARD_USE_CAPS ? 'D' : 'd';
        case SCAN_CODE_KEY_F: return KEYBOARD_USE_CAPS ? 'F' : 'f';
        case SCAN_CODE_KEY_G: return KEYBOARD_USE_CAPS ? 'G' : 'g';
        case SCAN_CODE_KEY_H: return KEYBOARD_USE_CAPS ? 'H' : 'h';
        case SCAN_CODE_KEY_J: return KEYBOARD_USE_CAPS ? 'J' : 'j';
        case SCAN_CODE_KEY_K: return KEYBOARD_USE_CAPS ? 'K' : 'k';
        case SCAN_CODE_KEY_L: return KEYBOARD_USE_CAPS ? 'L' : 'l';
        case SCAN_CODE_KEY_SEMICOLON: return g_shift_pressed ? ':' : ';';
        case SCAN_CODE_KEY_SINGLE_QUOTE: return g_shift_pressed ? '\"' : '\'';
        case SCAN_CODE_KEY_BACKSLASH: return g_shift_pressed ? '|' : '\\';
        case SCAN_CODE_KEY_Z: return KEYBOARD_USE_CAPS ? 'Z' : 'z';
        case SCAN_CODE_KEY_X: return KEYBOARD_USE_CAPS ? 'X' : 'x';
        case SCAN_CODE_KEY_C: return KEYBOARD_USE_CAPS ? 'C' : 'c';
        case SCAN_CODE_KEY_V: return KEYBOARD_USE_CAPS ? 'V' : 'v';
        case SCAN_CODE_KEY_B: return KEYBOARD_USE_CAPS ? 'B' : 'b';
        case SCAN_CODE_KEY_N: return KEYBOARD_USE_CAPS ? 'N' : 'n';
        case SCAN_CODE_KEY_M: return KEYBOARD_USE_CAPS ? 'M' : 'm';
        case SCAN_CODE_KEY_COMMA: return g_shift_pressed ? '<' : ',';
        case SCAN_CODE_KEY_DOT: return g_shift_pressed ? '>' : '.';
        case SCAN_CODE_KEY_FORESLHASH: return g_shift_pressed ? '?' : '/';
        case SCAN_CODE_KEY_SPACE: return ' ';
		case SCAN_CODE_KEY_ENTER: return '\n';
		case SCAN_CODE_KEY_TAB: return '\t';
		default: return 0;
    }
}

void keyboard_reset_event_data(void)
{
	keyboard_event_data_length = 0;
}

bool keyboard_get_scancode(uint8_t scancode)
{
	return scancode_map[scancode];
}

void keyboard_handler(regs_t *r)
{
	if(!(inportb(KEYBOARD_STATUS_PORT) & 1))
		return;

    uint8_t scancode = inportb(KEYBOARD_DATA_PORT);

	if (keyboard_event_data_length >= KEYBOARD_MAX_EVENTS)
		return;

	keyboard_event_data_t *event_data = &keyboard_event_data[keyboard_event_data_length];
	event_data->scancode = scancode;

    if (scancode & 0x80)
	{
		scancode -= 0x80;
		scancode_map[scancode] = false;

		switch (scancode)
		{
		case SCAN_CODE_KEY_LEFT_SHIFT:
			g_shift_pressed = false;
			break;

		case SCAN_CODE_KEY_RIGHT_SHIFT:
			g_shift_pressed = false;
			break;
		}

		event_data->ch = 0;
	}
	else
	{
		scancode_map[scancode] = true;

		switch (scancode)
		{
		case SCAN_CODE_KEY_CAPS_LOCK:
			g_capslocked = !g_capslocked;
			break;

		case SCAN_CODE_KEY_LEFT_SHIFT:
			g_shift_pressed = true;
			break;

		case SCAN_CODE_KEY_RIGHT_SHIFT:
			g_shift_pressed = true;
			break;
		
		default:
			event_data->ch = keyboard_scancode_to_char(scancode);
			break;
		}
	}

	keyboard_event_data_length++;
}

void keyboard_initialize(void)
{
    isr_register_interrupt_handler(IRQ_BASE + IRQ1_KEYBOARD, keyboard_handler);
}