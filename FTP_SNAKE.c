#include <stdint.h>
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_SNAKE.h"

/* Text to format the displayed score with */
char GameScoreText[] = "Score:";

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks raw finger circuit values and sets snake direction
    @note 0 = Up, 1 = Right, 2 = Down, 3 = Left
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void handle_input(Snake_GameState *game) {
    if (IndexCircuit > INPUT_MIN){
        game->direction = 0;
    };
    if (MiddleCircuit > INPUT_MIN){
        game->direction = 1;
    };
    if (RingCircuit > INPUT_MIN){
        game->direction = 2;
    };
    if (PinkyCircuit > INPUT_MIN){
        game->direction = 3;
    };
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Moves the head to a new position based on direction input from the player
    @note 0 = Up, 1 = Right, 2 = Down, 3 = Left
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void move_head(Snake_GameState *game) {
    switch(game->direction){
        case 0:
            game->snake.head.y -= Y_INCREMENT;
            break;
        case 1:
            game->snake.head.x += X_INCREMENT;
            break;
        case 2:
            game->snake.head.y += Y_INCREMENT;
            break;
        case 3:
            game->snake.head.x -= X_INCREMENT;
            break;
    };
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Checks if the snake has collided with food
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
uint8_t check_wall_collision(Vector2 *head) {
    return (head->x < 0 || head->x >= WIDTH || head->y < 0 || head->y >= HEIGHT);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks if the snake has collided with food
    @note Conditionals might have to be modified to keep it in the right place
    @param head A Vector2 with the position of the snakes head
    @param food A Vector2 with the position of the food
    @since March 4, 2024
**********************************************************************/
uint8_t check_food_collision(Vector2 *head, Vector2 *food) {
    return (head->x == food->x && head->y == food->y);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Adds a segment to the snakes' body
    @note Might be this simple, might need more to shift the elements specially
            Don't think so but not sure so need to check back.
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void add_segment(Snake_GameState *game) {
    game->snake.length += 1;
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
    @brief Draws the snakes' body and head to the screen
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void render_snake(Snake_GameState *game, uint8_t draw_erase_toggle) {
    if (draw_erase_toggle){
        for (int i = 0; i < game->snake.length; i++){
            d_Rect((game->snake.segments[i].x*SNAKEGAME_SCALE), (game->snake.segments[i].y*SNAKEGAME_SCALE), SNAKEGAME_SCALE, SNAKEGAME_SCALE, 2, ST7735_GRAY, 1, ST7735_WHITE);
        };
    } else if (!draw_erase_toggle){
        for (int i = 0; i < game->snake.length; i++){
            r_Rect((game->snake.segments[i].x*SNAKEGAME_SCALE), (game->snake.segments[i].y*SNAKEGAME_SCALE), SNAKEGAME_SCALE, SNAKEGAME_SCALE, ST7735_BLACK);
        };
    }
};

// /*!*******************************************************************
//     @authors Qwyntyn Scurr
//     @brief Draw the food to the screen
//     @param game A Snake_GameState with important values for the game
//     @deprecated food only drawn when collided, reduced excess function calls
//     @since March 4, 2024
// **********************************************************************/
// void render_food(Snake_GameState *game) {
//     d_Rect((game->food.x*SNAKEGAME_SCALE),(game->food.y*SNAKEGAME_SCALE),SNAKEGAME_SCALE,SNAKEGAME_SCALE, 2, ST7735_NEONGREEN, 1, ST7735_GREEN);
// };

// /*!*******************************************************************
//     @authors Qwyntyn Scurr
//     @brief Draw the score to the screen
//     @param game A Snake_GameState with important values for the game
//     @deprecated score only updated on food collision, reduced excess function calls
//     @since March 4, 2024
// **********************************************************************/
// void render_score(Snake_GameState *game) {
//     format_Print(0,0,game->score,GameScoreText);
// };

// /*!*******************************************************************
//     @authors Qwyntyn Scurr
//     @brief Use rendering functions to draw the game to the screen
//     @note Render Order: clear screen, render food, render snake, render score
//     @deprecated rendering done in gamestate update functions, reduced excess function calls
//     @param game A Snake_GameState with important values for the game
//     @since March 4, 2024
// **********************************************************************/
// void render_game(Snake_GameState *game) {
//     render_food(game);
//     render_snake(game);
//     render_score(game);
// };

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initializes the Snake_GameState
    @param game A Snake_GameState with important values for the game
    @since March 4, 2024
**********************************************************************/
void init_game(Snake_GameState *game) {
    game->snake.length = 1;
    game->snake.head.x = WIDTH / 2;
    game->snake.head.y = HEIGHT / 2;
    game->snake.segments[0].x = game->snake.head.x;
    game->snake.segments[0].y = game->snake.head.y;
    game->food.x = rand() % WIDTH;
    game->food.y = rand() % HEIGHT;
    game->score = 0;
    game->game_over = 0;
    d_Rect((game->food.x*SNAKEGAME_SCALE),(game->food.y*SNAKEGAME_SCALE),SNAKEGAME_SCALE,SNAKEGAME_SCALE, 2, ST7735_NEONGREEN, 1, ST7735_GREEN);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Move the head, check for collisions, move the body, render food, score and snake to screen
    @param game A Snake_GameState with important values for the game
    @returns gameover exitcode
    @since March 4, 2024
**********************************************************************/
void update_gamestate(Snake_GameState *game) {
    move_head(game);

    if (check_self_collision(&game->snake) || check_wall_collision(&game->snake.head)) {
        game->game_over = 1;
        return;
    };

    if (check_food_collision(&game->snake.segments[0], &game->food)) {
        r_Rect((game->food.x*SNAKEGAME_SCALE),(game->food.y*SNAKEGAME_SCALE),SNAKEGAME_SCALE,SNAKEGAME_SCALE,ST7735_BLACK);
        add_segment(game);
        game->score += SCORE_INCREMENT;
        format_Print(0,0,game->score,GameScoreText);
        game->food.x = rand() % GAMEWIDTH;
        game->food.y = rand() % GAMEHEIGHT;
        d_Rect((game->food.x*SNAKEGAME_SCALE),(game->food.y*SNAKEGAME_SCALE),SNAKEGAME_SCALE,SNAKEGAME_SCALE, 2, ST7735_NEONGREEN, 1, ST7735_GREEN);

    };
    render_snake(game, 0);
    move_body(game);
    render_snake(game, 1);
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Get input, update the Snake_GameState and render game to screen
            returns 0 back to system main if game over
    @returns Game over exitcode
    @since March 4, 2024
**********************************************************************/
uint8_t snake_main(void) {
    Snake_GameState SnakeGame;
    clearScreen(ST7735_BLACK);
    init_game(&SnakeGame);
    while (!SnakeGame.game_over){
        handle_input(&SnakeGame);
        update_gamestate(&SnakeGame);
        SysTick_Wait10ms(250);
    };
    // Snake Exitcode | General Exitcode
    return 0;
}