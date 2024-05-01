#include "keyboard.h"
#include "intr/isr.h"

#include <stdbool.h>
#include <ctype.h>

static bool g_caps_lock = false;
static bool g_shift_pressed = false;
char g_ch = 0, g_scan_code = 0;

static key_callback g_key_event;
static char_callback g_char_event;

char g_scan_code_chars[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

bool scancode_map[89];

static int get_scancode(void)
{
    int i, scancode = 0;

    for (i = 1000; i > 0; i++)
    {
        if ((inportb(KEYBOARD_STATUS_PORT) & 1) == 0) continue;
        scancode = inportb(KEYBOARD_DATA_PORT);
        break;
    }
    if (i > 0)
        return scancode;
    return 0;
}

char alternate_chars(char ch)
{
    switch(ch) {
        case '`': return '~';
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '\"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        default: return ch;
    }
}

bool keyboard_get_scancode(int32_t scancode)
{
	return scancode_map[scancode];
}

void keyboard_handler(regs_t *r)
{
    int scancode;

    scancode = get_scancode();
    g_key_event(scancode);

	/*switch (scancode)
	{
		case SCAN_CODE_KEY_DELETE: return;
        case SCAN_CODE_KEY_BACKSPACE: return;
        case SCAN_CODE_KEY_HOME: return;

		case SCAN_CODE_KEY_UP: return;
		case SCAN_CODE_KEY_LEFT: return;
		case SCAN_CODE_KEY_DOWN: return;
		case SCAN_CODE_KEY_RIGHT: return;
	}*/

    g_ch = 0;

    if (scancode & 0x80)
    {
		scancode -= 0x80;

        // Release
		scancode_map[scancode] = false;

		switch(scancode)
		{
        case SCAN_CODE_KEY_LEFT_SHIFT:
            g_shift_pressed = false;
            break;
		}
    }
    else
    {
        // Down
		scancode_map[scancode] = true;

        switch(scancode)
        {
            case SCAN_CODE_KEY_CAPS_LOCK:
                if (g_caps_lock == false)
                    g_caps_lock = true;
                else
                    g_caps_lock = false;
                break;

            case SCAN_CODE_KEY_ENTER:
                g_ch = '\n';
                break;

            case SCAN_CODE_KEY_TAB:
                //g_ch = '\t';
                break;
	
            case SCAN_CODE_KEY_LEFT_SHIFT:
                g_shift_pressed = true;
                break;

            default:
                g_ch = g_scan_code_chars[scancode];
                if (g_caps_lock)
                {
                    if (g_shift_pressed)
                    {
                        g_ch = alternate_chars(g_ch);
                    }
                    else
                        g_ch = toupper(g_ch);
                }
                else
                {
                    if (g_shift_pressed)
                    {
                        if (isalpha(g_ch))
                            g_ch = toupper(g_ch);
                        else 
                        g_ch = alternate_chars(g_ch);
                    }
                    else
                        g_ch = g_scan_code_chars[scancode];
                }
        		g_char_event(g_ch);
                break;
        }
    }
}

void keyboard_initialize(key_callback key_event, char_callback char_event)
{
    g_key_event = key_event;
    g_char_event = char_event;

    isr_register_interrupt_handler(IRQ_BASE + 1, keyboard_handler);
}