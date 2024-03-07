/* File FTP_TETRIS.h*/
#ifndef FTP_TETRIS_H
#define FTP_TETRIS_H

// Includes
#include <stdint.h>
#include "FTP_TYPES.h"
#include "FTP_ST7735R.h"

// Reference colors, Orange doesn't work for some reason
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0xF800
#define ST7735_RED     0x001F
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0xFFE0
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0x07FF
#define ST7735_WHITE   0xFFFF
#define ST7735_ORANGE  0xFD20
#define ST7735_NEONGREEN 0x3FE2
#define ST7735_GRAY 0x8410

/* Mino dimensions, entire scale of game is based on this 
    Only takes MinoDimension if width/height is fully divisable by it
    Otherwise defaults to ten */ 
// Number of pixels per square | IMPORTANT
#define TETRISGAME_SCALE 10

// At snake scale of 10, width = 12, height = 16
#define TETRISWIDTH ((ST7735_TFTWIDTH/TETRISGAME_SCALE) - ((ST7735_TFTWIDTH % TETRISGAME_SCALE)/TETRISGAME_SCALE))
#define TETRISHEIGHT ((ST7735_TFTHEIGHT/TETRISGAME_SCALE) - ((ST7735_TFTHEIGHT % TETRISGAME_SCALE)/TETRISGAME_SCALE))

/* App Core Types: Rely on standard types, app-specific*/
/* Tetris */

/* TETRIS */
typedef struct Tetromino{
    Vector2 falling_minos[4];
    Color color;
    Vector2 origin;
    Vector2 bounding_box;
} Tetromino;

typedef struct TetrisMatrix{
    Color stuck_minos[TETRISHEIGHT*TETRISWIDTH];
    Vector2 origin;
    Vector2 bounding_box;
    uint8_t arrSize;
} TetrisMatrix;

typedef struct Tetris_GameState{
    TetrisMatrix matrix;
    Tetromino tetromino;
    uint8_t active_tetromino;
    uint8_t game_over;
    uint8_t player_input;
    uint8_t sides_valid;
    uint8_t is_falling;
    uint8_t fall_speed;
    uint8_t affix_timer;
} Tetris_GameState;

// Function prototypes

void handle_tetrisinput(Tetris_GameState *game);

void move_tetromino(Tetris_GameState *game);

void check_matrix_collision(Tetris_GameState *game);

void check_boundary_collision(Tetris_GameState *game);

void affix_tetromino(Tetris_GameState *game);

void random_tetromino(Tetris_GameState *game);

void render_tetromino(Tetris_GameState *game, uint8_t color_toggle);

void render_matrix(Tetris_GameState *game);

void init_tetrisgame(Tetris_GameState *game);

void update_tetrisgamestate(Tetris_GameState *game);

void tetris_main(void);


/* Reference list of all Fixed Tetrominos, their 
    relative Mino coordinates and their colors
Tetrominos:
    I:
        ####
            {{0xFF,0xE0},{0,0},{1,0},{2,0},{3,0}}
        #
        #
        #
        #
            {{0xFF,0xE0},{0,0},{0,1},{0,2},{0,3}}

    O:
        ##
        ##
            {{0x07,0xFF},{0,0},{1,0},{0,1},{1,1}}

    T:
        ###
         #
            {{0xF8, 0x1F},{0,0},{1,0},{2,0},{1,1}}
        #
        ##
        #
            {{0xF8, 0x1F},{0,0},{0,1},{1,1},{0,2}}
         #
        ###
            {{0xF8, 0x1F},{1,0},{0,1},{1,1},{2,1}}
         #
        ##
         #
            {{0xF8, 0x1F},{1,0},{0,1},{1,1},{1,2}}

    J:
         #
         #
        ##
            {{0xF8,0x00},{1,0},{1,1},{0,2},{1,2}}
        ###
          #
            {{0xF8,0x00},{0,0},{1,0},{2,0},{2,1}}
        ##
        #
        #
            {{0xF8,0x00},{0,0},{1,0},{0,1},{0,2}}
        #
        ###
            {{0xF8,0x00},{0,0},{0,1},{1,1},{2,1}}

    L:
        ## 
         #
         #
            {{0xFD, 0x20},{0,0},{1,0},{1,1},{1,2}}
          #
        ###
            {{0xFD, 0x20},{2,0},{0,1},{1,1},{2,1}}
        #
        #
        ##
            {{0xFD, 0x20},{0,0},{0,1},{0,2},{1,2}}
        ###
        #
            {{0xFD, 0x20},{0,0},{1,0},{2,0},{0,1}}

    S:
         ##
        ##
            {{0x3F, 0xE2},{1,0},{2,0},{0,1},{1,1}}
        # 
        ##
         #
            {{0x3F, 0xE2},{0,0},{0,1},{1,1},{1,2}}

    Z:
        ##
         ##
            {{0x00, 0x1F},{0,0},{1,0},{1,1},{2,1}}
         #
        ##
        #
            {{0x00, 0x1F},{1,0},{0,1},{1,1},{0,2}}
*/
/* Reference bitmap of the Spaceship from Space Invaders
       #
       #
       #
      ###  
   3  ###  3
   3  ###  3  
3  # ##### #  3
3  #@##3##@#  3
#  @##333##@  #
#  ###3#3###  #
# ########### #
#####3###3#####
### 33###33 ###
##  33 # 33  ##
#      #      #
*/
/* Reference bitmap of Alien from Space Invaders
  #     #
   #   # 
  #######
 ## ### ##
###########
# ####### #
# #     # #
   ## ##
*/

#endif
