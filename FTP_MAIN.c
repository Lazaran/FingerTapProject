#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_GAMEOBJ.h"
#include "FTP_PHYSX.h"
#include "FTP_PONG.h"
#include "FTP_TETRIS.h"
#include "string.h"


// ADC requires wait for interrupts
#define COLORS 0xFFFF

int main(void){
    SysTick_Init();
    PLL_Init();
    ST7735_initR();
    clearScreen(ST77XX_BLACK);
    int fall = 0;
    int iter = 0;
        Tetromino testBlock;
            memcpy(testBlock, FixedTetrominos[5], GetTetrominoSize());
    while (1){
        if (iter < 19){
        };
        while(fall < 14){
            if (fall < 14){
                uint16_t minoColor = ((testBlock[0][0] << 8) | (testBlock[0][1]));
                DrawTetromino(testBlock,3,fall,ST7735_BLACK);
                fall++;
                DrawTetromino(testBlock,3,fall,minoColor);
                SysTick_Wait10ms(250);
            };
        };
        // iter++;
        // DrawMatrixBorder();
        // pong_main();
    };
};
