#ifndef FTP_TETRIS_H
#define FTP_TETRIS_H

#include <stdint.h>

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

#define MINO_WIDTH ((uint8_t) 10)
#define MINO_HEIGHT ((uint8_t) 10)
// Matrix size determined from mino and screen dimensions
#define MATRIX_WIDTH ((uint8_t) (ST7735_TFTWIDTH/MINO_WIDTH))
#define MATRIX_HEIGHT ((uint8_t) (ST7735_TFTHEIGHT/MINO_HEIGHT))
// Subtract 2 from matrix width and height to allow for borders
#define BORDERED_MATRIX_WIDTH ((uint8_t) (MATRIX_WIDTH - 2))
#define BORDERED_MATRIX_HEIGHT ((uint8_t) (MATRIX_HEIGHT - 2))

typedef uint8_t Tetromino[5][2];

typedef uint8_t MinoPile[2];

extern uint8_t FixedTetrominos[19][5][2]; 

uint8_t GetTetrominoSize(void);

void SelectTetromino(uint8_t index);

void DrawMatrixBorder(void);

void tetris_main(uint8_t exitcode);

void DrawTetromino(Tetromino activeBlock, uint8_t xPos, uint8_t yPos, uint16_t color);

/*
The following is a list of the shapes, their orientations and 
    the relative positions of each block in each orientation

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
            {{0xF8,0x00},{0,0},{0,1},{0,2},{0,3}}

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



#endif