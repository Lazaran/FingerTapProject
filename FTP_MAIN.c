/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief The main file of the FingerTap Project, operates the main 
            superloop and includes all necessary libraries to run all the
            applications
    @since March 17, 2024
    @version Rev 4
**********************************************************************/

#include <stdio.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_SNAKE.h"
#include "FTP_PONG.h"
#include "FTP_TETRIS.h"
#include "FTP_NOTES.h"
#include "FTP_PAINT.h"
#include "FTP_MENU.h"
// #include "FTP_APPTIMER.h"


char IndexTextCircuit[] = "< Index Circuit";
char MiddleTextCircuit[] = "< Middle Circuit";
char RingTextCircuit[] = "< Ring Circuit";
char PinkyTextCircuit[] = "< Pinky Circuit";
char IndexTextFSR[] = "< Index FSR";
char MiddleTextFSR[] = "< Middle FSR";
char RingTextFSR[] = "< Ring FSR";
char PinkyTextFSR[] = "< Pinky FSR";

void WaitForInterrupt(void);

typedef struct App_State {
    uint8_t menu_code;
    uint8_t pong_code;
    uint8_t snake_code;
    uint8_t tetris_code;
    uint8_t notes_code;
    uint8_t paint_code;
} App_State;

void init_appstate(App_State *appstate){
    appstate->menu_code = 0;
    appstate->notes_code = 0;
    appstate->paint_code = 0;
    appstate->pong_code = 0;
    appstate->snake_code = 0;
    appstate->tetris_code = 0;
}

int main(){
    App_State appstate;
    init_appstate(&appstate);
    PLL_Init();
    SysTick_Init();
    Input_Init();

    ST7735_initR();
    clearScreen(ST77XX_BLACK);
    ScreenOrientation = 1;

    while (1){
        // format_dec_text(0,0,IndexCircuit,IndexTextCircuit);
        // format_dec_text(0,1,MiddleCircuit,MiddleTextCircuit);
        // format_dec_text(0,2,RingCircuit,RingTextCircuit);
        // format_dec_text(0,3,PinkyCircuit,PinkyTextCircuit);

        // format_dec_text(0,5,IndexFSR,IndexTextFSR);
        // format_dec_text(0,6,MiddleFSR,MiddleTextFSR);
        // format_dec_text(0,7,RingFSR,RingTextFSR);
        // format_dec_text(0,8,PinkyFSR,PinkyTextFSR);
        if (ScreenOrientation != 0){
            ScreenOrientation = 0;
            setDirection(ScreenOrientation);
        }
        clearScreen(ST77XX_BLACK);
        appstate.menu_code = menu_main();
        WaitForInterrupt();
        switch(appstate.menu_code){
            case 1:
                appstate.snake_code = snake_main();
                break;
            case 2:
                appstate.pong_code = pong_main();
                break;
            case 3:
                appstate.tetris_code = tetris_main();
                break;
            case 4:
                appstate.notes_code = notes_main();
                break;
            case 5:
                appstate.paint_code = paint_main();
                break;
            default:
                break;
        }
   };
};

/* Check for differences between snake and pong main functions
The menu responds correctly by clearing on gameover for pong
But doesn't work for snake
WHY
also, triple check game display directions, maybe use test rectangles
*/


/* IMPORTANT
Function Calls might need to be shuffled around in all application mains
because some aspects may load faster than others and some require
to load faster, maybe, idk, i think at least.
*/

/* Most of the apps only use a couple of the fingers because the true beauty of the device
is found in the simple tapping of the fingers operating a device single-handedly. It would
be pedantic if the function was arbritrarily spread around the glove, it would in fact 
make it less elegant because of the awkward nature of remembering what buttons do what 
without a significant amount of practice with the device. 

Ironically, the application most apt for using the full functionality of the glove is, in fact,
too expansive to operate to its full potential on this prototype. I didn't include enough pads in 
the plans, partially on purpose, to accomodate the entire English alphabet, so my notes app alphabet
only goes from 'a' to 't'. Frankly, considering the nightmare mess of wiring that the first prototype
is, it would be challenging to create a prototype with the required dexterity with the current
available materials. I would need much better tools to create something that could accomodate the
full english alphabet.

I (know/believe) that the drawing app uses the most of the glove, by having the size and positions of
the shapes set by the FSRs. 1-step undo, 1-step redo, shape type, shape color, infill toggle,
infill color all set by the conductive pads. (Whether I got to it, thats what I planned at least)

*/