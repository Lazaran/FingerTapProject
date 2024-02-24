#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"


// ADC requires wait for interrupts

int main(void){
        SysTick_Init();
        PLL_Init();
        ST7735_initR();
    while (1){

        dRect(20,20,50,50,2,0xFF,0,0xFF);
    
    }



};
