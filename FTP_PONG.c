/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief An emulation of the classic Atari game Pong. Move your paddle
            up and down to bounce the ball back to the AI. Careful! The AI
            is VERY GOOD XD
    @since March 17, 2024
    @version Rev 4
**********************************************************************/

// Includes
#include <stdint.h>
#include "SysTick.h"
#include "tm4c123gh6pm.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_PONG.h"

/* Text to format the displayed score with */
char PongGameScore[] = "Score:";
char PongGameOver[] = "Game Over!";
char PongYourScoreIs[] = "Your Score Is:";

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initialize a Pong_GameState
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void init_pong(Pong_GameState *game) {
  // Ball Init
  game->ball.origin.x = ScreenW /2;
  game->ball.origin.y = ScreenH /2;
  game->ball.old_origin.x = game->ball.origin.x;
  game->ball.old_origin.y = game->ball.origin.y;
  game->ball.bounding_box.x = BALL_WIDTH;
  game->ball.bounding_box.y = BALL_HEIGHT;
  game->ball.velocity.x = 1;
  game->ball.velocity.y = -1;
  
  // Player Paddle Init
  game->player_paddle.origin.x = 20;
  game->player_paddle.origin.y = 20;
  game->player_paddle.old_origin.x = game->player_paddle.origin.x;
  game->player_paddle.old_origin.y = game->player_paddle.origin.y;
  game->player_paddle.bounding_box.x = PADDLE_WIDTH;
  game->player_paddle.bounding_box.y = PADDLE_HEIGHT;

  // AI Paddle Init
  game->ai_paddle.origin.x = 140;
  game->ai_paddle.origin.y = 20;
  game->ai_paddle.old_origin.x = game->ai_paddle.origin.x;
  game->ai_paddle.old_origin.y = game->ai_paddle.origin.y;
  game->ai_paddle.bounding_box.x = PADDLE_WIDTH;
  game->ai_paddle.bounding_box.y = PADDLE_HEIGHT;

  // Other
  game->direction = 0;
  game->game_over = 0;
  game->score = 0;

  d_Rect(game->ball.origin.x*PONG_SCALE,game->ball.origin.y*PONG_SCALE,BALL_WIDTH*PONG_SCALE,BALL_HEIGHT*PONG_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);
  d_Rect(game->ai_paddle.origin.x*PONG_SCALE,game->ai_paddle.origin.y*PONG_SCALE,PADDLE_WIDTH*PONG_SCALE,PADDLE_HEIGHT*PONG_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);
  d_Rect(game->player_paddle.origin.x*PONG_SCALE,game->player_paddle.origin.y*PONG_SCALE,PADDLE_WIDTH*PONG_SCALE,PADDLE_HEIGHT*PONG_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);

}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for player input and sets the direction the player paddle will move
    @note 2 = Up, 3 = Down
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void pong_input(Pong_GameState *game) {
    switch(Circuit_Parse()){
        // Index PA
        case 12:
            game->direction = 2;
            return;
        // Index MA
        case 13:
            game->direction = 3;
            return;
        // Pinky DP
        case 45:
            game->game_over = 1;
            return;
        default:
            break;
    };
    return;
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
  return (ball->origin.y <= 0 || (ball->origin.y + ball->bounding_box.x) >= ScreenH);
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
  return (ball->origin.x <= 0 || (ball->origin.x + ball->bounding_box.y) >= ScreenW);
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
  return (ball->origin.x <= (paddle->origin.x + paddle->bounding_box.x) && 
          ball->origin.x >= paddle->origin.x && 
          ball->origin.y < (paddle->origin.y + paddle->bounding_box.y) &&
          (ball->origin.y + ball->bounding_box.y) > paddle->origin.y);
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
  return ((ball->origin.x + ball->bounding_box.x) >= paddle->origin.x &&
          (ball->origin.x + ball->bounding_box.x) <= (paddle->origin.x + paddle->bounding_box.x) &&
          ball->origin.y < paddle->bounding_box.y &&
          ball->bounding_box.y > paddle->origin.y);
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
  d_Rect(ball->old_origin.x*PONG_SCALE,ball->old_origin.y*PONG_SCALE,BALL_WIDTH*PONG_SCALE,BALL_HEIGHT*PONG_SCALE,3,ST7735_BLACK,0,ST7735_BLACK);
  d_Rect(ball->origin.x*PONG_SCALE,ball->origin.y*PONG_SCALE,BALL_WIDTH*PONG_SCALE,BALL_HEIGHT*PONG_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);
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
    game->ball.origin.y = game->ball.old_origin.y;
    game->ball.velocity.y *= -1;
  }
  if (ball_ai_collision(&game->ai_paddle, &game->ball) ||
      ball_player_collision(&game->player_paddle, &game->ball)){
    game->ball.origin.x = game->ball.old_origin.x;
    game->ball.velocity.x *= -1;
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
  switch(game->direction){
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
  return (paddle->origin.x < 0 || (paddle->origin.x + paddle->bounding_box.x) > ScreenW);
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
  d_Rect(paddle->old_origin.x*PONG_SCALE,paddle->old_origin.y*PONG_SCALE,PADDLE_WIDTH*PONG_SCALE,PADDLE_HEIGHT*PONG_SCALE,3,ST7735_BLACK,0,ST7735_BLACK);
  d_Rect(paddle->origin.x*PONG_SCALE,paddle->origin.y*PONG_SCALE,PADDLE_WIDTH*PONG_SCALE,PADDLE_HEIGHT*PONG_SCALE,3,ST7735_WHITE,0,ST7735_BLACK);
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

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Displays the GameOver text along with the users' score
    @param game A Pong_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void pong_game_over(Pong_GameState *game){
    d_DrawString(5,4,PongGameOver,ST7735_WHITE);
    d_DrawString(3,5,PongYourScoreIs,ST7735_WHITE);
    format_dec_text(8,6,game->score," ");
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Get input, update Pong GameState and render game objects to screen
    @returns Game Over exitcode
    @since March 6, 2024
**********************************************************************/
uint8_t pong_main(void) {
    // Orient the screen
    if (ScreenOrientation != PONG_ORIENT){
        ScreenOrientation = PONG_ORIENT;
        setDirection(ScreenOrientation);
    }  
    // Clear the screen
    clearScreen(ST7735_BLACK);
    // under_construction();
    // Initialize the PongGame GameState
    Pong_GameState PongGame;
    init_pong(&PongGame);
    // Render screen border
    d_Rect(0,0,ScreenW,ScreenH,2,ST7735_WHITE,0,ST7735_BLACK);
    // Application superloop
    while(!PongGame.game_over){
        // Get user inputs
        pong_input(&PongGame);
        // Update game object positions
        update_ball(&PongGame);
        update_player_paddle(&PongGame);
        update_ai_paddle(&PongGame);
    };
    // Display gameover screen
    pong_game_over(&PongGame);
    // Wait for 15 seconds
    SysTick_Wait10ms(1500);
    // Pong Exitcode
    return 12;
}































































