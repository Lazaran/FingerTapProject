/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_NOTES
    @since March 4, 2024
    @version Rev 3
**********************************************************************/

#ifndef FTP_NOTES_H
#define FTP_NOTES_H

#include "FTP_TYPES.h"
#include <stdint.h>

#define LETTERS_LENGTH 100

typedef char LetterArr[LETTERS_LENGTH];

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

void move_cursor(NoteState *notes, uint8_t cursor_move_code);

void add_letter(NoteState *notes);

// void render_array(NoteState *notes);

void render_cursor(NoteState *notes);

void update_notestate(NoteState *notes);

void init_notes(NoteState *notes);

uint8_t notes_main(void);

#endif