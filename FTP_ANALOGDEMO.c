/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Demonstrates that all analog inputs are functioning on the FingerTap
            System and displays the raw signals from all inputs.
    @since March 21, 2024
    @version Rev 1
**********************************************************************/

// Includes
#include <stdint.h>
#include "FTP_INPUT.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_ANALOGDEMO.h"

// Display Text
char IndexTextCircuit[] = "< Index Circuit";
char MiddleTextCircuit[] = "< Middle Circuit";
char RingTextCircuit[] = "< Ring Circuit";
char PinkyTextCircuit[] = "< Pinky Circuit";
char IndexTextFSR[] = "< Index FSR";
char MiddleTextFSR[] = "< Middle FSR";
char RingTextFSR[] = "< Ring FSR";
char PinkyTextFSR[] = "< Pinky FSR";


/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Gets user input to exit the program
    @since March 21, 2024
**********************************************************************/
void analog_input(Demo_AppState *demo){
    switch(Circuit_Parse()){
        case 45:
            demo->exit_code = 1;
            break;
        default:
            break;
    };
    return;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Orient the screen for the applicaton, intialize the application object
            display raw analog values
    @returns Program done exitcode
    @since March 21, 2024
**********************************************************************/
void init_demo(Demo_AppState *demo){
    demo->exit_code = 0;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Orient the screen for the applicaton, intialize the application object
            display raw analog values
    @returns Program done exitcode
    @since March 21, 2024
**********************************************************************/
uint8_t demo_main(void) {
    // Orient the screen
    if (ScreenOrientation != DEMO_ORIENT){
        ScreenOrientation = DEMO_ORIENT;
        setDirection(ScreenOrientation);
    };
    // Clear the screen
    clearScreen(ST7735_BLACK);
    // Initialize
    Demo_AppState demo;
    init_appstate(&demo);
    // Application superloop
    while (!demo.exit_code){
        analog_input(&demo);
        // Analog Circuit Raw Data Formatting
        format_dec_text(0,0,IndexCircuit,IndexTextCircuit);
        format_dec_text(0,1,MiddleCircuit,MiddleTextCircuit);
        format_dec_text(0,2,RingCircuit,RingTextCircuit);
        format_dec_text(0,3,PinkyCircuit,PinkyTextCircuit);
        // FSR Raw Data Formatting
        format_dec_text(0,5,IndexFSR,IndexTextFSR);
        format_dec_text(0,6,MiddleFSR,MiddleTextFSR);
        format_dec_text(0,7,RingFSR,RingTextFSR);
        format_dec_text(0,8,PinkyFSR,PinkyTextFSR);
        SysTick_Wait10ms(200);
    }
    clearScreen(ST7735_GREEN);
    return 15;
}