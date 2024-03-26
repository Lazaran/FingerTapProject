/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_MAIN.c, holds macros and function
            prototypes
    @since March 21, 2024
    @version Rev 4
**********************************************************************/

/* File FTP_MAIN.h*/
#ifndef FTP_MAIN_H
#define FTP_MAIN_H

// Includes
#include <stdint.h>

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Important values for the AppState to function, including 
            the selected application menu code and the application exit codes
            themselves
    @struct Pong_GameState
    @since March 21, 2024
**********************************************************************/
typedef struct App_State {
    uint8_t menu_code;
    uint8_t pong_code;
    uint8_t snake_code;
    uint8_t tetris_code;
    uint8_t notes_code;
    uint8_t paint_code;
} App_State;

// Prototypes
void init_appstate(App_State *main);

int main(void);

#endif /* File FTP_MAIN.h*/