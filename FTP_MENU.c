/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief The main menu of the FingerTap, displays all available apps
            and all apps return to this menu either on menuover or by
            exiting the application.
    @since March 4, 2024
    @version Rev 3
**********************************************************************/
#include <stdint.h>
#include "SysTick.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_ST7735R.h"
#include "FTP_MENU.h"

// Array holding the X position of all icon origins
uint8_t ICON_X[5] = {
    20,70,120,
      30,70
};

// Array holding the Y position of all icon origins
uint8_t ICON_Y[5] = {
    20, 20, 20,
      60, 60
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Renders the Tetris menu Icon to the screen
    @param x X Position of the Icon Origin
    @param y Y Position of the Icon Origin
    @since March 4, 2024
**********************************************************************/
void render_tetris_icon(uint8_t x, uint8_t y){
    // Border
    d_Rect(x+1,y+1,31,31,2,ST7735_GRAY,0,ST7735_BLACK);
    // Blue Tetromino
    d_Rect(x+7,y+17,4,12,1,ST7735_BLUE,1,ST7735_BLUE);
    d_Rect(x+11,y+25,4,4,1,ST7735_BLUE,1,ST7735_BLUE);
    // Red Tetromino
    d_Rect(x+11,y+21,8,4,1,ST7735_RED,1,ST7735_RED);
    d_Rect(x+15,y+25,8,4,1,ST7735_RED,1,ST7735_RED);
    // Purple Tetromino
    d_Rect(x+19,y+21,4,4,1,ST7735_MAGENTA,1,ST7735_MAGENTA);
    d_Rect(x+23,y+17,4,12,1,ST7735_MAGENTA,1,ST7735_MAGENTA);
    // Green Tetromino
    d_Rect(x+13,y+5,4,8,1,ST7735_GREEN,1,ST7735_GREEN);
    d_Rect(x+17,y+9,4,8,1,ST7735_GREEN,1,ST7735_GREEN);
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Renders the Snake menu Icon to the screen
    @param x X Position of the Icon Origin
    @param y Y Position of the Icon Origin
    @since March 4, 2024
**********************************************************************/
void render_snake_icon(uint8_t x, uint8_t y){
    // Border
    d_Rect(x+1,y+1,31,31,2,ST7735_GRAY,0,ST7735_BLACK);
    // Snake
    d_Rect(x+9,y+7,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+9,y+11,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+9,y+15,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+9,y+19,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+9,y+23,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+13,y+23,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+17,y+23,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+21,y+23,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+21,y+19,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+21,y+15,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    d_Rect(x+17,y+15,4,4,1,ST7735_GRAY,1,ST7735_WHITE);
    // Food
    d_Rect(x+17,y+7,4,4,1,ST7735_GREEN,1,ST7735_GREEN);
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Renders the Pong menu Icon to the screen
    @param x X Position of the Icon Origin
    @param y Y Position of the Icon Origin
    @since March 4, 2024
**********************************************************************/
void render_pong_icon(uint8_t x, uint8_t y){
    // Border
    d_Rect(x+1,y+1,31,31,2,ST7735_GRAY,0,ST7735_BLACK);
    // Left Paddle
    d_Rect(x+7,y+7,4,16,1,ST7735_WHITE,1,ST7735_WHITE);
    // Ball
    d_Rect(x+15,y+14,4,4,1,ST7735_WHITE,1,ST7735_WHITE);
    // Right Paddle
    d_Rect(x+23,y+11,4,16,1,ST7735_WHITE,1,ST7735_WHITE);
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Renders the Paint Application Icon to the screen
    @param x X Position of the Icon Origin
    @param y Y Position of the Icon Origin
    @since March 4, 2024
**********************************************************************/
void render_paint_icon(uint8_t x, uint8_t y){
    // Border
    d_Rect(x+1,y+1,31,31,2,ST7735_GRAY,0,ST7735_BLACK);
    // PlaceHolder
    d_Rect(x+4,y+4,16,16,2,ST7735_BLUE,0,ST7735_BLACK);

}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Renders the Notes Application Icon to the screen
    @param x X Position of the Icon Origin
    @param y Y Position of the Icon Origin
    @since March 4, 2024
**********************************************************************/
void render_notes_icon(uint8_t x, uint8_t y){
    // Border
    d_Rect(x+1,y+1,31,31,2,ST7735_GRAY,0,ST7735_BLACK);
    // PlaceHolder
    d_Rect(x+16,y+4,4,16,2,ST7735_BLUE,0,ST7735_BLACK);
    // PlaceHolder
    d_Rect(x+4,y+16,16,4,2,ST7735_GREEN,0,ST7735_BLACK);
}


/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Checks for user input and moves the selector or
            selects an application
    @param menu A Menu_APpState with important values for the menu
    @since March 4, 2024
**********************************************************************/
void menu_input(Menu_AppState *menu){
    switch(Circuit_Parse()){
        case 13:
            if (menu->selector_position == SELECTOR_MAX){
                menu->selector_position = 1;
            }
            else {
                menu->selector_position += 1;
            }
            return;
        case 14:
            menu->did_selection = 1;
            return;
        case 23:
            if (menu->selector_position == 1){
                menu->selector_position = SELECTOR_MAX;
            }
            else {
                menu->selector_position -= 1;
            }
            return;
        default:
            break;
    };
    return;
};

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Initialize a Menu_AppState
    @param menu A Menu_APpState with important values for the menu
    @since March 4, 2024
**********************************************************************/
void init_menu(Menu_AppState *menu){
    menu->did_selection = 0;
    menu->exit_code = 0;
    menu->old_selector_position = 1;
    menu->selector_position = 1;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Renders the 3 possible border colors based on a border_select
    @note Gray = Normal | Yellow = Highlighted | Red = Selected
    @param menu A Menu_AppState with important values for the menu
    @since March 4, 2024
**********************************************************************/
void render_border(Menu_AppState *menu, uint8_t border_select){
    switch(border_select){
        // Normal Border
        case 1:
            d_Rect(ICON_X[menu->old_selector_position-1]+1,ICON_Y[menu->old_selector_position-1]+1,31,31,2,ST7735_GRAY,0,ST7735_BLACK);
            return;
        // Highlighted Border
        case 2:
            d_Rect(ICON_X[menu->selector_position-1]+1,ICON_Y[menu->selector_position-1]+1,31,31,2,ST7735_YELLOW,0,ST7735_BLACK);
            return;
        // Selected Border
        case 3:
            d_Rect(ICON_X[menu->selector_position-1]+1,ICON_Y[menu->selector_position-1]+1,31,31,2,ST7735_RED,0,ST7735_BLACK);
            return;
        default:
            break;
    }
    return;
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Updates the system based on values within a Menu_AppState
    @param menu A Menu_AppState with important values for the application
    @since March 4, 2024
**********************************************************************/
void update_appstate(Menu_AppState *menu){
    if (menu->did_selection){
        render_border(menu, 3);
        SysTick_Wait10ms(500);
        menu->exit_code = 1;
    }
    
    // Check position updates from input
    if (menu->selector_position != menu->old_selector_position){
        render_border(menu, 1);
        render_border(menu, 2);
        menu->old_selector_position = menu->selector_position;
    }
}

/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Render icons, get user input, update the MainMenu AppState, returning
            a code that chooses which app will be run
    @since March 4, 2024
**********************************************************************/
uint8_t menu_main(void){
    setDirection(MENU_DIR);
    Menu_AppState MainMenu;
    init_menu(&MainMenu);
    clearScreen(ST7735_BLACK);
    render_snake_icon(ICON_X[0],ICON_Y[0]);
    render_pong_icon(ICON_X[1],ICON_Y[1]);
    render_tetris_icon(ICON_X[2],ICON_Y[2]);
    render_notes_icon(ICON_X[3],ICON_Y[3]);
    render_paint_icon(ICON_X[4],ICON_Y[4]);

    while (!MainMenu.exit_code){
        menu_input(&MainMenu);
        update_appstate(&MainMenu);
        // Replacing this with a system timer would probably work A LOT BETTER
        // DO ITTTTTT
        SysTick_Wait10ms(250);
    }
    return MainMenu.selector_position;
}