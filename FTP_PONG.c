//Standard Includes
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//FTP Includes
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_PONG.h"
#include "FTP_INPUT.h"

//External Lib Includes
#include "SysTick.h"
#include "tm4c123gh6pm.h"

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initialize a Pong_GameState
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void pong_init(Pong_GameState *game) {
  // Ball Init
  game->ball.origin.x = TETRIS_MIDWIDTH;
  game->ball.origin.y = TETRIS_MIDHEIGHT;
  game->ball.old_origin.x = game->ball.origin.x;
  game->ball.old_origin.y = game->ball.origin.y;
  game->ball.bounding_box.x = BALL_WIDTH;
  game->ball.bounding_box.y = BALL_HEIGHT;
  game->ball.velocity.x = 1;
  game->ball.velocity.y = -1;
  
  // Player Paddle Init
  game->player_paddle.origin.x = TETRIS_MIDWIDTH;
  game->player_paddle.origin.y = 3;
  game->player_paddle.old_origin.x = game->player_paddle.origin.x;
  game->player_paddle.old_origin.y = game->player_paddle.origin.y;
  game->player_paddle.bounding_box.x = PADDLE_WIDTH;
  game->player_paddle.bounding_box.y = PADDLE_HEIGHT;

  // AI Paddle Init
  game->ai_paddle.origin.x = TETRIS_MIDWIDTH;
  game->ai_paddle.origin.y = TETRIS_HEIGHT - 3;
  game->ai_paddle.old_origin.x = game->ai_paddle.origin.x;
  game->ai_paddle.old_origin.y = game->ai_paddle.origin.y;
  game->ai_paddle.bounding_box.x = PADDLE_WIDTH;
  game->ai_paddle.bounding_box.y = PADDLE_HEIGHT;

  // Other
  game->player_direction = 0;
  game->game_over = 0;
  game->score = 0;

  d_Rect(game->ball.origin.x*TETRIS_SCALE,game->ball.origin.y*TETRIS_SCALE,BALL_WIDTH*TETRIS_SCALE,BALL_HEIGHT*TETRIS_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);
  d_Rect(game->ai_paddle.origin.x*TETRIS_SCALE,game->ai_paddle.origin.y*TETRIS_SCALE,PADDLE_WIDTH*TETRIS_SCALE,PADDLE_HEIGHT*TETRIS_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);
  d_Rect(game->player_paddle.origin.x*TETRIS_SCALE,game->player_paddle.origin.y*TETRIS_SCALE,PADDLE_WIDTH*TETRIS_SCALE,PADDLE_HEIGHT*TETRIS_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);

}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for player input and sets the direction the player paddle will move
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void pong_input(Pong_GameState *game) {
  game->player_direction = 0;
  if (IndexCircuit > INPUT_MIN) {
    game->player_direction = 1;
  }
  if (MiddleCircuit > INPUT_MIN) {
    game->player_direction = 2;
  }
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Move the ball according to its velocity
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void move_ball(Pong_GameState *game) {
  game->ball.origin.x += game->ball.velocity.x;
  game->ball.origin.y += game->ball.velocity.y;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for a collision between the ball and the sides of the screen
    @returns 1 if collision, 0 if no collision
    @param ball A Ball of the ball
    @since March 6, 2024
**********************************************************************/
uint8_t ball_sides_collision(Ball *ball) {
  return (ball->origin.x < 0 || (ball->origin.x + ball->bounding_box.x) > TETRIS_WIDTH);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for a collision between the ball and the ends 
            of the screen behind the paddles
    @returns 1 if collision, 0 if no collision
    @param ball A Ball of the ball
    @since March 6, 2024
**********************************************************************/
uint8_t ball_ends_collision(Ball *ball) {
  return (ball->origin.y < 0 || (ball->origin.y + ball->bounding_box.y) > TETRIS_HEIGHT);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for a collision between the ball and the Player paddle
    @returns 1 if collision, 0 if no collision
    @param paddle A Paddle of the Player Paddle
    @param ball A Ball of the ball
    @since March 6, 2024
**********************************************************************/
uint8_t ball_player_collision(Paddle *paddle, Ball *ball) {
  return (ball->origin.y <= (paddle->origin.y + paddle->bounding_box.y) && 
          ball->origin.y >= paddle->origin.y && 
          ball->origin.x < (paddle->origin.x + paddle->bounding_box.x) &&
          (ball->origin.x + ball->bounding_box.x) > paddle->origin.x);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for a collision between the ball and the AI paddle
    @returns 1 if collision, 0 if no collision
    @param paddle A Paddle of the AI Paddle
    @param ball A Ball of the ball
    @since March 6, 2024
**********************************************************************/
uint8_t ball_ai_collision(Paddle *paddle, Ball *ball) {
  return ((ball->origin.y + ball->bounding_box.y) >= paddle->origin.y &&
          (ball->origin.y + ball->bounding_box.y) <= (paddle->origin.y + paddle->bounding_box.y) &&
          ball->origin.x < paddle->bounding_box.x &&
          ball->bounding_box.x > paddle->origin.x);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Erases the old ball and draws a new one if the new origin is
            not the same as the old origin
    @param ball A Ball to be drawn to the screen
    @since March 6, 2024
**********************************************************************/
void render_ball(Ball *ball) {
  if (ball->origin.x == ball->old_origin.x && ball->origin.y == ball->old_origin.y){
    return;
  }
  d_Rect(ball->old_origin.x*TETRIS_SCALE,ball->old_origin.y*TETRIS_SCALE,BALL_WIDTH*TETRIS_SCALE,BALL_HEIGHT*TETRIS_SCALE,3,ST7735_BLACK,0,ST7735_BLACK);
  d_Rect(ball->origin.x*TETRIS_SCALE,ball->origin.y*TETRIS_SCALE,BALL_WIDTH*TETRIS_SCALE,BALL_HEIGHT*TETRIS_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Update the ball by moving its origin, checking for collisions
            (possibly ending the game if colliding with ends), resetting origin 
            if collisions occured and rendering the ball to the screen, 
            and finally cementing the new origin into the old origin
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void update_ball(Pong_GameState *game) {
  move_ball(game);
  // Check bad collision
  if (ball_ends_collision(&game->ball)){
    game->game_over = 1;
  }
  // Check good collisions
  if (ball_sides_collision(&game->ball)){
    game->ball.origin.x = game->ball.old_origin.x;
    game->ball.velocity.x *= -1;
  }
  if (ball_ai_collision(&game->ai_paddle, &game->ball) ||
      ball_player_collision(&game->player_paddle, &game->ball)){
    game->ball.origin.y = game->ball.old_origin.y;
    game->ball.velocity.y *= -1;
  }
  render_ball(&game->ball);
  game->ball.old_origin = game->ball.origin;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Moves the player paddle based on player inputs
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void move_player_paddle(Pong_GameState *game) {
  switch(game->player_direction){
    case 1:
      game->player_paddle.origin.x += 1;
      break;
    case 2:
      game->player_paddle.origin.x -= 1;
      break;
  };
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Move AI paddle according to the origin of the ball
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void move_ai_paddle(Pong_GameState *game) {
  if (game->ball.origin.x > (game->ai_paddle.origin.x + (game->ai_paddle.bounding_box.x/2))){
    game->ai_paddle.origin.x += 1;
  } else {
    game->ai_paddle.origin.x -= 1;
  }
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for a collision with the side walls of the screen
    @returns 1 if collision, 0 if no collision
    @param paddle A Paddle to check
    @since March 6, 2024
**********************************************************************/
uint8_t paddle_sides_collision(Paddle *paddle) {
  return (paddle->origin.x < 0 || (paddle->origin.x + paddle->bounding_box.x) > TETRIS_WIDTH);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Erases the old paddle and draws a new one if the new origin is
            not the same as the old origin
    @param paddle A Paddle to be drawn to the screen
    @since March 6, 2024
**********************************************************************/
void render_paddle(Paddle *paddle) {
  if (paddle->origin.x == paddle->old_origin.x && paddle->origin.y == paddle->old_origin.y){
    return;
  }
  d_Rect(paddle->old_origin.x*TETRIS_SCALE,paddle->old_origin.y*TETRIS_SCALE,PADDLE_WIDTH*TETRIS_SCALE,PADDLE_HEIGHT*TETRIS_SCALE,3,ST7735_BLACK,0,ST7735_BLACK);
  d_Rect(paddle->origin.x*TETRIS_SCALE,paddle->origin.y*TETRIS_SCALE,PADDLE_WIDTH*TETRIS_SCALE,PADDLE_HEIGHT*TETRIS_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Update the Player paddle by moving its origin, checking for collisions
            resetting origin if collisions occured and rendering the paddle 
            to the screen, and finally cementing the new origin into the old origin
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void update_player_paddle(Pong_GameState *game) {
  move_player_paddle(game);
  if (paddle_sides_collision(&game->player_paddle)){
    game->player_paddle.origin = game->player_paddle.old_origin;
  } else {
    render_paddle(&game->player_paddle);
    game->player_paddle.old_origin = game->player_paddle.origin;
  }
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Update the AI paddle by moving its origin, checking for collisions
            resetting origin if collisions occured and rendering the paddle 
            to the screen, and finally cementing the new origin into the old origin
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void update_ai_paddle(Pong_GameState *game) {
  move_ai_paddle(game);
  if (paddle_sides_collision(&game->ai_paddle)){
    game->ai_paddle.origin = game->ai_paddle.old_origin;
  } else {
    render_paddle(&game->ai_paddle);
    game->ai_paddle.old_origin = game->ai_paddle.origin;
  }
}

char OriginTextX[] = "OriginX";
char OriginTextY[] = "OriginY";

char OldOriginTextX[] = "OldOriginX";
char OldOriginTextY[] = "OldOriginY";

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Get input and update game elements, waiting until gameover state
    @returns Game over exitcode
    @since March 6, 2024
**********************************************************************/
uint8_t pong_main(void) {
  Pong_GameState PongGame;
  clearScreen(ST7735_BLACK);
  pong_init(&PongGame);
  while(!PongGame.game_over){
    pong_input(&PongGame);
    update_ball(&PongGame);
    update_player_paddle(&PongGame);
    update_ai_paddle(&PongGame);
  };
  return 0;
}































































