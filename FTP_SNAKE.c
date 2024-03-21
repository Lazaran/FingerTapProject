/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief An emulation of the classic Snake game. Move the snake to eat the red
            food, don't run into your own body or the sides of the screen or its
            game over!
    @since March 17, 2024
    @version Rev 4
**********************************************************************/

#include <stdint.h>
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_SNAKE.h"

/* Text to format the displayed score with */
char SnakeGameScore[] = "Score:";
char SnakeGameOver[] = "Game Over!";
char SnakeYourScoreIs[] = "Your Score Is:";

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Calls Circuit_Parse to get the current input code and sets
            movement direction of snake accordingly
    @note 1 = Up, 2 = Right, 3 = Down, 4 = Left
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void snake_input(Snake_GameState *game) {
    switch(Circuit_Parse()){
        // Index PA
        case 12:
            if (game->direction != 3){
                game->direction = 1;
            }
            return;
        // Index MA
        case 13:
            if (game->direction != 4){
                game->direction = 2;
            }
            return;
        // Middle PA
        case 22:
            if (game->direction != 1){
                game->direction = 3;
            }
            return;
        // Middle MA
        case 23:
            if (game->direction != 2){
                game->direction = 4;
            }
            return;
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
    @brief Moves the head to a new position based on input snake direction
    @note 0 = Up, 1 = Right, 2 = Down, 3 = Left
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void move_head(Snake_GameState *game) {
    switch(game->direction){
        case 1:
            game->snake.head.y -= Y_INCREMENT;
            return;
        case 2:
            game->snake.head.x += X_INCREMENT;
            return;
        case 3:
            game->snake.head.y += Y_INCREMENT;
            return;
        case 4:
            game->snake.head.x -= X_INCREMENT;
            return;
        default:
            break;
    };
    return;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Checks the position of the snake head against the position of each
            body segement to see if the snake has collided with itself
  @param snake A Snake that contains the positions of all segments and the length of the snake
  @since March 4, 2024
**********************************************************************/
uint8_t check_self_collision(Snake *snake) {
    for (int i = 1; i < snake->length; i++) {
        if (snake->segments[0].x == snake->segments[i].x && snake->segments[0].y == snake->segments[i].y) {
            return 1;
        };
    };
    return 0;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Checks if the snake has collided with a wall
  @note Conditionals might have to be modified to keep it in the right place
  @param head A Vector2 with the position of the snakes head
  @since March 4, 2024
**********************************************************************/
uint8_t check_wall_collision(s16Vector2 *head) {
    return (head->x < 0 || head->x >= SNAKEWIDTH || head->y < 0 || head->y >= SNAKEHEIGHT);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks if the snake has collided with food
    @note Conditionals might have to be modified to keep it in the right place
    @param head A Vector2 with the position of the snakes head
    @param food A Vector2 with the position of the food
    @since March 4, 2024
**********************************************************************/
uint8_t check_food_collision(s16Vector2 *head, u8Vector2 *food) {
    return (head->x == food->x && head->y == food->y);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Moves each element in the segments array one index to the right
            Most importantly it moves the head position into the segments 
            so the head can move somewhere new
    @note I feel like the length conditional might be wrong
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void move_body(Snake_GameState *game) {
    for (int i = game->snake.length - 1; i > 0; i--){
        game->snake.segments[i] = game->snake.segments[i - 1];
    };
    game->snake.segments[0] = game->snake.head;
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Draws the snake body and head to the screen
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void render_snake(Snake_GameState *game, uint8_t draw_erase_toggle) {
    // Toggle is 1, draw the snake
    if (draw_erase_toggle){
        for (int i = 0; i < game->snake.length; i++){
            d_Rect((game->snake.segments[i].x*SNAKEGAME_SCALE), (game->snake.segments[i].y*SNAKEGAME_SCALE), SNAKEGAME_SCALE, SNAKEGAME_SCALE, 2, ST7735_GRAY, 1, ST7735_WHITE);
        };
    // Toggle is 0, erase the snake
    } else if (!draw_erase_toggle){
        for (int i = 0; i < game->snake.length; i++){
            r_Rect((game->snake.segments[i].x*SNAKEGAME_SCALE), (game->snake.segments[i].y*SNAKEGAME_SCALE), SNAKEGAME_SCALE, SNAKEGAME_SCALE, ST7735_BLACK);
        };
    }
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initializes the Snake_GameState
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void init_snake(Snake_GameState *game) {
    game->snake.length = 1;
    game->snake.head.x = SNAKEWIDTH / 2;
    game->snake.head.y = SNAKEHEIGHT / 2;
    game->snake.segments[0].x = game->snake.head.x;
    game->snake.segments[0].y = game->snake.head.y;
    game->food.x = rand() % SNAKEWIDTH;
    game->food.y = rand() % SNAKEHEIGHT;
    game->score = 0;
    game->game_over = 0;
    d_Rect((game->food.x*SNAKEGAME_SCALE),(game->food.y*SNAKEGAME_SCALE),SNAKEGAME_SCALE,SNAKEGAME_SCALE, 2, ST7735_NEONGREEN, 1, ST7735_GREEN);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Move the head, check for collisions, move the body, render food, score and snake to screen
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void update_snake_gamestate(Snake_GameState *game) {
    // Move the head of the snake based on user inputs
    move_head(game);
    
    // Check for collisions with wall or the snake itself
    if (check_self_collision(&game->snake) || check_wall_collision(&game->snake.head)) {
        game->game_over = 1;
        return;
    };

    // Check for collision with the food
    if (check_food_collision(&game->snake.segments[0], &game->food)) {
        
        // Erase the food
        r_Rect((game->food.x*SNAKEGAME_SCALE),(game->food.y*SNAKEGAME_SCALE),
                SNAKEGAME_SCALE,SNAKEGAME_SCALE,ST7735_BLACK);

        // Increase the length of the snake
        game->snake.length += 1;

        // Increase the score
        game->score += SCORE_INCREMENT;
        
        // Randomly place food until it isn't generated inside the snake
        while (check_food_validity(game)){
            game->food.x = rand() % SNAKEWIDTH;
            game->food.y = rand() % SNAKEHEIGHT;
        }

        // Draw a new piece of food at a random location
        d_Rect((game->food.x*SNAKEGAME_SCALE),(game->food.y*SNAKEGAME_SCALE),
                SNAKEGAME_SCALE,SNAKEGAME_SCALE, 2, ST7735_NEONGREEN, 1, ST7735_GREEN);
    };

    // Print the current score to the screen
    format_text_dec(0,0,game->score,SnakeGameScore);

    // Erase the snake at its old position
    render_snake(game, 0);

    // Move the snake
    move_body(game);

    // Draw the snake at its new position
    render_snake(game, 1);

};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Check if the piece of food has a valid position by comparing
            it to each segement of the snake
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
uint8_t check_food_validity(Snake_GameState *game){
    uint8_t i = 0;
    for (i = 0; i < game->snake.length; i++){
        if (game->snake.segments[i].x == game->food.x && game->snake.segments[i].y == game->food.y){
            return 1;
        }
    }
    return 0;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Displays the GameOver text along with the users' score
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void snake_game_over(Snake_GameState *game){
    d_DrawString(5,4,SnakeGameOver,ST7735_WHITE);
    d_DrawString(3,5,SnakeYourScoreIs,ST7735_WHITE);
    format_dec_text(8,6,game->score," ");
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Get input, update the Snake_GameState and render game to screen
            returns 0 back to system main if game over
    @returns Game over exitcode
    @since March 4, 2024
**********************************************************************/
uint8_t snake_main(void) {
    if (ScreenOrientation != 5){
        ScreenOrientation = 5;
        setDirection(ScreenOrientation);
    }
    clearScreen(ST7735_BLACK);
    SysTick_Wait10ms(10);
    Snake_GameState SnakeGame;
    init_snake(&SnakeGame);
    SysTick_Wait10ms(10);
    d_Rect(0,0,ScreenW,ScreenH,2,ST7735_WHITE,0,ST7735_BLACK);
    while (!SnakeGame.game_over){

        snake_input(&SnakeGame);
        update_snake_gamestate(&SnakeGame);
        // Replacing this with a system timer would probably work A LOT BETTER
        // DO ITTTTTT
        SysTick_Wait10ms(200);
    };
    snake_game_over(&SnakeGame);
    // Replacing this with a system timer would probably work A LOT BETTER
    // DO ITTTTTT
    SysTick_Wait10ms(1000);
    // Snake Exitcode
    return 11;
}