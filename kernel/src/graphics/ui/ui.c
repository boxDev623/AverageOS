#include "ui.h"
#include "devices/ps2/keyboard.h"

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FIXED_TYPES
#define NK_IMPLEMENTATION
#define NK_RAWFB_IMPLEMENTATION
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_SOFTWARE_FONT

#include "mm/liballoc.h"

#define STBTT_malloc(x,u)  ((void)(u),kmalloc(x))
#define STBTT_free(x,u)    ((void)(u),kfree(x))

#define STBTT_assert

#include "nuklear.h"
#include "nuklear_rawfb.h"

#include "../images/wallpaper.h"

unsigned char tex_scratch[512 * 512];
struct rawfb_context *rawfb;

#define INPUT_TEXT_MAX 256

uint32_t text_length = 0;
char text[INPUT_TEXT_MAX];

static void open_app_pad()
{
    if (nk_popup_begin(&rawfb->ctx, NK_POPUP_STATIC, "Note", NK_WINDOW_CLOSABLE, nk_rect(20,100, 300,190)))
    {
        nk_layout_row_dynamic(&rawfb->ctx, 20, 1);
        nk_label(&rawfb->ctx, "There is no filesystem", NK_TEXT_LEFT);
        nk_label(&rawfb->ctx, "So no save or load", NK_TEXT_LEFT);
        nk_popup_end(&rawfb->ctx);
    }

    /*if (nk_begin(ctx, "Pad", nk_rect(50,50, 250,250),
    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_TITLE)){
        char buf[256] = {0};
        nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, buf, sizeof(buf) - 1, nk_filter_default);
    }
    nk_end(ctx);*/
}

static void open_app_menu()
{
    if (nk_begin(&rawfb->ctx, "Start", nk_rect(0,768-450, 200,450),
    NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_MINIMIZABLE)){
        nk_layout_row_static(&rawfb->ctx, 20, 150, 1);
        if (nk_button_label(&rawfb->ctx, "Pad"))
            open_app_pad();
    }
    nk_end(&rawfb->ctx);
}

void ui_initialize(void)
{
    rawfb = nk_rawfb_init(graphics_get_backbuffer(), tex_scratch, lfb_get_width(), lfb_get_height(), lfb_get_width() * 4, PIXEL_LAYOUT_XRGB_8888);
}

void ui_shutdown(void)
{
    nk_rawfb_shutdown(rawfb);
}

void ui_char_event(char ch)
{
	if (text_length < INPUT_TEXT_MAX)
		text[text_length++] = ch;
}

void ui_key_event(int32_t scancode)
{

}

void ui_render(void)
{
	struct nk_context *ctx = &rawfb->ctx;
    nk_input_begin(ctx);
    int32_t mouse_x = mouse_get_x();
    int32_t mouse_y = mouse_get_y();
    mouse_status_t mouse_status = mouse_get_status();
    nk_input_motion(ctx, mouse_x, mouse_y);
    nk_input_button(ctx, NK_BUTTON_LEFT, mouse_x, mouse_y, mouse_status.left_button);
    nk_input_button(ctx, NK_BUTTON_MIDDLE, mouse_x, mouse_y, mouse_status.middle_button);
    nk_input_button(ctx, NK_BUTTON_RIGHT, mouse_x, mouse_y, mouse_status.right_button);

    for (int32_t i = 0; i < text_length; ++i)
        nk_input_unicode(ctx, text[i]);

    nk_input_key(ctx, NK_KEY_ENTER, keyboard_get_scancode(SCAN_CODE_KEY_ENTER));
    //nk_input_key(ctx, NK_KEY_TAB, keyboard_get_scancode(SCAN_CODE_KEY_TAB));
    nk_input_key(ctx, NK_KEY_BACKSPACE, keyboard_get_scancode(SCAN_CODE_KEY_BACKSPACE));
    nk_input_key(ctx, NK_KEY_UP, keyboard_get_scancode(SCAN_CODE_KEY_UP));
    nk_input_key(ctx, NK_KEY_DOWN, keyboard_get_scancode(SCAN_CODE_KEY_DOWN));
    nk_input_key(ctx, NK_KEY_TEXT_START, keyboard_get_scancode(SCAN_CODE_KEY_HOME));
    nk_input_key(ctx, NK_KEY_TEXT_END, keyboard_get_scancode(SCAN_CODE_KEY_END));
    nk_input_key(ctx, NK_KEY_SCROLL_START, keyboard_get_scancode(SCAN_CODE_KEY_HOME));
    nk_input_key(ctx, NK_KEY_SCROLL_END, keyboard_get_scancode(SCAN_CODE_KEY_END));
    nk_input_key(ctx, NK_KEY_SCROLL_DOWN, keyboard_get_scancode(SCAN_CODE_KEY_PAGE_DOWN));
    nk_input_key(ctx, NK_KEY_SCROLL_UP, keyboard_get_scancode(SCAN_CODE_KEY_PAGE_UP));
    nk_input_key(ctx, NK_KEY_SHIFT, keyboard_get_scancode(SCAN_CODE_KEY_LEFT_SHIFT)||
                                    keyboard_get_scancode(SCAN_CODE_KEY_RIGHT_SHIFT));

    if (keyboard_get_scancode(SCAN_CODE_KEY_LEFT_CTRL))
	{
        nk_input_key(ctx, NK_KEY_COPY, keyboard_get_scancode(SCAN_CODE_KEY_C));
        nk_input_key(ctx, NK_KEY_PASTE, keyboard_get_scancode(SCAN_CODE_KEY_V));
        nk_input_key(ctx, NK_KEY_CUT, keyboard_get_scancode(SCAN_CODE_KEY_X));
        nk_input_key(ctx, NK_KEY_TEXT_UNDO, keyboard_get_scancode(SCAN_CODE_KEY_Z));
        nk_input_key(ctx, NK_KEY_TEXT_REDO, keyboard_get_scancode(SCAN_CODE_KEY_R));
        nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, keyboard_get_scancode(SCAN_CODE_KEY_LEFT));
        nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, keyboard_get_scancode(SCAN_CODE_KEY_RIGHT));
        nk_input_key(ctx, NK_KEY_TEXT_LINE_START, keyboard_get_scancode(SCAN_CODE_KEY_B));
        nk_input_key(ctx, NK_KEY_TEXT_LINE_END, keyboard_get_scancode(SCAN_CODE_KEY_E));
        nk_input_key(ctx, NK_KEY_TEXT_SELECT_ALL, keyboard_get_scancode(SCAN_CODE_KEY_A));
    }
	else
	{
        //nk_input_key(ctx, NK_KEY_LEFT, keyboard_get_scancode(SCAN_CODE_KEY_LEFT));
        //nk_input_key(ctx, NK_KEY_RIGHT, keyboard_get_scancode(SCAN_CODE_KEY_RIGHT));
        nk_input_key(ctx, NK_KEY_COPY, 0);
        nk_input_key(ctx, NK_KEY_PASTE, 0);
        nk_input_key(ctx, NK_KEY_CUT, 0);
        nk_input_key(ctx, NK_KEY_SHIFT, 0);
    }

    nk_input_end(ctx);

	text_length = 0;

        /* GUI */
        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 500, 500),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
            NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE|NK_WINDOW_SCALABLE)) {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 80, 1);
            nk_button_label(ctx, "button");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
			static char buf[256] = {0};
			// in window
			nk_edit_string_zero_terminated (ctx, NK_EDIT_FIELD | NK_EDIT_MULTILINE, buf, sizeof(buf) - 1, nk_filter_default);
			if (nk_button_label (ctx, "Done"))
				printk("%s\n", buf);
        }
        nk_end(ctx);

        if (nk_begin(ctx, "EASD", nk_rect(50, 50, 500, 500),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
            NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE|NK_WINDOW_SCALABLE)) {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 80, 1);
            nk_button_label(ctx, "button");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
        }
        nk_end(ctx);
    
    open_app_menu();

    {
        uint32_t *wallpaper = g_wallpaper;
        uint32_t *back_buffer = graphics_get_backbuffer();
        uint32_t end_buffer = wallpaper + lfb_get_width() * lfb_get_height();
        for (; wallpaper < end_buffer; wallpaper++, back_buffer++)
            *back_buffer = *wallpaper;
    }

    nk_rawfb_render(rawfb, nk_rgb(0,0,0), 0);

    graphics_swapbuffers();
}