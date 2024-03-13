#include "FTP_NOTES.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"

// Placehodler values, replace with real later
#define NOTES_WIDTH 20
#define NOTES_HEIGHT 10

#define CURSOR_WIDTH 1
#define CURSOR_HEIGHT 8
// Without the offset, the cursor would draw over top of the existing letters, causing holes to appear in the text 
#define CURSOR_XOFFSET 1
/* Pad Codes List
These are the exact codes from the input handler
The pad array puts each of these at index n-1 instead of n
1 = a
2 = b
3 = c
4 = d
5 = e
6 = f
7 = g
8 = h
9 = i
10 = j
11 = k
12 = l
13 = m
14 = n
15 = o
16 = p
17 = q
18 = r
19 = s
20 = t
*/
char PadArray[20] = {
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t'
};

/* FSR Codes
1 = Move Cursor Right | X ++
2 = Move Cursor Left | X --
3 = Move Cursor Down | Y ++
4 = Move Cursor Up | Y --
*/



LetterArr letter_arr_template = 0;

// Move the cursor based on some input move code, either from the FSR or from the letter addition inputs
void move_cursor(NoteState *notes, uint8_t cursor_move_code){
    switch (cursor_move_code)
    {
    case 1:
        notes->cursor_position += 1;
        break;
    case 2:
        notes->cursor_position -= 1;
        break;
    case 3:
        notes->cursor_position += NOTES_WIDTH;
        break;
    case 4:
        notes->cursor_position -= NOTES_WIDTH;
        break;
    default:
        break;
    }
}


// Adds a letter to the letters array at the cursor position
add_letter(NoteState *notes){
    uint16_t i = 0;
    notes->written_length += 1; // Unsure, definitely going to change
    for (i = notes->written_length; i > 0; i--){
        if (i == notes->cursor_position){
            notes->letters[i] = PadArray[notes->pad_code];
            move_cursor(notes, 1);
        } else if (i > notes->cursor_position){
            notes->letters[i] = notes->letters[i-1]; 
        } else if (i < notes->cursor_position){
            continue; // 90% sure this can be a break since all characters are in the same position before the insertion point.
        }
    }
}

// Reads through the letters array and draws it to the screen row by row
// If the new length of the array is the same as the old length, don't redraw
// Reduces unneeded calls, only draws when the array actually changes
render_array(NoteState *notes){
    if (notes->written_length == notes->old_written_length){
        return;
    }
    uint8_t array_height = 1 + ((notes->written_length/NOTES_WIDTH) - ((notes->written_length/NOTES_WIDTH)/NOTES_WIDTH));
    uint8_t i = 0;
    for (i = 0; i < array_height; i++){
        uint8_t j = 0;
        uint8_t array_fragment[NOTES_WIDTH] = 0;
        for (j = 0; j < NOTES_WIDTH; j++){
            array_fragment[((i*NOTES_WIDTH)+j)];
        }
        d_DrawString(0,i,array_fragment,ST7735_WHITE);
    }
    notes->written_length = notes->old_written_length;
}

// Renders the cursor using its index position to convert to x,y coordinates to draw to the screen
// Controls the blinking of the cursor so it is visibly changing on the screen
render_cursor(NoteState *notes){
    uint8_t cursor_x = (notes->cursor_position % NOTES_WIDTH);
    uint8_t cursor_y = ((notes->cursor_position/NOTES_WIDTH) - ((notes->cursor_position/NOTES_WIDTH)/NOTES_WIDTH));
    if (notes->cursor_blink){
        d_Rect(cursor_x*NOTES_WIDTH - 1,cursor_y*NOTES_HEIGHT,CURSOR_WIDTH,CURSOR_HEIGHT,1,ST7735_WHITE,0,ST7735_WHITE);
        notes->cursor_blink = 0;
    } else {
        d_Rect(cursor_x*NOTES_WIDTH - 1,cursor_y*NOTES_HEIGHT,CURSOR_WIDTH,CURSOR_HEIGHT,1,ST7735_WHITE,0,ST7735_WHITE);
        notes->cursor_blink = 1;
    }
}

// Updates the notestate using modifying functions
update_notestate(NoteState *notes){
    if (notes->pad_code != 0){
        add_letter(notes);
    }
    render_array(notes);
    render_cursor(notes);
}

// Initializes a Notestate struct
init_notes(NoteState *notes){
    notes->cursor_position = 0;
    notes->written_length = 0;
    notes->old_written_length = 0;
    // Cant seem to set empty array, need to look at that
    // notes->letters = letter_arr_template;
    notes->pad_code = 0;
    notes->FSR_code = 0;
    notes->exitcode = 0;

}


// The main function for the notes app
uint8_t notes_main(void){
    NoteState NoteApp;
    clearScreen(ST7735_BLACK);
    init_notes(&NoteApp);
    while(1){

    }
}