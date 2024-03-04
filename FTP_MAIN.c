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
#include "string.h"

char IndexText[] = "Index Value";
char MiddleText[] = "Middle Value";
char RingText[] = "Ring Value";
char PinkyText[] = "Pinky Value";

void WaitForInterrupt(void);

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Prints the input number and text to the LCD at the positions defined
  @param x X position for the string to draw at
  @param y Y position for the string to draw at
  @param num Number to format into drawn string
  @param text String to format into drawn string
  @since March 3, 2024
**********************************************************************/
void format_Print(uint8_t x, uint8_t y, int num, char* text){
	char buffer[100];
//    if (sizeof(text)/sizeof(text[0]) <= 10){
        // Format the inputs number and string for display
        snprintf(buffer, 100, "%d < %s  ", num, text);
        // Draw formatted string to LCD
        d_DrawString(x,y,buffer,ST7735_WHITE);
//		};
};

int main(void){
    PLL_Init();
    SysTick_Init();
    ST7735_initR();
    Input_Init();

    clearScreen(ST77XX_BLACK);
    
    while (1){
        d_DrawString(0,0,IndexText,ST7735_WHITE);
        // WaitForInterrupt();
        format_Print(0,0,IndexCircuit,IndexText);
        format_Print(0,2,MiddleCircuit,MiddleText);
        format_Print(0,4,RingCircuit,RingText);
        format_Print(0,6,PinkyCircuit,PinkyText);
        SysTick_Wait10ms(100);
        // tetris_main(1);
    };
};
