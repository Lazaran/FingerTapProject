/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_NOTES.c, holds macros and function
            prototypes
    @since March 15, 2024
    @version Rev 4
**********************************************************************/

/* File FTP_NOTES.h*/
#ifndef FTP_NOTES_H
#define FTP_NOTES_H

// Includes
#include "FTP_TYPES.h"
#include <stdint.h>

// Macros
#define NOTES_WIDTH 20
#define NOTES_HEIGHT 10
#define CURSOR_WIDTH 1
#define CURSOR_HEIGHT 8
// Without the offset, the cursor would draw over top of the existing letters, causing holes to appear in the text 
#define CURSOR_XOFFSET 1
#define LETTERS_LENGTH 100

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Array holding the all the written text
    @typedef LetterArr
    @since March 15, 2024
**********************************************************************/
typedef char LetterArr[LETTERS_LENGTH];

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Holds all important values for the Notes application to 
            operate, including; Array with all written letters, current
            cursor position, cursor blinking state, current number of 
            written letters, the old number of written letters, application
            exitcode, the analog circuit input code, the FSR input code
    @typedef NoteState
    @since March 15, 2024
**********************************************************************/
typedef struct NoteState{
    LetterArr letters;
    uint16_t cursor_position;
    uint8_t cursor_blink;
    uint8_t written_length;
    uint8_t old_written_length;
    uint8_t exitcode;
    uint8_t pad_code;
    uint8_t FSR_code;
} NoteState;

// Prototypes
void move_cursor(NoteState *notes, uint8_t cursor_move_code);

void add_letter(NoteState *notes);

// void render_array(NoteState *notes);

void render_cursor(NoteState *notes);

void update_notestate(NoteState *notes);

void init_notes(NoteState *notes);

uint8_t notes_main(void);

#endif /* File FTP_NOTES.h*/
