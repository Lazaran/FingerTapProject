#include <stdint.h>
#include <string.h>
#include "SysTick.h"
#include "FTP_TETRIS.h"
#include "FTP_GFX.h"
#include "FTP_GAMEOBJ.h"
#include "FTP_ST7735R.h"
#include "FTP_PHYSX.h"


uint8_t FixedTetrominos[19][5][2] = {
    
    // Tetromino: I | Orientation: 0
    {{0xFF,0xE0}, {0,0}, {1,0}, {2,0}, {3,0}},   
    
    // Tetromino: I | Orientation: 1
    {{0xFF,0xE0}, {0,0}, {0,1}, {0,2}, {0,3}},   
    
    // Tetromino: O | Orientation: 0
    {{0x07,0xFF}, {0,0}, {1,0}, {0,1}, {1,1}},   
    
    // Tetromino: T | Orientation: 0
    {{0xF8,0x1F}, {0,0}, {1,0}, {2,0}, {1,1}},   
    
    // Tetromino: T | Orientation: 1
    {{0xF8,0x1F}, {0,0}, {0,1}, {1,1}, {0,2}},   
    
    // Tetromino: T | Orientation: 2
    {{0xF8,0x1F}, {1,0}, {0,1}, {1,1}, {2,1}},   
    
    // Tetromino: T | Orientation: 3
    {{0xF8,0x1F}, {1,0}, {0,1}, {1,1}, {1,2}},   
    
    // Tetromino: J | Orientation: 0
    {{0xF8,0x00}, {1,0}, {1,1}, {0,2}, {1,2}},   
    
    // Tetromino: J | Orientation: 1
    {{0xF8,0x00}, {0,0}, {1,0}, {2,0}, {2,1}},   
    
    // Tetromino: J | Orientation: 2
    {{0xF8,0x00}, {0,0}, {1,0}, {0,1}, {0,2}},   
    
    // Tetromino: J | Orientation: 3
    {{0xF8,0x00}, {0,0}, {0,1}, {0,2}, {0,3}},   
    
    // Tetromino: L | Orientation: 0
    {{0xFD,0x20}, {0,0}, {1,0}, {1,1}, {1,2}},   
    
    // Tetromino: L | Orientation: 1
    {{0xFD,0x20}, {2,0}, {0,1}, {1,1}, {2,1}},   
    
    // Tetromino: L | Orientation: 2
    {{0xFD,0x20}, {0,0}, {0,1}, {0,2}, {1,2}},   
    
    // Tetromino: L | Orientation: 3
    {{0xFD,0x20}, {0,0}, {1,0}, {2,0}, {0,1}},   
    
    // Tetromino: S | Orientation: 0
    {{0x3F,0xE2}, {1,0}, {2,0}, {0,1}, {1,1}},   
    
    // Tetromino: S | Orientation: 1
    {{0x3F,0xE2}, {0,0}, {0,1}, {1,1}, {1,2}},   
    
    // Tetromino: Z | Orientation: 0
    {{0x00,0x1F}, {0,0}, {1,0}, {1,1}, {2,1}},   
    
    // Tetromino: Z | Orientation: 1
    {{0x00,0x1F}, {1,0}, {0,1}, {1,1}, {0,2}}    
};
/*
    Ensure that rotation can only occur above the BlockMass boundary line 
*/ 

/*
Tetromino will consist of 1 color, and 4 positions relative to the position
To keep the en
*/

/* The following array is filled in order according to the reference list at the bottom
*/


// Tetromino testTetromino = {{0x00,0x1F}, {1,0}, {0,1}, {1,1}, {0,2}};

//Tetromino *SelectTetromino(uint8_t index){
//    Tetromino Selection = FixedTetrominos[index];
//    return Selection;
//};

uint8_t GetTetrominoSize(void){
    return sizeof(FixedTetrominos[0]);
}

void DrawMatrixBorder(){
    for (int i = 0; i < MATRIX_WIDTH; i++){
        for (int j = 0; j < MATRIX_HEIGHT; j++){
            if ((i != 0 && i != (MATRIX_WIDTH-1)) && (j != 0 && j != (MATRIX_HEIGHT-1))){
                continue;
            }
            r_Rect((i*MINO_WIDTH),(j*MINO_HEIGHT),MINO_WIDTH,MINO_HEIGHT,ST7735_GRAY);
        };
    };
};

void DrawTetromino(Tetromino activeBlock, uint8_t xPos, uint8_t yPos, uint16_t color){
    r_Rect(((xPos*MINO_WIDTH)+(MINO_WIDTH*activeBlock[1][0])),((yPos*MINO_HEIGHT)+(MINO_HEIGHT*activeBlock[1][1])),MINO_WIDTH,MINO_HEIGHT,color);
    r_Rect(((xPos*MINO_WIDTH)+(MINO_WIDTH*activeBlock[2][0])),((yPos*MINO_HEIGHT)+(MINO_HEIGHT*activeBlock[2][1])),MINO_WIDTH,MINO_HEIGHT,color);
    r_Rect(((xPos*MINO_WIDTH)+(MINO_WIDTH*activeBlock[3][0])),((yPos*MINO_HEIGHT)+(MINO_HEIGHT*activeBlock[3][1])),MINO_WIDTH,MINO_HEIGHT,color);
    r_Rect(((xPos*MINO_WIDTH)+(MINO_WIDTH*activeBlock[4][0])),((yPos*MINO_HEIGHT)+(MINO_HEIGHT*activeBlock[4][1])),MINO_WIDTH,MINO_HEIGHT,color);
};



void tetris_main(uint8_t exitcode){
    Tetromino testBlock;
    memcpy(testBlock, FixedTetrominos, GetTetrominoSize());

    while(exitcode){
        SysTick_Wait10ms(100);


    };
};

