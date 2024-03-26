/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief The main file of the FingerTap Project, operates the main 
            superloop and includes all necessary libraries to run all the
            applications
    @since March 21, 2024
    @version Rev 4
**********************************************************************/
// Includes
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "FTP_MAIN.h"
#include "FTP_TYPES.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_MENU.h"
#include "FTP_SNAKE.h"
#include "FTP_PONG.h"
#include "FTP_TETRIS.h"
#include "FTP_NOTES.h"
#include "FTP_PAINT.h"
#include "FTP_FSRDEMO.h"
#include "FTP_ANALOGDEMO.h"

// Declared WaitForInterrupt for use
void WaitForInterrupt(void);

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initialize a the main AppState
    @param game A Pong_GameState to be modified
    @since March 6, 2024
**********************************************************************/
void init_appstate(App_State *main){
    main->menu_code = 0;
    main->notes_code = 0;
    main->paint_code = 0;
    main->pong_code = 0;
    main->snake_code = 0;
    main->tetris_code = 0;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Operates all libraries and functions
    @since March 21, 2024
**********************************************************************/
int main(void){
    // Initialize AppState main
    App_State main;
    init_appstate(&main);
    // Initialize subsystems
    PLL_Init();
    SysTick_Init();
    Input_Init();
    ST7735_initR();

    // Clear screen
    clearScreen(ST7735_BLACK);
    // Set orientation flag non-zero
    ScreenOrientation = 1;
    while (1){
        // Set screen orientation
        if (ScreenOrientation != 0){
            ScreenOrientation = 0;
            setDirection(ScreenOrientation);
        }
        // Clear screen
        clearScreen(ST7735_BLACK);
        // Enter Main menu program, return selected program code
        main.menu_code = menu_main();
        WaitForInterrupt();
        // Select program to run based on menu selection
        switch(main.menu_code){
            case 1:
                main.snake_code = snake_main();
                break;
            case 2:
                main.pong_code = pong_main();
                break;
            case 3:
                main.tetris_code = tetris_main();
                break;
            case 4:
                main.notes_code = fsr_main();
                break;
            case 5:
                main.paint_code = demo_main();
                break;
            default:
                break;
        };
    SysTick_Wait10ms(500);
   };
};
