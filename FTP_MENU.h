/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Corresponding H file for FTP_MENU
    @since March 4, 2024
    @version Rev 3
**********************************************************************/

#ifndef FTP_MENU_H
#define FTP_MENU_H

#include <stdint.h>

// Screen orientation for Main Menu
#define MENU_DIR 5
#define SELECTOR_MAX 5

typedef struct Menu_AppState {
    uint8_t selector_position;
    uint8_t old_selector_position;
    uint8_t did_selection;
    uint8_t exit_code;
} Menu_AppState;

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

#endif