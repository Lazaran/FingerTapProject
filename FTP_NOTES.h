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

#endif