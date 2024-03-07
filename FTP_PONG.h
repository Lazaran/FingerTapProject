
#ifndef FTP_PONG_H
#define FTP_PONG_H

#include <stdint.h>
#include "FTP_TYPES.h"

#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 2

#define BALL_WIDTH 1
#define BALL_HEIGHT 1

#define PONG_SPEED 50

#define INPUT_MIN 2000
#define TETRIS_SCALE 4
#define TETRIS_WIDTH ((ST7735_TFTWIDTH/TETRIS_SCALE) - ((ST7735_TFTWIDTH % TETRIS_SCALE)/TETRIS_SCALE))
#define TETRIS_HEIGHT ((ST7735_TFTHEIGHT/TETRIS_SCALE) - ((ST7735_TFTHEIGHT % TETRIS_SCALE)/TETRIS_SCALE))
#define TETRIS_MIDWIDTH ((TETRIS_WIDTH/2) - ((TETRIS_WIDTH % 2)/2))
#define TETRIS_MIDHEIGHT ((TETRIS_HEIGHT/2) - ((TETRIS_HEIGHT % 2)/2))

/* Members: uVector2 origin, uVector2 old_origin, uVector2 bounding_box, sVector2 velocity*/
typedef struct Ball{
    sVector2 origin;
    sVector2 old_origin;
    sVector2 bounding_box;
    sVector2 velocity;
} Ball;

/* Members: uVector2 origin, uVector2 old_origin, uVector2 bounding_box*/
typedef struct Paddle{
    sVector2 origin;
    sVector2 old_origin;
    sVector2 bounding_box;
} Paddle;

/* Members: Ball ball, Paddle player_paddle, Paddle ai_paddle, uint8_t player_direction, uint8_t game_over, uint8_t score*/
typedef struct Pong_GameState{
    Ball ball;
    Paddle player_paddle;
    Paddle ai_paddle;
    uint8_t player_direction;
    uint8_t game_over;
    uint8_t score;
} Pong_GameState;

void pong_init(Pong_GameState *game);

void pong_input(Pong_GameState *game);

void move_ball(Pong_GameState *game);

uint8_t ball_sides_collision(Ball *ball);

uint8_t ball_ends_collision(Ball *ball);

uint8_t ball_player_collision(Paddle *paddle, Ball *ball);

uint8_t ball_ai_collision(Paddle *paddle, Ball *ball);

void render_ball(Ball *ball);

void update_ball(Pong_GameState *game);

void move_player_paddle(Pong_GameState *game);

void move_ai_paddle(Pong_GameState *game);

uint8_t paddle_sides_collision(Paddle *paddle);

void render_paddle(Paddle *paddle);

void update_player_paddle(Pong_GameState *game);

void update_ai_paddle(Pong_GameState *game);

uint8_t pong_main(void);

#endif


