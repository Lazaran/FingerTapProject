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

#define MINO_DIMENSION 10
/* Mino dimensions, entire scale of game is based on this 
    Only takes MinoDimension if width/height is fully divisable by it
    Otherwise defaults to ten */ 
#define MINO_WIDTH ((uint8_t) ((ST7735_TFTWIDTH % MINO_DIMENSION == 0) ? MINO_DIMENSION : 10))
#define MINO_HEIGHT ((uint8_t) ((ST7735_TFTHEIGHT % MINO_DIMENSION == 0) ? MINO_DIMENSION : 10))
/* Matrix dimensions, determined from mino and screen dimensions */
#define MATRIX_WIDTH ((uint8_t) (ST7735_TFTWIDTH/MINO_WIDTH))
#define MATRIX_HEIGHT ((uint8_t) (ST7735_TFTHEIGHT/MINO_HEIGHT))
/* Viewport dimensions, subtract 2 from matrix dimensions */
#define BORDERED_MATRIX_WIDTH ((uint8_t) (MATRIX_WIDTH - 2))
#define BORDERED_MATRIX_HEIGHT ((uint8_t) (MATRIX_HEIGHT - 2))



// Array declarations, don't really like these, want to find better ways
extern Vector2 StarshipWhite[83];
extern Vector2 StarshipBlue[4];
extern Vector2 StarshipRed[24];
extern Vector2 Alien[46];
extern ActiveTetromino FixedTetrominos[19]; 

// Function prototypes
void Tetromino_SetOrigin(ActiveTetromino active, Vector2 newOrigin);

void Tetromino_SetSize(ActiveTetromino active, Vector2 newSize);

void Tetromino_SetColor(ActiveTetromino active, Color color);

void Tetromino_SetMinos(ActiveTetromino active, Tetromino newTetromino);

void DrawMatrixBorder(void);

void DrawTetromino(ActiveTetromino active, Color color);

void Matrix_Draw(ActiveMatrix active, Vector1 arrSize, Color colors[arrSize]);

void Matrix_SetOrigin(ActiveMatrix active, Vector2 newOrigin);

void Matrix_SetSize(ActiveMatrix active, Vector2 newSize);

void Matrix_SetMaxHeight(ActiveMatrix active, Vector1 newMaxHeight);

void Matrix_SetArrSize(ActiveMatrix active, Vector1 newArrSize);

void Matrix_AddRow(ActiveMatrix active, Color colors[active.arrSize], Vector1 newRow);

void Matrix_DeleteRow(ActiveMatrix active, Color colors[active.arrSize], Vector1 delRow);

void tetris_main(uint8_t exitcode);


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
