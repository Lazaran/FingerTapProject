#include <stdio.h>
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
#include "FTP_SNAKE.h"
#include "string.h"

char IndexText[] = "Index Value";
char MiddleText[] = "Middle Value";
char RingText[] = "Ring Value";
char PinkyText[] = "Pinky Value";

void WaitForInterrupt(void);



int main(void){
    PLL_Init();
    SysTick_Init();
    ST7735_initR();
    Input_Init();

    clearScreen(ST77XX_BLACK);
    
    while (1){
        snake_main();
        // d_DrawString(0,0,IndexText,ST7735_WHITE);
        // // WaitForInterrupt();
        // format_Print(0,0,IndexCircuit,IndexText);
        // format_Print(0,2,MiddleCircuit,MiddleText);
        // format_Print(0,4,RingCircuit,RingText);
        // format_Print(0,6,PinkyCircuit,PinkyText);
        // SysTick_Wait10ms(100);
        // // tetris_main(1);
    };
};
