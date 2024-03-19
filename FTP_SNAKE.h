#ifndef FTP_SNAKE_H
#define FTP_SNAKE_H

#include <stdint.h>
#include "FTP_TYPES.h"
#include "FTP_ST7735R.h"

// Number of pixels per square | IMPORTANT
#define SNAKEGAME_SCALE 10

// At snake scale of 10, width = 12, height = 16
#define SNAKEWIDTH ((ST7735_TFTWIDTH/SNAKEGAME_SCALE) - ((ST7735_TFTWIDTH % SNAKEGAME_SCALE)/SNAKEGAME_SCALE))
#define SNAKEHEIGHT ((ST7735_TFTHEIGHT/SNAKEGAME_SCALE) - ((ST7735_TFTHEIGHT % SNAKEGAME_SCALE)/SNAKEGAME_SCALE))
#define SNAKE_SPEED 100
#define SCORE_INCREMENT 2
#define Y_INCREMENT 1
#define X_INCREMENT 1
#define INPUT_MIN 2000

/* SNAKE */
/* Members: Vector2 segments, Vector2 head, uint8_t length */
typedef struct Snake{
    s16Vector2 segments[SNAKEWIDTH * SNAKEHEIGHT];
    s16Vector2 head;
    uint8_t length;
} Snake;

/* Members: Snake snake, Vector2 food, uint8_t direction, uint8_t score, uint8_t game_over */
typedef struct Snake_GameState{
    Snake snake;
    u8Vector2 oldfood;
    u8Vector2 food;
    uint8_t direction;
    uint8_t game_over;
    uint8_t score;
} Snake_GameState;


void snake_input(Snake_GameState *game);

void move_head(Snake_GameState *game);

uint8_t check_self_collision(Snake *snake);

uint8_t check_wall_collision(s16Vector2 *head);

uint8_t check_food_collision(s16Vector2 *head, u8Vector2 *food);

void move_body(Snake_GameState *game);

void render_snake(Snake_GameState *game, uint8_t draw_erase_toggle);

uint8_t check_food_validity(Snake_GameState *game);

void init_snake(Snake_GameState *game);

void update_gamestate(Snake_GameState *game);

void snake_game_over(Snake_GameState *game);

uint8_t snake_main(void);

#endif