/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Add shapes likes rectangles and lines to the screen, color them
            and change their sizes however you like!
    @since March 14, 2024
    @version Rev 1
**********************************************************************/
// Includes
#include <stdint.h>
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_PAINT.h"

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Get input, update the Paint_AppState and render shapes to screen
    @returns Game over exitcode
    @since March 14, 2024
**********************************************************************/
uint8_t paint_main(void){
    // Set Screen Orientation
    if (ScreenOrientation != 5){
        ScreenOrientation = 5;
        setDirection(ScreenOrientation);
    }
    under_construction();
    // while (1){
    // }
    SysTick_Wait10ms(2000);
    return 15;
}