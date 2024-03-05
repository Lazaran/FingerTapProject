#ifndef FTP_SNAKE_H
#define FTP_SNAKE_H

#include <stdint.h>
#include "FTP_TYPES.h"

// Number of pixels per square | IMPORTANT
#define SNAKEGAME_SCALE 10

// At snake scale of 10, width = 12, height = 16
#define WIDTH ((ST7735_TFTWIDTH/SNAKEGAME_SCALE) - ((ST7735_TFTWIDTH % SNAKEGAME_SCALE)/SNAKEGAME_SCALE))
#define HEIGHT ((ST7735_TFTHEIGHT/SNAKEGAME_SCALE) - ((ST7735_TFTHEIGHT % SNAKEGAME_SCALE)/SNAKEGAME_SCALE))
#define SNAKE_SPEED 100
#define SCORE_INCREMENT 2
#define Y_INCREMENT 1
#define X_INCREMENT 1
#define INPUT_MIN 2000

/* Members: Vector2 segments, Vector2 head, uint8_t length */
typedef struct {
    Vector2 segments[HEIGHT * WIDTH];
    Vector2 head;
    uint8_t length;
} Snake;

/* Members: Snake snake, Vector2 food, uint8_t direction, uint8_t score, uint8_t game_over */
typedef struct {
    Snake snake;
    Vector2 oldfood;
    Vector2 food;
    uint8_t direction;
    uint8_t score;
    uint8_t game_over;
} GameState;

void handle_input(GameState *game);

void move_head(GameState *game);

uint8_t check_self_collision(Snake *snake);

uint8_t check_wall_collision(Vector2 *head);

uint8_t check_food_collision(Vector2 *head, Vector2 *food);

void add_segment(GameState *game);

void move_body(GameState *game);

void render_snake(GameState *game, uint8_t draw_erase_toggle);

// void render_food(GameState *game);

// void render_score(GameState *game);

// void render_game(GameState *game);

void init_game(GameState *game);

void update_gamestate(GameState *game);

uint8_t snake_main(void);

#endif