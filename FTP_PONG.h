/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_PONG.c, holds macros and function
            prototypes
    @since March 21, 2024
    @version Rev 4
**********************************************************************/
/* File FTP_PONG.h*/
#ifndef FTP_PONG_H
#define FTP_PONG_H

// Includes
#include <stdint.h>
#include "FTP_TYPES.h"

// Macros
#define PADDLE_WIDTH 8
#define PADDLE_HEIGHT 2
#define PONG_ORIENT 5
#define BALL_WIDTH 1
#define BALL_HEIGHT 1
#define PONG_SPEED 50
#define INPUT_MIN 2000
// #define PONG_SCALE 4
// #define PONG_WIDTH ((ScreenW/PONG_SCALE) - ((ScreenW % PONG_SCALE)/PONG_SCALE))
// #define PONG_HEIGHT ((ScreenH/PONG_SCALE) - ((ScreenH % PONG_SCALE)/PONG_SCALE))
#define PONG_SCALE 1
#define PONG_MIDWIDTH ((ScreenW/2) - ((ScreenW % 2)/2))
#define PONG_MIDHEIGHT ((ScreenH/2) - ((ScreenH % 2)/2))

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Important values for a pong ball to be built, including the current
            origin, the old origin, the bounding box of the ball and its current
            velocity.
    @typedef Ball
    @since March 21, 2024
**********************************************************************/
typedef struct Ball{
    s16Vector2 origin;
    s16Vector2 old_origin;
    u8Vector2 bounding_box;
    s8Vector2 velocity;
} Ball;

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Important values for a pong paddle to be built, including the current
            origin, the old origin and the bounding box of the paddle.
    @typedef Paddle
    @since March 21, 2024
**********************************************************************/
typedef struct Paddle{
    s16Vector2 origin;
    s16Vector2 old_origin;
    u8Vector2 bounding_box;
} Paddle;

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Important values for the Pong game to function, including the ball
            the player paddle, the ai paddle, the direction the player
            is trying to go, the gameover state and the game score
    @typedef Pong_GameState
    @since March 21, 2024
**********************************************************************/
typedef struct Pong_GameState{
    Ball ball;
    Paddle player_paddle;
    Paddle ai_paddle;
    uint8_t direction;
    uint8_t game_over;
    uint8_t score;
} Pong_GameState;

// Prototypes
void init_pong(Pong_GameState *game);

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

void pong_game_over(Pong_GameState *game);

uint8_t pong_main(void);

#endif /* File FTP_PONG.h*/
