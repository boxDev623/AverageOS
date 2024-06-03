#include "snake.h"
#include "graphics/nuklear.h"
#include "graphics/ui.h"

#include <stdlib.h>

bool enable_app_snake = false;

#define TILE_SIZE 20
#define BOARD_WIDTH 23
#define BOARD_HEIGHT 23
#define INITIAL_SNAKE_LENGTH 5

struct Position {
    int x, y;
};

struct Snake {
    struct Position segments[BOARD_WIDTH * BOARD_HEIGHT];
    int length;
    char direction;
};

static struct Position food;
static int is_game_over = 0;
static float timer = 0;
static float speed = 0.1f;
static struct Snake snake;

static void place_food() {
    food.x = rand() % (BOARD_WIDTH - 2) + 1;
    food.y = rand() % (BOARD_HEIGHT - 2) + 1;
}

static void initialize_game() {
    snake.length = INITIAL_SNAKE_LENGTH;
    for (int i = 0; i < INITIAL_SNAKE_LENGTH; ++i) {
        snake.segments[i].x = INITIAL_SNAKE_LENGTH - i;
        snake.segments[i].y = 1;
    }
    snake.direction = 'R';
    place_food();
    is_game_over = 0;
    timer = 0;
}

static int check_collision(struct Position new_head) {
    if (new_head.x < 1 || new_head.x > BOARD_WIDTH || new_head.y < 1 || new_head.y > BOARD_HEIGHT) {
        return 1;
    }
    for (int i = 0; i < snake.length; ++i) {
        if (snake.segments[i].x == new_head.x && snake.segments[i].y == new_head.y) {
            return 1;
        }
    }
    return 0;
}

static void update_snake() {
    struct Position head = snake.segments[0];
    struct Position new_head = head;

    switch (snake.direction) {
        case 'L': new_head.x--; break;
        case 'R': new_head.x++; break;
        case 'U': new_head.y--; break;
        case 'D': new_head.y++; break;
    }

    if (check_collision(new_head)) {
        is_game_over = 1;
        return;
    }

    for (int i = snake.length; i > 0; --i) {
        snake.segments[i] = snake.segments[i - 1];
    }
    snake.segments[0] = new_head;
    snake.length++;

    if (new_head.x == food.x && new_head.y == food.y) {
        place_food();
    } else {
        snake.length--;
    }
}

static void handle_input(struct nk_context *ctx) {
    if (nk_input_is_key_pressed(&ctx->input, NK_KEY_UP)) {
        if (snake.direction != 'D') snake.direction = 'U';
    }
    if (nk_input_is_key_pressed(&ctx->input, NK_KEY_DOWN)) {
        if (snake.direction != 'U') snake.direction = 'D';
    }
    if (nk_input_is_key_pressed(&ctx->input, NK_KEY_LEFT)) {
        if (snake.direction != 'R') snake.direction = 'L';
    }
    if (nk_input_is_key_pressed(&ctx->input, NK_KEY_RIGHT)) {
        if (snake.direction != 'L') snake.direction = 'R';
    }
}

static void render_game(struct nk_context *ctx) {
    struct nk_command_buffer *canvas = nk_window_get_canvas(ctx);
    struct nk_rect total_space = nk_window_get_content_region(ctx);

    for (int i = 0; i < snake.length; ++i) {
        struct Position segment = snake.segments[i];
        nk_fill_rect(canvas, nk_rect(total_space.x + (segment.x - 1) * TILE_SIZE, 
                                     total_space.y + (segment.y - 1) * TILE_SIZE, 
                                     TILE_SIZE, TILE_SIZE), 0, nk_rgb(0, 255, 0));
    }

    nk_fill_rect(canvas, nk_rect(total_space.x + (food.x - 1) * TILE_SIZE, 
                                 total_space.y + (food.y - 1) * TILE_SIZE, 
                                 TILE_SIZE, TILE_SIZE), 0, nk_rgb(255, 0, 0));
}

void app_snake_initialize(void)
{
    initialize_game();
}

void app_snake_update(void)
{
    if (is_game_over)
        initialize_game();

    if (nk_window_is_active(nk_ctx, "Snake"))
        handle_input(nk_ctx);

    timer += 1.0f / 60.0f;
    if (timer >= speed)
    {
        update_snake();
        timer = 0;
    }

    if (nk_begin(nk_ctx, "Snake", nk_rect(50, 50, 500, 500), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE))
    {
        int x, y;
        struct nk_rect bounds = nk_window_get_content_region(nk_ctx);
        x = (int)bounds.w;
        y = (int)bounds.h;
        nk_layout_row_static(nk_ctx, y, x, 1);
        render_game(nk_ctx);
    }
    nk_end(nk_ctx);

    if (nk_window_is_hidden(nk_ctx, "Snake"))
        enable_app_snake = false;
}