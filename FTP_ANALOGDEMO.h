/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_ANALOGDEMO.c, holds macros and function
            prototypes
    @since March 21, 2024
    @version Rev 1
**********************************************************************/

/* File FTP_ANALOGDEMO.h*/
#ifndef FTP_ANALOGDEMO_H
#define FTP_ANALOGDEMO_H

// Includes
#include <stdint.h>

// Macros
#define DEMO_ORIENT 6

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief The exitcode for the demo program
    @struct Demo_AppState
    @since March 21, 2024
**********************************************************************/
typedef struct Demo_AppState {
    uint8_t exit_code;
} Demo_AppState;

// Prototypes
void init_demo(Demo_AppState *demo);

void analog_input(Demo_AppState *demo);

uint8_t demo_main(void);

#endif /* File FTP_ANALOGDEMO.h*/

