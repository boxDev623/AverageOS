#include "snake.h"

bool enable_app_snake = false;
const char app_snake_code[] = 
"local TILE_SIZE = 20\n"
"local BOARD_WIDTH = 20\n"
"local BOARD_HEIGHT = 15\n"
"local INITIAL_SNAKE_LENGTH = 5\n"
"local DIRECTIONS = {\n"
    "left = { x = -1, y = 0 },\n"
    "right = { x = 1, y = 0 },\n"
    "up = { x = 0, y = -1 },\n"
    "down = { x = 0, y = 1 },\n"
"}\n"
"\n"
"local snake = {}\n"
"local snake_direction = \"right\"\n"
"local food = {}\n"
"local is_game_over = false\n"
"local timer = 0\n"
"local speed = 0.1\n"
"\n"
"local function place_food()\n"
    "food.x = math.random(1, BOARD_WIDTH - 2)\n"
    "food.y = math.random(1, BOARD_HEIGHT - 2)\n"
"end\n"
"\n"
"local function initialize_game()\n"
    "snake = {}\n"
    "for i = 1, INITIAL_SNAKE_LENGTH do\n"
        "table.insert(snake, { x = INITIAL_SNAKE_LENGTH - i + 1, y = 1 })\n"
    "end\n"
    "snake_direction = \"right\"\n"
    "place_food()\n"
    "is_game_over = false\n"
    "timer = 0\n"
"end\n"
"\n"
"local function check_collision(new_head)\n"
    "if new_head.x < 1 or new_head.x > BOARD_WIDTH or new_head.y < 1 or new_head.y > BOARD_HEIGHT then\n"
        "return true\n"
    "end\n"
    "for _, segment in ipairs(snake) do\n"
        "if segment.x == new_head.x and segment.y == new_head.y then\n"
            "return true\n"
        "end\n"
    "end\n"
    "return false\n"
"end\n"
"\n"
"local function update_snake()\n"
    "local head = snake[1]\n"
    "local new_head = {\n"
        "x = head.x + DIRECTIONS[snake_direction].x,\n"
        "y = head.y + DIRECTIONS[snake_direction].y,\n"
    "}\n"
"\n"
    "if check_collision(new_head) then\n"
        "is_game_over = true\n"
        "return\n"
    "end\n"
"\n"
    "table.insert(snake, 1, new_head)\n"
    "if new_head.x == food.x and new_head.y == food.y then\n"
        "place_food()\n"
    "else\n"
        "table.remove(snake)\n"
    "end\n"
"end\n"
"\n"
"local function handle_input()\n"
    "if os.is_key_pressed(72) then\n"
        "if snake_direction ~= \"down\" then\n"
            "snake_direction = \"up\"\n"
        "end\n"
    "elseif os.is_key_pressed(80) then\n"
        "if snake_direction ~= \"up\" then\n"
            "snake_direction = \"down\"\n"
        "end\n"
    "elseif os.is_key_pressed(75) then\n"
        "if snake_direction ~= \"right\" then\n"
            "snake_direction = \"left\"\n"
        "end\n"
    "elseif os.is_key_pressed(77) then\n"
        "if snake_direction ~= \"left\" then\n"
            "snake_direction = \"right\"\n"
        "end\n"
    "end\n"
"end\n"
"\n"
"local function render_game()\n"
    "wx, wy = nuklear.widget_get_position()\n"
    "local cmd_buffer = nuklear.get_canvas()\n"
    "for _, segment in ipairs(snake) do\n"
        "nuklear.fill_rect(cmd_buffer, { wx + (segment.x - 1) * TILE_SIZE, wy + (segment.y - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE }, 0, { 0, 255, 0, 255 })\n"
    "end\n"
    "nuklear.fill_rect(cmd_buffer, { wx + (food.x - 1) * TILE_SIZE, wy + (food.y - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE }, 0, { 255, 0, 0, 255 })\n"
"end\n"
"\n"
"initialize_game()\n"
"function __averageos_update_event__()\n"
    "if is_game_over then\n"
        "initialize_game()\n"
    "end\n"
    "if nuklear.window_is_active(\"Snake\") then\n"
        "handle_input()\n"
    "end\n"
    "timer = timer + 1 / 60\n"
    "if timer >= speed then\n"
        "update_snake()\n"
        "timer = 0\n"
    "end\n"
    "nuklear.begin_window(\"Snake\", 50, 50, 500, 500)\n"
    "if nuklear.window_is_collapsed(\"Snake\") == false then\n"
        "nuklear.layout_row_dynamic(nuklear.window_get_height(), 1)\n"
        "render_game()\n"
        "ww, wh = nuklear.widget_get_size()\n"
        "BOARD_WIDTH = ww / TILE_SIZE\n"
        "BOARD_HEIGHT = wh / TILE_SIZE\n"
    "end\n"
    "nuklear.end_window()\n"
    "if nuklear.window_is_closed(\"Snake\") == true then\n"
    	"os.close_proc(os.get_current_proc())\n"
    "end\n"
"end\n";
void app_snake()
{
    if (enable_app_snake)
    {
        appman_new_proc("SNAKE", app_snake_code);
        enable_app_snake = false;
    }
}