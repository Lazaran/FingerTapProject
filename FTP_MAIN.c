#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_PONG.h"


// ADC requires wait for interrupts
#define COLORS 0xFFFF

int main(void){
        SysTick_Init();
        PLL_Init();
        ST7735_initR();
	    clearScreen(ST77XX_BLACK);

    while (1){
        pong_main();
    };
};
