/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_MENU.c, holds macros and function
            prototypes
    @since March 18, 2024
    @version Rev 3
**********************************************************************/

/* File FTP_MENU.h*/
#ifndef FTP_MENU_H
#define FTP_MENU_H

// Includes
#include <stdint.h>

// Macros
#define MENU_ORIENT 5
#define SELECTOR_MAX 5

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Holds all important values for the Menu to function, including
            the current selector position, the old selector position,
            the selection state and the exitcode to determine which
            program to run
    @typedef NoteState
    @since March 21, 2024
**********************************************************************/
typedef struct Menu_AppState {
    uint8_t selector_position;
    uint8_t old_selector_position;
    uint8_t did_selection;
    uint8_t exit_code;
} Menu_AppState;

// Protypes
void render_tetris_icon(uint8_t x, uint8_t y);

void render_snake_icon(uint8_t x, uint8_t y);

void render_pong_icon(uint8_t x, uint8_t y);

void render_paint_icon(uint8_t x, uint8_t y);

void render_notes_icon(uint8_t x, uint8_t y);

void menu_input(Menu_AppState *menu);

void init_menu(Menu_AppState *menu);

void render_border(Menu_AppState *menu, uint8_t border_select);

void update_appstate(Menu_AppState *menu);

uint8_t menu_main(void);

#endif /* File FTP_MENU.h*/
