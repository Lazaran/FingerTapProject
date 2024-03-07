// Includes
#include <stdint.h>
#include <string.h>
#include "SysTick.h"
#include "FTP_GFX.h"
#include "FTP_TETRIS.h"
#include "FTP_TYPES.h"
#include "FTP_INPUT.h"

#define INPUT_THRESHOLD 2000
#define FIXED_TETROMINOS 19
#define INPUT_LEFT 0x1
#define INPUT_RIGHT 0x2
#define INPUT_BOTH 0x03
#define AFFIX_TIMER 5


/* Notes:
        Ensure that rotation can only occur above the BlockMass boundary line 
        Tetromino will consist of 1 color, and 4 positions relative to the position
        To keep the en
        The following array is filled in order according to the reference list at the bottom
*/ 


/* Color type array of defined colors */
Color TetrominoColors[11] = {
    ST7735_BLACK,
    ST7735_BLUE ,
    ST7735_RED  ,
    ST7735_GREEN,
    ST7735_CYAN ,
    ST7735_MAGENTA,
    ST7735_YELLOW,
    ST7735_WHITE,
    ST7735_ORANGE,
    ST7735_NEONGREEN,
    ST7735_GRAY 
};

/* Array of ActiveTetrominos each intialized with a Fixed Tetromino 
    and respective color and Max Relative Mino Coordinates */ 
Tetromino FixedTetrominos[19] = {
    // Tetromino: I | Orientation: 0
    {{{0,0}, {1,0}, {2,0}, {3,0}}, 0xFFE0, {0,0}, {3,0}},
    // Tetromino:{0,0} I | Orientation: 1
    {{{0,0}, {0,1}, {0,2}, {0,3}}, 0xFFE0, {0,0}, {0,3}},
    // Tetromino:{0,0} O | Orientation: 0
    {{{0,0}, {1,0}, {0,1}, {1,1}}, 0x07FF, {0,0}, {1,1}},
    // Tetromino:{0,0} T | Orientation: 0
    {{{0,0}, {1,0}, {2,0}, {1,1}}, 0xF81F, {0,0}, {2,1}},
    // Tetromino:{0,0} T | Orientation: 1
    {{{0,0}, {0,1}, {1,1}, {0,2}}, 0xF81F, {0,0}, {1,2}},
    // Tetromino:{0,0} T | Orientation: 2
    {{{1,0}, {0,1}, {1,1}, {2,1}}, 0xF81F, {0,0}, {2,1}},
    // Tetromino:{0,0} T | Orientation: 3
    {{{1,0}, {0,1}, {1,1}, {1,2}}, 0xF81F, {0,0}, {1,2}},
    // Tetromino:{0,0} J | Orientation: 0
    {{{1,0}, {1,1}, {0,2}, {1,2}}, 0xF800, {0,0}, {1,2}},
    // Tetromino:{0,0} J | Orientation: 1
    {{{0,0}, {1,0}, {2,0}, {2,1}}, 0xF800, {0,0}, {2,1}},
    // Tetromino:{0,0} J | Orientation: 2
    {{{0,0}, {1,0}, {0,1}, {0,2}}, 0xF800, {0,0}, {1,2}},
    // Tetromino:{0,0} J | Orientation: 3
    {{{0,0}, {0,1}, {1,1}, {2,1}}, 0xF800, {0,0}, {0,3}},
    // Tetromino:{0,0} L | Orientation: 0
    {{{0,0}, {1,0}, {1,1}, {1,2}}, 0xFBC0, {0,0}, {1,2}},
    // Tetromino:{0,0} L | Orientation: 1
    {{{2,0}, {0,1}, {1,1}, {2,1}}, 0xFBC0, {0,0}, {2,1}},
    // Tetromino:{0,0} L | Orientation: 2
    {{{0,0}, {0,1}, {0,2}, {1,2}}, 0xFBC0, {0,0}, {1,2}},
    // Tetromino:{0,0} L | Orientation: 3
    {{{0,0}, {1,0}, {2,0}, {0,1}}, 0xFBC0, {0,0}, {2,1}},
    // Tetromino:{0,0} S | Orientation: 0
    {{{1,0}, {2,0}, {0,1}, {1,1}}, 0x3FE2, {0,0}, {2,1}},
    // Tetromino:{0,0} S | Orientation: 1
    {{{0,0}, {0,1}, {1,1}, {1,2}}, 0x3FE2, {0,0}, {1,2}},
    // Tetromino:{0,0} Z | Orientation: 0
    {{{0,0}, {1,0}, {1,1}, {2,1}}, 0x001F, {0,0}, {2,1}},
    // Tetromino:{0,0} Z | Orientation: 1
    {{{1,0}, {0,1}, {1,1}, {0,2}}, 0x001F, {0,0}, {1,2}}    
};

// /*!*******************************************************************
//   @authors Qwyntyn Scurr
//   @brief Draws the game border around the edge of the screen.
//             Bounds the game and is true to the original
//   @since February 29, 2024
// **********************************************************************/
// void DrawMatrixBorder(void){
//     for (int i = 0; i < MATRIX_WIDTH; i++){
//         for (int j = 0; j < MATRIX_HEIGHT; j++){
//             if ((i != 0 && i != (MATRIX_WIDTH-1)) && (j != 0 && j != (MATRIX_HEIGHT-1))){
//                 continue;
//             }
//             r_Rect((i*MINO_WIDTH),(j*MINO_HEIGHT),MINO_WIDTH,MINO_HEIGHT,ST7735_GRAY);
//         };
//     };
// };

// /*!*******************************************************************
//   @authors Qwyntyn Scurr
//   @brief Draws a Tetromino to the screen
//   @param active An Tetromino object
//   @param color 16-bit color in 5-6-5 format
//   @since February 29, 2024
// **********************************************************************/
// void DrawTetromino(Tetromino active, Color color){
//     for (int i = 0; i < 4; i++){
//         r_Rect((active.origin[0]+active.minos[i][0])*MINO_WIDTH,(active.origin[1]+active.minos[i][1])*MINO_HEIGHT,MINO_WIDTH,MINO_HEIGHT,color);
//     };
// };


// /*!*******************************************************************
//   @authors Qwyntyn Scurr
//   @brief Draws the MinoMatrix to the screen, dependent on MinoWidth and Height
//   @param active An Tetromino object
//   @param color 16-bit color in 5-6-5 format
//   @since February 29, 2024
// **********************************************************************/
// void Matrix_Draw(TetrisMatrix active, uint8_t arrSize, Color colors[arrSize]){
//     Vector2 drawPosition = {0,0};
//     for (int colorsIndex = 0; colorsIndex < arrSize; colorsIndex++){
//         if ((colorsIndex % BORDERED_MATRIX_WIDTH) == 0){
//             drawPosition[1] += 1;
//         }
//         drawPosition[0] = (colorsIndex % BORDERED_MATRIX_WIDTH);
//         r_Rect((active.origin[0]+drawPosition[0])*MINO_WIDTH, (active.origin[0]+drawPosition[1])*MINO_HEIGHT, MINO_WIDTH, MINO_HEIGHT, colors[colorsIndex]);
//     };
// };



// /*!*******************************************************************
//   @authors Qwyntyn Scurr
//   @brief Adds a row at position 'newRow' of an TetrisMatrix object
//   @param active An TetrisMatrix object
//   @param colors Array of Matrix Mino colors
//   @param newRow A uint8_t with the new row
//   @since February 29, 2024
// **********************************************************************/
// void Matrix_AddRow(TetrisMatrix active, Color colors[active.arrSize], uint8_t newRow){
//     active.arrSize += BORDERED_MATRIX_WIDTH;
//     active.size[1] += 1;
//     uint8_t tempCount = 0;
//     Color* newMatrix = (Color*) calloc(active.arrSize, sizeof(Color));
//     for (int row = 0; row < active.size[1]; row++){
//         for (int col = 0; col < active.size[0]; col++){
//             if (row < newRow){
//                 newMatrix[tempCount] = colors[tempCount];
//             }
//             else if (row > newRow){
//                 newMatrix[tempCount] = colors[tempCount-BORDERED_MATRIX_WIDTH];
//             }
//         }
//     }
// }

// /*!*******************************************************************
//   @authors Qwyntyn Scurr
//   @brief Deletes a row from position 'delRow' of an TetrisMatrix object
//   @note Copied from Matrix_AddRow, even less sure this will work as only flipped a couple values
//   @param active An TetrisMatrix object
//   @param colors Array of Matrix Mino colors
//   @param delRow A uint8_t with the row to be deleted
//   @since February 29, 2024
// **********************************************************************/
// void Matrix_DeleteRow(TetrisMatrix active, Color colors[active.arrSize], uint8_t delRow){
//     active.arrSize -= BORDERED_MATRIX_WIDTH;
//     active.size[1] -= 1;
//     uint8_t tempCount = 0;
//     Color* newMatrix = (Color*) calloc(active.arrSize, sizeof(Color));
//     for (int row = 0; row < active.size[1]; row++){
//         for (int col = 0; col < active.size[0]; col++){
//             if (row < delRow){
//                 newMatrix[tempCount] = colors[tempCount];
//             }
//             else if (row > delRow){
//                 newMatrix[tempCount] = colors[tempCount+BORDERED_MATRIX_WIDTH];
//             }
//         }
//     }
// }

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks raw finger circuit values and moves falling tetromino 
    @note 0x1 = Left, 0x2 = Right
    @param game A Tetris_GameState with important values for the game
    @since March 5, 2024
**********************************************************************/
void handle_tetrisinput(Tetris_GameState *game) {
    if (MiddleCircuit > INPUT_THRESHOLD){
        game->player_input = 0x01;
    };
    if (RingCircuit > INPUT_THRESHOLD){
        game->player_input = 0x02;
    };
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Moves the falling tetromino
    @note 0x1 = Left, 0x02 = Right
    @param game A Tetris_GameState with important values for the game
    @since March 5, 2024
**********************************************************************/
void move_tetromino(Tetris_GameState *game) {
    if (game->is_falling){
        game->tetromino.origin.y += 1;
    };
    switch(game->sides_valid & game->player_input){
        case 0x01:
            game->tetromino.origin.x -= 1;
            break;
        case 0x02:
            game->tetromino.origin.x += 1;
            break;
    };
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for a collision between the matrix and the falling tetromino
            voiding valid directions if collisions found
    @param game A Tetris_GameState with important values for the game
    @since March 5, 2024
**********************************************************************/
void check_matrix_collision(Tetris_GameState *game) {
    int i = 0;
    int j = 0;
    int yPosition = TETRISHEIGHT;
    int xPosition = 0;
    for (j = game->matrix.arrSize - 1; j >= 0; j--){        
        if (game->matrix.stuck_minos[i] == 0){
            continue;
        }

        if (j % TETRISWIDTH == 0){
            yPosition--;
        }
        xPosition = (j % TETRISWIDTH);

        for (i = 0; i < 4; i++){
            // Blocking down
            if (game->tetromino.falling_minos[i].y+1 == yPosition && game->tetromino.falling_minos[i].x == xPosition){
                game->is_falling = 0;
            };
            // Blocking left side
            if (game->tetromino.falling_minos[i].y == yPosition && game->tetromino.falling_minos[i].x-1 == xPosition){
                game->sides_valid &= ~INPUT_LEFT;
            };
            // Blocking right side
            if (game->tetromino.falling_minos[i].y == yPosition && game->tetromino.falling_minos[i].x+1 == xPosition){
                game->sides_valid &= ~INPUT_RIGHT;
            };
        };
    };
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for a collision between the falling tetromino and the boundaries of the game screen
    @param game A Tetris_GameState with important values for the game
    @since March 6, 2024
**********************************************************************/
void check_boundary_collision(Tetris_GameState *game) {
    if (game->tetromino.origin.x == 0){
        game->sides_valid &= ~INPUT_LEFT;
    }
    if (game->tetromino.bounding_box.x == TETRISWIDTH){
        game->sides_valid &= ~INPUT_RIGHT;
    }
    if (game->tetromino.bounding_box.y == TETRISHEIGHT){
        game->is_falling = 0;
    }
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Adds the colored minos of a newly fallen tetromino to the matrix
    @param game A Tetris_GameState with important values for the game
    @since March 6, 2024
**********************************************************************/
void affix_tetromino(Tetris_GameState *game) {
    int i = 0;
    int tempPos = 0;
    for (i = 0; i < 4; i++){
        tempPos = ((game->tetromino.falling_minos[i].y * TETRISWIDTH) + game->tetromino.falling_minos[i].x);
        game->matrix.stuck_minos[tempPos] = game->tetromino.color;
    }
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Sets a random active tetromino from the FixedTetrominos list
    @param game A Tetris_GameState with important values for the game
    @since March 6, 2024
**********************************************************************/
void random_tetromino(Tetris_GameState *game) {
    game->active_tetromino = rand() % 19;
    game->tetromino.origin.x = ((TETRISWIDTH / 2) - ((TETRISWIDTH % 2) / 2));
    game->tetromino.origin.y = 0;
    game->tetromino = FixedTetrominos[game->active_tetromino];
}


/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Renders the falling tetromino to the screen
    @param game A Tetris_GameState with important values for the game
    @since March 6, 2024
**********************************************************************/
void render_tetromino(Tetris_GameState *game, uint8_t color_toggle) {
    int i = 0;
    if (color_toggle){
        for (i = 0; i < 4; i++){
            d_Rect(game->tetromino.falling_minos[i].x*TETRISGAME_SCALE,game->tetromino.falling_minos[i].y*TETRISGAME_SCALE,TETRISGAME_SCALE,TETRISGAME_SCALE, 3, game->tetromino.color, 0, game->tetromino.color);
        }
    } else {
        for (i = 0; i < 4; i++){
            d_Rect(game->tetromino.falling_minos[i].x*TETRISGAME_SCALE,game->tetromino.falling_minos[i].y*TETRISGAME_SCALE,TETRISGAME_SCALE,TETRISGAME_SCALE, 3, ST7735_BLACK, 0, ST7735_BLACK);
        }
    }
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Renders the matrix to the screen
    @param game A Tetris_GameState with important values for the game
    @since March 6, 2024
**********************************************************************/
void render_matrix(Tetris_GameState *game) {
    int i = 0;
    int tempX = 0;
    int tempY = TETRISHEIGHT;
    for (i = 0; i < game->matrix.stuck_minos[i]; i++){
        if (game->matrix.stuck_minos[i] == 0){
            continue;
        }
        
        if (i % TETRISWIDTH == 0){
            tempY--;
        }
        tempX = (i % TETRISWIDTH);

        d_Rect(tempX*TETRISGAME_SCALE, tempY*TETRISGAME_SCALE, TETRISGAME_SCALE, TETRISGAME_SCALE, 4, game->matrix.stuck_minos[i], 0,game->matrix.stuck_minos[i]);
    }
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initialize the GameState
    @param game A Tetris_GameState with important values for the game
    @since March 5, 2024
**********************************************************************/
void init_tetrisgame(Tetris_GameState *game) {
    // Matrix
    game->matrix.origin.x = 0;
    game->matrix.origin.y = 0;  
    game->matrix.bounding_box.x = 0;
    game->matrix.bounding_box.y = 0;
    game->matrix.arrSize = TETRISHEIGHT * TETRISWIDTH;

    // Tetromino
    game->active_tetromino = rand() % 19;
    game->tetromino.origin.x = ((TETRISWIDTH / 2) - ((TETRISWIDTH % 2) / 2));
    game->tetromino.origin.y = 0;
    game->tetromino = FixedTetrominos[game->active_tetromino];
    
    // Other
    game->game_over = 0;
    game->player_input = 0;
    game->sides_valid = 3;
    game->is_falling = 1;
    game->fall_speed = 1;
    game->affix_timer = AFFIX_TIMER;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Check for collisions, check gameover condition, move the falling tetromino, affix fallen tetromino to matrix
            render all objects to screen
    @param game A Tetris_GameState with important values for the game
    @returns gameover exitcode
    @since March 6, 2024
**********************************************************************/
void update_tetrisgamestate(Tetris_GameState *game) {
    game->sides_valid = 0x3;
    check_matrix_collision(game);
    check_boundary_collision(game);

    if (game->sides_valid == 0 && game->is_falling == 0 && game->tetromino.origin.y == 0){
        game->game_over = 1;
    }

    if (!game->is_falling){
        if (game->affix_timer == 0){
            affix_tetromino(game);
            random_tetromino(game);
            render_matrix(game);
        } else {
            game->affix_timer -= 1;
        };
    } else {
        if (game->affix_timer != AFFIX_TIMER){
            game->affix_timer = AFFIX_TIMER;
        };
    };

    render_tetromino(game, 0);
    move_tetromino(game);
    render_tetromino(game, 1);
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Main game loop
  @since February 29, 2024
**********************************************************************/
void tetris_main(void) {
    Tetris_GameState TetrisGame;
    clearScreen(ST7735_BLACK);
    // init_game(&TetrisGame);
    while(!TetrisGame.game_over){
        handle_tetrisinput(&TetrisGame);
        update_tetrisgamestate(&TetrisGame);
        SysTick_Wait10ms(100/TetrisGame.fall_speed);
    }
};




