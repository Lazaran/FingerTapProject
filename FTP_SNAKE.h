/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_SNAKE.c, holds macros and function
            prototypes
    @since March 21, 2024
    @version Rev 4
**********************************************************************/

/* File FTP_SNAKE.h*/
#ifndef FTP_SNAKE_H
#define FTP_SNAKE_H

// Includes
#include <stdint.h>
#include "FTP_TYPES.h"
#include "FTP_ST7735R.h"

// Macros
#define SNAKE_ORIENT 5
#define SNAKEGAME_SCALE 10
#define SEGMENTS_WIDTH ((ST7735_TFTWIDTH/SNAKEGAME_SCALE) - ((ST7735_TFTWIDTH % SNAKEGAME_SCALE)/SNAKEGAME_SCALE))
#define SEGMENTS_HEIGHT ((ST7735_TFTHEIGHT/SNAKEGAME_SCALE) - ((ST7735_TFTHEIGHT % SNAKEGAME_SCALE)/SNAKEGAME_SCALE))
#define SNAKEWIDTH ((ScreenW/SNAKEGAME_SCALE) - ((ScreenW% SNAKEGAME_SCALE)/SNAKEGAME_SCALE))
#define SNAKEHEIGHT ((ScreenH/SNAKEGAME_SCALE) - ((ScreenH % SNAKEGAME_SCALE)/SNAKEGAME_SCALE))
#define SNAKE_SPEED 100
#define SCORE_INCREMENT 2
#define Y_INCREMENT 1
#define X_INCREMENT 1
#define INPUT_MIN 2000

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Important values to build a snake, including the positions
            of each segment of its body, the position of the head of the
            snake and the current length of the snake
    @typedef Snake
    @since March 21, 2024
**********************************************************************/
typedef struct Snake{
    s16Vector2 segments[SEGMENTS_WIDTH * SEGMENTS_HEIGHT];
    s16Vector2 head;
    uint8_t length;
} Snake;

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Important values for the Snake Game to function, including
            the snake and its positions, the positions of the old and new
            food, the direction the player is trying to go, the gameover state
            and the game score
    @typedef Snake_GameState
    @since March 21, 2024
**********************************************************************/
typedef struct Snake_GameState{
    Snake snake;
    u8Vector2 oldfood;
    u8Vector2 food;
    uint8_t direction;
    uint8_t game_over;
    uint8_t score;
} Snake_GameState;

// Prototypes
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

#endif /* File FTP_SNAKE.h*/

