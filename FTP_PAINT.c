/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Add shapes likes rectangles and lines to the screen, color them
            and change their sizes however you like!
    @since March 14, 2024
    @version Rev 4
**********************************************************************/

#include <stdint.h>
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_PAINT.h"

uint8_t paint_main(void){
    if (ScreenOrientation != 5){
        ScreenOrientation = 5;
        setDirection(ScreenOrientation);
    }
    clearScreen(ST7735_BLACK);
    d_Rect((ScreenW/8),(ScreenH/8),ScreenW/2, ScreenH/2,3,ST7735_CYAN,0,ST7735_BLACK);
    d_Rect((ScreenW/2),(ScreenH/8),ScreenW/2, ScreenH/2,3,ST7735_RED,0,ST7735_BLACK);
    d_Rect((ScreenW/8),(ScreenH/2),ScreenW/2, ScreenH/2,3,ST7735_YELLOW,0,ST7735_BLACK);
    d_Rect((ScreenW/2),(ScreenH/2),ScreenW/2, ScreenH/2,3,ST7735_GREEN,0,ST7735_BLACK);
    d_DrawString(11,5,"Under",ST7735_WHITE);
    d_DrawString(8,6,"Construction",ST7735_WHITE);
    SysTick_Wait10ms(2000);
    return 15;
}