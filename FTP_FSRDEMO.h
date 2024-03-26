/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_FSRDEMO.c, holds macros and function
            prototypes
    @since March 21, 2024
    @version Rev 1
**********************************************************************/

/* File FTP_FSRDEMO.h*/
#ifndef FTP_FSRDEMO_H
#define FTP_FSRDEMO_H

// Includes
#include <stdint.h>

#define FSR_ORIENT 5

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Important values for the FSR demo appstate, including the 
            input codes for each fingers' FSR and the exitcode for the 
            program
    @struct Demo_AppState
    @since March 21, 2024
**********************************************************************/
typedef struct FSR_AppState {
    uint8_t index_code;
    uint8_t middle_code;
    uint8_t ring_code;
    uint8_t pinky_code;
    uint8_t exit_code;
} FSR_AppState;

// Prototypes
void fsrdemo_input(FSR_AppState *fsrdemo);

void render_FSR(FSR_AppState *fsrdemo, uint8_t toggle);

void init_fsrdemo(FSR_AppState *fsrdemo);

uint8_t fsr_main(void);

#endif /* File FTP_FSRDEMO.h*/