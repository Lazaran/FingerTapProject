/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Demonstrates FSR responsivness
    @since March 21, 2024
    @version Rev 1
**********************************************************************/

// Includes
#include <stdint.h>
#include "systick.h"
#include "FTP_INPUT.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_FSRDEMO.h"

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Gets user input from the FSRs and analog circuits
    @param fsrdemo AppState with important values for FSR Demo
    @since March 21, 2024
**********************************************************************/
void fsrdemo_input(FSR_AppState *fsrdemo){
    // Get FSR Inputs
    uint8_t temp_fsr = FSR_Parse();
    switch(temp_fsr){
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            fsrdemo->index_code = temp_fsr - 10;
            break;
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
            fsrdemo->middle_code = temp_fsr - 20;
            break;
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
            fsrdemo->ring_code = temp_fsr - 30;
            break;
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
            fsrdemo->pinky_code = temp_fsr - 40;
            break;
        default:
            break;
    };
    // Get analog circuit inputs
    switch(Circuit_Parse()){
        case 45:
            fsrdemo->exit_code = 1;
            break;
        default:
            break;
    };
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initializes the FSR Demmo Appstate
    @param fsrdemo AppState with important values for FSR Demo
    @param toggle 1 = Render colors | 0 = Erase colors
    @since March 21, 2024
**********************************************************************/
void render_FSR(FSR_AppState *fsrdemo, uint8_t toggle){
    if (toggle){
        d_Rect(25,25,(5*fsrdemo->index_code)+10,(5*fsrdemo->index_code)+10,2,ST7735_RED,0,ST7735_BLACK);
        d_Rect(75,25,(5*fsrdemo->middle_code)+10,(5*fsrdemo->middle_code)+10,2,ST7735_GREEN,0,ST7735_BLACK);
        d_Rect(25,75,(5*fsrdemo->ring_code)+10,(5*fsrdemo->ring_code)+10,2,ST7735_CYAN,0,ST7735_BLACK);
        d_Rect(75,75,(5*fsrdemo->pinky_code)+10,(5*fsrdemo->pinky_code)+10,2,ST7735_YELLOW,0,ST7735_BLACK);
    }
    else {    
        d_Rect(5,5,(5*fsrdemo->index_code)+10,(5*fsrdemo->index_code)+10,2,ST7735_BLACK,0,ST7735_BLACK);
        d_Rect(50,5,(5*fsrdemo->middle_code)+10,(5*fsrdemo->middle_code)+10,2,ST7735_BLACK,0,ST7735_BLACK);
        d_Rect(5,50,(5*fsrdemo->ring_code)+10,(5*fsrdemo->ring_code)+10,2,ST7735_BLACK,0,ST7735_BLACK);
        d_Rect(50,50,(5*fsrdemo->pinky_code)+10,(5*fsrdemo->pinky_code)+10,2,ST7735_BLACK,0,ST7735_BLACK);
    }
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initializes the FSR Demmo Appstate
    @param fsrdemo AppState with important values for FSR Demo
    @since March 21, 2024
**********************************************************************/
void init_fsrdemo(FSR_AppState *fsrdemo){
    fsrdemo->index_code = 0;
    fsrdemo->middle_code = 0;
    fsrdemo->ring_code = 0;
    fsrdemo->pinky_code = 0;
    fsrdemo->exit_code = 0;   
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Orient the screen for the applicaton, intialize the application
            object to respond to FSR inputs and display changes accordingly 
    @returns Game Over exitcode
    @since March 21, 2024
**********************************************************************/
uint8_t fsr_main(void){
    // Orient the screen
    if (ScreenOrientation != FSR_ORIENT){
        ScreenOrientation = FSR_ORIENT;
        setDirection(ScreenOrientation);
    };
    clearScreen(ST7735_BLACK);
    // Initialize the FSR Demo
    FSR_AppState FSRDemo;
    init_fsrdemo(&FSRDemo);
    // FSR Demo superloop
    while (!FSRDemo.exit_code){
        fsrdemo_input(&FSRDemo);
        // Render dynamically changing rectangles to screen
        clearScreen(ST7735_BLACK);
        render_FSR(&FSRDemo,1);
        SysTick_Wait10ms(200);
    }
    return 14;
}