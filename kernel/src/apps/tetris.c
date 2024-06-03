#include "text_editor.h"
#include "graphics/nuklear.h"
#include "graphics/ui.h"

#include "devices/ps2/keyboard.h"

#include <stdlib.h>
#include <string.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define TILE_SIZE 30
#define MAX_COLOR_VALUE 255
#define SHADOW_COLOR_MULTIPLIER 0.5

// Tetromino definitions
int TETROMINOS[7][4][4] = {
    { {1, 1, 1, 1} }, // I
    { {1, 1}, {1, 1} }, // O
    { {0, 1, 1}, {1, 1, 0} }, // S
    { {1, 1, 0}, {0, 1, 1} }, // Z
    { {1, 1, 1}, {0, 1, 0} }, // T
    { {1, 1, 1}, {1, 0, 0} }, // L
    { {1, 1, 1}, {0, 0, 1} }, // J
};

struct Color {
    int r, g, b, a;
};

struct Color COLORS[7] = {
    {0, 255, 255, 255}, // I
    {255, 255, 0, 255}, // O
    {0, 255, 0, 255}, // S
    {255, 0, 0, 255}, // Z
    {128, 0, 128, 255}, // T
    {255, 165, 0, 255}, // L
    {0, 0, 255, 255}, // J
};

static int board[BOARD_HEIGHT][BOARD_WIDTH];
static int current_piece[4][4];
static struct Color current_color;
static int current_x, current_y;
static int is_game_over = 0;
static double timer = 0;
static double speed = 0.5; // Time in seconds between each move

// Function prototypes
static void initialize_game();
static void new_piece();
static int check_collision(int piece[4][4], int x, int y);
static void lock_piece();
static void clear_lines();
static void rotate_piece();
static void handle_input();
static int get_shadow_position();
static void render_game(struct nk_context *ctx);

static void initialize_game() {
    memset(board, 0, sizeof(board));
    new_piece();
    is_game_over = 0;
    timer = 0;
}

static void new_piece() {
    int index = rand() % 7;
    memcpy(current_piece, TETROMINOS[index], sizeof(current_piece));
    current_color = COLORS[index];
    current_x = BOARD_WIDTH / 2 - 2;
    current_y = 0;
    if (check_collision(current_piece, current_x, current_y)) {
        is_game_over = 1;
    }
}

static int check_collision(int piece[4][4], int x, int y) {
    for (int py = 0; py < 4; ++py) {
        for (int px = 0; px < 4; ++px) {
            if (piece[py][px] != 0) {
                int bx = x + px;
                int by = y + py;
                if (bx < 0 || bx >= BOARD_WIDTH || by < 0 || by >= BOARD_HEIGHT || board[by][bx] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

static void lock_piece() {
    for (int py = 0; py < 4; ++py) {
        for (int px = 0; px < 4; ++px) {
            if (current_piece[py][px] != 0) {
                board[current_y + py][current_x + px] = 1; // Mark board position as filled
            }
        }
    }
    clear_lines();
    new_piece();
}

static void clear_lines() {
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        int full = 1;
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (board[y][x] == 0) {
                full = 0;
                break;
            }
        }
        if (full) {
            for (int ty = y; ty > 0; --ty) {
                memcpy(board[ty], board[ty - 1], sizeof(board[ty]));
            }
            memset(board[0], 0, sizeof(board[0]));
        }
    }
}

void rotate_piece() {
    int new_piece[4][4] = {0};
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            new_piece[x][y] = current_piece[3 - y][x];
        }
    }

    if (!check_collision(new_piece, current_x, current_y)) {
        memcpy(current_piece, new_piece, sizeof(current_piece));
    }
}

static float key_timer = 0.0f;
static bool rotate_pressed = false;
static bool space_pressed = false;

static void handle_input() {
    key_timer += 1.0f / 4.0f;
    if (keyboard_get_scancode(75) && key_timer > 1.0f) { // Left arrow
        if (!check_collision(current_piece, current_x - 1, current_y)) {
            current_x -= 1;
        }
        key_timer = 0.0f;
    } else if (keyboard_get_scancode(77) && key_timer > 1.0f) { // Right arrow
        if (!check_collision(current_piece, current_x + 1, current_y)) {
            current_x += 1;
        }
        key_timer = 0.0f;
    } else if (keyboard_get_scancode(80) && key_timer > 1.0f) { // Down arrow
        if (!check_collision(current_piece, current_x, current_y + 1)) {
            current_y += 1;
        } else {
            lock_piece();
        }
        key_timer = 0.0f;
    } else if (keyboard_get_scancode(72) && !rotate_pressed) { // Up arrow
        rotate_piece();
        rotate_pressed = true;
    } else if (keyboard_get_scancode(57) && !space_pressed) { // Space bar
        current_y = get_shadow_position();
        lock_piece();
        space_pressed = true;
    }

    if (!keyboard_get_scancode(72))
        rotate_pressed = false;

    if (!keyboard_get_scancode(57))
        space_pressed = false;
}

static int get_shadow_position() {
    int shadow_y = current_y;
    while (!check_collision(current_piece, current_x, shadow_y + 1)) {
        shadow_y += 1;
    }
    return shadow_y;
}

static void render_game(struct nk_context *ctx) {
    struct nk_command_buffer *cmd_buffer = nk_window_get_canvas(ctx);
    struct nk_vec2 win_size = nk_widget_position(ctx);

    // Draw the board
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (board[y][x] != 0) {
                struct nk_rect rect = nk_rect(win_size.x + x * TILE_SIZE, win_size.y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                nk_fill_rect(cmd_buffer, rect, 0, nk_rgba(255, 255, 255, 255));
            }
        }
    }

    // Draw the shadow piece
    int shadow_y = get_shadow_position();
    struct nk_color shadow_color = {
        current_color.r * SHADOW_COLOR_MULTIPLIER,
        current_color.g * SHADOW_COLOR_MULTIPLIER,
        current_color.b * SHADOW_COLOR_MULTIPLIER,
        current_color.a * SHADOW_COLOR_MULTIPLIER
    };
    for (int py = 0; py < 4; ++py) {
        for (int px = 0; px < 4; ++px) {
            if (current_piece[py][px] != 0) {
                struct nk_rect rect = nk_rect(win_size.x + (current_x + px) * TILE_SIZE, win_size.y + (shadow_y + py) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                nk_fill_rect(cmd_buffer, rect, 0, nk_rgba(100, 100, 100, 100));
            }
        }
    }

    // Draw the current piece
    for (int py = 0; py < 4; ++py) {
        for (int px = 0; px < 4; ++px) {
            if (current_piece[py][px] != 0) {
                struct nk_rect rect = nk_rect(win_size.x + (current_x + px) * TILE_SIZE, win_size.y + (current_y + py) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                nk_fill_rect(cmd_buffer, rect, 0, nk_rgba(255, 255, 255, 255));
            }
        }
    }
}

static void update_event(struct nk_context *ctx) {
    if (is_game_over) {
        initialize_game();
        return;
    }

    if (nk_window_is_active(nk_ctx, "Tetris"))
        handle_input();

    timer += 1.0 / 60.0; // Assuming 60 FPS
    if (timer >= speed) {
        if (!check_collision(current_piece, current_x, current_y + 1)) {
            current_y += 1;
        } else {
            lock_piece();
        }
        timer = 0;
    }

    render_game(ctx);
}

bool enable_app_tetris = false;

void app_tetris_intialize(void)
{
    initialize_game();
}

void app_tetris_update(void)
{
    if (nk_begin(nk_ctx, "Tetris", nk_rect(50, 50, 340, 660), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_NO_SCROLLBAR))
    {
        int x, y;
        struct nk_rect bounds = nk_window_get_content_region(nk_ctx);
        x = (int)bounds.w;
        y = (int)bounds.h;
        nk_layout_row_static(nk_ctx, y, x, 1);
        update_event(nk_ctx);
    }
    nk_end(nk_ctx);

    if (nk_window_is_hidden(nk_ctx, "Tetris"))
        enable_app_tetris = false;
}