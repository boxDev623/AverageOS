#include "ui.h"

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

void ui_initialize(void)
{
    struct nk_context ctx;

    rawfb = nk_rawfb_init(graphics_get_backbuffer(), tex_scratch, lfb_get_width(), lfb_get_height(), lfb_get_width() * 4, PIXEL_LAYOUT_XRGB_8888);
}

void ui_shutdown(void)
{
    nk_rawfb_shutdown(rawfb);
}

void ui_render(void)
{
    nk_input_begin(&rawfb->ctx);
    int32_t mouse_x = mouse_get_x();
    int32_t mouse_y = mouse_get_y();
    mouse_status_t mouse_status = mouse_get_status();
    nk_input_motion(&rawfb->ctx, mouse_x, mouse_y);
    nk_input_button(&rawfb->ctx, NK_BUTTON_LEFT, mouse_x, mouse_y, mouse_status.left_button);
    nk_input_button(&rawfb->ctx, NK_BUTTON_MIDDLE, mouse_x, mouse_y, mouse_status.middle_button);
    nk_input_button(&rawfb->ctx, NK_BUTTON_RIGHT, mouse_x, mouse_y, mouse_status.right_button);
    nk_input_end(&rawfb->ctx);

        /* GUI */
        if (nk_begin(&rawfb->ctx, "Demo", nk_rect(50, 50, 500, 500),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
            NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(&rawfb->ctx, 30, 80, 1);
            nk_button_label(&rawfb->ctx, "button");
            nk_layout_row_dynamic(&rawfb->ctx, 30, 2);
            if (nk_option_label(&rawfb->ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(&rawfb->ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(&rawfb->ctx, 25, 1);
            nk_property_int(&rawfb->ctx, "Compression:", 0, &property, 100, 10, 1);
        }
        nk_end(&rawfb->ctx);

        if (nk_begin(&rawfb->ctx, "EASD", nk_rect(50, 50, 500, 500),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
            NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(&rawfb->ctx, 30, 80, 1);
            nk_button_label(&rawfb->ctx, "button");
            nk_layout_row_dynamic(&rawfb->ctx, 30, 2);
            if (nk_option_label(&rawfb->ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(&rawfb->ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(&rawfb->ctx, 25, 1);
            nk_property_int(&rawfb->ctx, "Compression:", 0, &property, 100, 10, 1);
        }
        nk_end(&rawfb->ctx);

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

void ui_draw_menu()
{
    if (mouse_get_status().left_button)
    {
        graphics_fillrect(mouse_get_x(), mouse_get_y(), 100, 300, lfb_rgb(255,255,255));
        graphics_drawstring("test", mouse_get_x()+10, mouse_get_y()+1, lfb_rgb(0,0,0));
    }
}