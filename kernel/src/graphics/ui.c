#include "ui.h"
#include "devices/ps2/keyboard.h"

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FIXED_TYPES
#define NK_IMPLEMENTATION
#define NK_RAWFB_IMPLEMENTATION
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_SOFTWARE_FONT
#define NK_ASSERT

#include "mm/liballoc.h"

#define STBTT_malloc(x,u)  ((void)(u),kmalloc(x))
#define STBTT_free(x,u)    ((void)(u),kfree(x))

#define STBTT_assert

#include "nuklear.h"
#include "nuklear_rawfb.h"

#include "images/wallpaper.h"

unsigned char tex_scratch[512 * 512];

struct rawfb_context *rawfb;
struct nk_context *nk_ctx;

void ui_initialize(void)
{
	struct rawfb_pl pl;
    pl.bytesPerPixel = 4;
    pl.ashift = 24;
    pl.rshift = 16;
    pl.gshift = 8;
    pl.bshift = 0;
    pl.aloss = 0;
    pl.rloss = 0;
    pl.gloss = 0;
    pl.bloss = 0;

    rawfb = nk_rawfb_init(graphics_back_buffer, tex_scratch, lfb_width, lfb_height, lfb_width * 4, pl);
	nk_ctx = &rawfb->ctx;
}

void ui_shutdown(void)
{
    nk_rawfb_shutdown(rawfb);
}

void ui_begin(void)
{
    nk_input_begin(nk_ctx);

    int32_t mouse_x = mouse_get_x();
    int32_t mouse_y = mouse_get_y();
    mouse_status_t mouse_status = mouse_get_status();

    nk_input_motion(nk_ctx, mouse_x, mouse_y);
    nk_input_button(nk_ctx, NK_BUTTON_LEFT, mouse_x, mouse_y, mouse_status.left_button);
    nk_input_button(nk_ctx, NK_BUTTON_MIDDLE, mouse_x, mouse_y, mouse_status.middle_button);
    nk_input_button(nk_ctx, NK_BUTTON_RIGHT, mouse_x, mouse_y, mouse_status.right_button);

    nk_input_key(nk_ctx, NK_KEY_TEXT_START, keyboard_get_scancode(SCAN_CODE_KEY_HOME));
    nk_input_key(nk_ctx, NK_KEY_TEXT_END, keyboard_get_scancode(SCAN_CODE_KEY_END));
    nk_input_key(nk_ctx, NK_KEY_SCROLL_START, keyboard_get_scancode(SCAN_CODE_KEY_HOME));
    nk_input_key(nk_ctx, NK_KEY_SCROLL_END, keyboard_get_scancode(SCAN_CODE_KEY_END));
    nk_input_key(nk_ctx, NK_KEY_SCROLL_DOWN, keyboard_get_scancode(SCAN_CODE_KEY_PAGE_DOWN));
    nk_input_key(nk_ctx, NK_KEY_SCROLL_UP, keyboard_get_scancode(SCAN_CODE_KEY_PAGE_UP));
    nk_input_key(nk_ctx, NK_KEY_SHIFT, keyboard_get_scancode(SCAN_CODE_KEY_LEFT_SHIFT)||
                                    keyboard_get_scancode(SCAN_CODE_KEY_RIGHT_SHIFT));

    for (int32_t i = 0; i < keyboard_event_data_length; ++i)
	{
		if (keyboard_get_scancode(SCAN_CODE_KEY_LEFT_CTRL))
		{
			switch (keyboard_event_data[i].scancode)
			{
			case SCAN_CODE_KEY_C: nk_input_key(nk_ctx, NK_KEY_COPY, true); break;
			case SCAN_CODE_KEY_V: nk_input_key(nk_ctx, NK_KEY_PASTE, true); break;
			case SCAN_CODE_KEY_X: nk_input_key(nk_ctx, NK_KEY_CUT, true); break;
			case SCAN_CODE_KEY_Z: nk_input_key(nk_ctx, NK_KEY_TEXT_UNDO, true); break;
			case SCAN_CODE_KEY_R: nk_input_key(nk_ctx, NK_KEY_TEXT_REDO, true); break;
			case SCAN_CODE_KEY_LEFT: nk_input_key(nk_ctx, NK_KEY_TEXT_WORD_LEFT, true); break;
			case SCAN_CODE_KEY_RIGHT: nk_input_key(nk_ctx, NK_KEY_TEXT_WORD_RIGHT, true); break;
			case SCAN_CODE_KEY_B: nk_input_key(nk_ctx, NK_KEY_TEXT_LINE_START, true); break;
			case SCAN_CODE_KEY_E: nk_input_key(nk_ctx, NK_KEY_TEXT_LINE_END, true); break;
			case SCAN_CODE_KEY_A: nk_input_key(nk_ctx, NK_KEY_TEXT_SELECT_ALL, true); break;
			}
		}
		else
		switch (keyboard_event_data[i].scancode)
		{
        case SCAN_CODE_KEY_ENTER: nk_input_key(nk_ctx, NK_KEY_ENTER, true); break;
		case SCAN_CODE_KEY_BACKSPACE: nk_input_key(nk_ctx, NK_KEY_BACKSPACE, true); break;
		case SCAN_CODE_KEY_DELETE: nk_input_key(nk_ctx, NK_KEY_DEL, true); break;
		case SCAN_CODE_KEY_LEFT: nk_input_key(nk_ctx, NK_KEY_LEFT, true); break;
		case SCAN_CODE_KEY_RIGHT: nk_input_key(nk_ctx, NK_KEY_RIGHT, true); break;
		case SCAN_CODE_KEY_UP: nk_input_key(nk_ctx, NK_KEY_UP, true); break;
		case SCAN_CODE_KEY_DOWN: nk_input_key(nk_ctx, NK_KEY_DOWN, true); break;
    	case SCAN_CODE_KEY_TAB: nk_input_key(nk_ctx, NK_KEY_TAB, true); break;

		default:
			if (keyboard_event_data[i].ch)
				nk_input_char(nk_ctx, keyboard_event_data[i].ch);
			break;
		}
	}

    nk_input_end(nk_ctx);
	keyboard_reset_event_data();

    if (nk_begin(nk_ctx, "__Cursor__", nk_rect(-1, -1, 0, 0), 0));
    nk_end(nk_ctx);
}

void ui_end(void)
{
    {
        uint32_t *wallpaper = wallpaper_waves_01;
        uint32_t *back_buffer = graphics_back_buffer;

        uint32_t *end_buffer = wallpaper + lfb_width * lfb_height;

        for (; wallpaper < end_buffer; wallpaper++, back_buffer++)
            *back_buffer = *wallpaper;
    }

    nk_rawfb_render(rawfb, nk_black, 0);

    graphics_swapbuffers();
}