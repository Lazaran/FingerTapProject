// Includes
#include <stdint.h>
#include <string.h>
#include "SysTick.h"
#include "FTP_GFX.h"
#include "FTP_TETRIS.h"
#include "FTP_TYPES.h"



/* Notes:
        Ensure that rotation can only occur above the BlockMass boundary line 
        Tetromino will consist of 1 color, and 4 positions relative to the position
        To keep the en
        The following array is filled in order according to the reference list at the bottom
*/ 

/* Vector2 bitmap of Alien from Space Invaders*/
Vector2 Alien[46] = {
{2,0},{8,0},{3,1},{7,1},{2,2},{3,2},{4,2},
{5,2},{6,2},{7,2},{8,2},{1,3},{2,3},{4,3},
{5,3},{6,3},{8,3},{9,3},{0,4},{1,4},{2,4},
{3,4},{4,4},{5,4},{6,4},{7,4},{8,4},{9,4},
{10,4},{0,5},{2,5},{3,5},{4,5},{5,5},
{6,5},{7,5},{8,5},{10,5},{0,6},{2,6},
{8,6},{10,6},{3,7},{4,7},{6,7},{7,7},
};

/* Vector2 bitmap of White porition of Spaceship from Space Invaders*/
Vector2 StarshipWhite[83] = {
    {7,0},{7,1},{7,2},{6,3},{7,3},{8,3},{6,3},{7,3},{8,3},
    {6,4},{7,4},{8,4},{3,5},{5,5},{6,5},{7,5},{8,5},{9,5},
    {11,5},{3,6},{5,6},{6,6},{8,6},{9,6},{11,6},{0,7},{4,7},
    {5,7},{9,7},{10,7},{14,7},{0,8},{3,8},{4,8},{5,8},{7,8},
    {9,8},{10,8},{11,8},{14,8},{0,9},{2,9},{3,9},{4,9},{5,9},
    {6,9},{7,9},{8,9},{9,9},{10,9},{11,9},{12,9},{14,9},{0,10},
    {1,10},{2,10},{3,10},{4,10},{6,10},{7,10},{8,10},{10,10},
    {11,10},{12,10},{13,10},{14,10},{0,11},{1,11},{2,11},
    {6,11},{7,11},{8,11},{12,11},{13,11},{14,11},{0,12},
    {1,12},{7,12},{13,12},{14,12},{0,13},{7,13},{14,13}
};

/* Vector2 bitmap of Red portion of Spaceship from Space Invaders*/
Vector2 StarshipRed[24] = {
    {3,3},{3,4},{11,3},{11,4},{0,5},{0,6},{14,5},
    {14,6},{7,6},{6,7},{7,7},{8,7},{6,8},{8,8},
    {4,12},{5,12},{4,11},{5,11},{5,10},{9,12},
    {10,12},{9,11},{10,11},{9,10}
};

/* Vector2 bitmap of Blue portion of Spaceship from Space Invaders*/
Vector2 StarshipBlue[4] = {
    {4,6},{3,7},{10,6},{11,7}
};

/* Color type array of defined colors */
Color TetrominoColors[11] = {
    ST7735_BLACK,
    ST7735_BLUE ,
    ST7735_RED  ,
    ST7735_GREEN,
    ST7735_CYAN ,
    ST7735_MAGENTA,
    ST7735_YELLOW,
    ST7735_WHITE,
    ST7735_ORANGE,
    ST7735_NEONGREEN,
    ST7735_GRAY 
};

/* Color type array storing colors of each 
    fallen Mino to draw for the MinoMatrix */
Color arrMatrix[20] = {
    ST7735_BLACK,
    ST7735_GREEN,
    ST7735_YELLOW,
    ST7735_BLUE,
    ST7735_RED,
    ST7735_CYAN,
    ST7735_NEONGREEN,
    ST7735_GRAY,
    ST7735_MAGENTA,
    ST7735_WHITE,
    ST7735_BLACK,
    ST7735_GREEN,
    ST7735_YELLOW,
    ST7735_BLUE,
    ST7735_RED,
    ST7735_CYAN,
    ST7735_NEONGREEN,
    ST7735_GRAY,
    ST7735_MAGENTA,
    ST7735_WHITE
};

/* Array of ActiveTetrominos each intialized with a Fixed Tetromino 
    and respective color and Max Relative Mino Coordinates */ 
ActiveTetromino FixedTetrominos[19] = {
    // Tetromino: I | Orientation: 0
    {{{0,0}, {1,0}, {2,0}, {3,0}}, 0xFFE0, {0,0}, {3,0}},
    // Tetromino:{0,0} I | Orientation: 1
    {{{0,0}, {0,1}, {0,2}, {0,3}}, 0xFFE0, {0,0}, {0,3}},
    // Tetromino:{0,0} O | Orientation: 0
    {{{0,0}, {1,0}, {0,1}, {1,1}}, 0x07FF, {0,0}, {1,1}},
    // Tetromino:{0,0} T | Orientation: 0
    {{{0,0}, {1,0}, {2,0}, {1,1}}, 0xF81F, {0,0}, {2,1}},
    // Tetromino:{0,0} T | Orientation: 1
    {{{0,0}, {0,1}, {1,1}, {0,2}}, 0xF81F, {0,0}, {1,2}},
    // Tetromino:{0,0} T | Orientation: 2
    {{{1,0}, {0,1}, {1,1}, {2,1}}, 0xF81F, {0,0}, {2,1}},
    // Tetromino:{0,0} T | Orientation: 3
    {{{1,0}, {0,1}, {1,1}, {1,2}}, 0xF81F, {0,0}, {1,2}},
    // Tetromino:{0,0} J | Orientation: 0
    {{{1,0}, {1,1}, {0,2}, {1,2}}, 0xF800, {0,0}, {1,2}},
    // Tetromino:{0,0} J | Orientation: 1
    {{{0,0}, {1,0}, {2,0}, {2,1}}, 0xF800, {0,0}, {2,1}},
    // Tetromino:{0,0} J | Orientation: 2
    {{{0,0}, {1,0}, {0,1}, {0,2}}, 0xF800, {0,0}, {1,2}},
    // Tetromino:{0,0} J | Orientation: 3
    {{{0,0}, {0,1}, {1,1}, {2,1}}, 0xF800, {0,0}, {0,3}},
    // Tetromino:{0,0} L | Orientation: 0
    {{{0,0}, {1,0}, {1,1}, {1,2}}, 0xFBC0, {0,0}, {1,2}},
    // Tetromino:{0,0} L | Orientation: 1
    {{{2,0}, {0,1}, {1,1}, {2,1}}, 0xFBC0, {0,0}, {2,1}},
    // Tetromino:{0,0} L | Orientation: 2
    {{{0,0}, {0,1}, {0,2}, {1,2}}, 0xFBC0, {0,0}, {1,2}},
    // Tetromino:{0,0} L | Orientation: 3
    {{{0,0}, {1,0}, {2,0}, {0,1}}, 0xFBC0, {0,0}, {2,1}},
    // Tetromino:{0,0} S | Orientation: 0
    {{{1,0}, {2,0}, {0,1}, {1,1}}, 0x3FE2, {0,0}, {2,1}},
    // Tetromino:{0,0} S | Orientation: 1
    {{{0,0}, {0,1}, {1,1}, {1,2}}, 0x3FE2, {0,0}, {1,2}},
    // Tetromino:{0,0} Z | Orientation: 0
    {{{0,0}, {1,0}, {1,1}, {2,1}}, 0x001F, {0,0}, {2,1}},
    // Tetromino:{0,0} Z | Orientation: 1
    {{{1,0}, {0,1}, {1,1}, {0,2}}, 0x001F, {0,0}, {1,2}}    
};



/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the origin of an ActiveTetromino
  @note might not work because of the pointer issue with structs, minor fix
  @param active An ActiveTetromino object
  @param newPosition A Vector2 with the new position 
  @since February 29, 2024
**********************************************************************/
void Tetromino_SetOrigin(ActiveTetromino active, Vector2 newOrigin){
    active.origin[0] = newOrigin[0];
    active.origin[1] = newOrigin[1];
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the size of an ActiveTetromino
  @note might not work because of the pointer issue with structs, minor fix
  @param active An ActiveTetromino object
  @param newSize A Vector2 with the new size 
  @since February 29, 2024
**********************************************************************/
void Tetromino_SetSize(ActiveTetromino active, Vector2 newSize){
    active.size[0] = newSize[0];
    active.size[1] = newSize[1];
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the color of an ActiveTetromino
  @note might not work because of the pointer issue with structs, minor fix
  @param active An ActiveTetromino object
  @param color A Color with the new color
  @since February 29, 2024
**********************************************************************/
void Tetromino_SetColor(ActiveTetromino active, Color color){
    active.color = color;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the color of an ActiveTetromino
  @note might not work because of the pointer issue with structs,
            additionally memory might not set correctly, biggest fix of all TetrominoSets
  @param active An ActiveTetromino object
  @param color A Color with the new color
  @since February 29, 2024
**********************************************************************/
void Tetromino_SetMinos(ActiveTetromino active, Tetromino newTetromino){
    memcpy(active.minos, newTetromino, sizeof(Tetromino));
}


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Draws the game border around the edge of the screen.
            Bounds the game and is true to the original
  @since February 29, 2024
**********************************************************************/
void DrawMatrixBorder(void){
    for (int i = 0; i < MATRIX_WIDTH; i++){
        for (int j = 0; j < MATRIX_HEIGHT; j++){
            if ((i != 0 && i != (MATRIX_WIDTH-1)) && (j != 0 && j != (MATRIX_HEIGHT-1))){
                continue;
            }
            r_Rect((i*MINO_WIDTH),(j*MINO_HEIGHT),MINO_WIDTH,MINO_HEIGHT,ST7735_GRAY);
        };
    };
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Draws a Tetromino to the screen
  @param active An ActiveTetromino object
  @param color 16-bit color in 5-6-5 format
  @since February 29, 2024
**********************************************************************/
void DrawTetromino(ActiveTetromino active, Color color){
    for (int i = 0; i < 4; i++){
        r_Rect((active.origin[0]+active.minos[i][0])*MINO_WIDTH,(active.origin[1]+active.minos[i][1])*MINO_HEIGHT,MINO_WIDTH,MINO_HEIGHT,color);
    };
};


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Draws the MinoMatrix to the screen, dependent on MinoWidth and Height
  @param active An ActiveTetromino object
  @param color 16-bit color in 5-6-5 format
  @since February 29, 2024
**********************************************************************/
void Matrix_Draw(ActiveMatrix active, Vector1 arrSize, Color colors[arrSize]){
    Vector2 drawPosition = {0,0};
    for (int colorsIndex = 0; colorsIndex < arrSize; colorsIndex++){
        if ((colorsIndex % BORDERED_MATRIX_WIDTH) == 0){
            drawPosition[1] += 1;
        }
        drawPosition[0] = (colorsIndex % BORDERED_MATRIX_WIDTH);
        r_Rect((active.origin[0]+drawPosition[0])*MINO_WIDTH, (active.origin[0]+drawPosition[1])*MINO_HEIGHT, MINO_WIDTH, MINO_HEIGHT, colors[colorsIndex]);
    };
};


void ActiveMatrix_Build(ActiveMatrix active, Vector2 neworigin, Vector2 newsize, Vector1 newmaxheight, Vector1 newarrSize){
    Matrix_SetOrigin(active, neworigin);
    Matrix_SetSize(active, newsize);
    Matrix_SetMaxHeight(active, newmaxheight);
    Matrix_SetArrSize(active, newarrSize);
}


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the origin of an ActiveMatrix
  @param active An ActiveMatrix object
  @param newOrigin A Vector2 with the new origin
  @since February 29, 2024
**********************************************************************/
void Matrix_SetOrigin(ActiveMatrix active, Vector2 newOrigin){
    active.origin [0] = newOrigin[0];
    active.origin[1] = newOrigin[1];

}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the size of an ActiveMatrix
  @param active An ActiveMatrix object
  @param newSize A Vector2 with the new size
  @since February 29, 2024
**********************************************************************/
void Matrix_SetSize(ActiveMatrix active, Vector2 newSize){
    active.size[0] = newSize[0];
    active.size[1] = newSize[1];
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the max height of an ActiveMatrix
  @param active An ActiveMatrix object
  @param newMaxHeight A Vector2 with the new max height
  @since February 29, 2024
**********************************************************************/
void Matrix_SetMaxHeight(ActiveMatrix active, Vector1 newMaxHeight){
    active.maxheight = newMaxHeight;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Sets the array size of an ActiveMatrix
  @param active An ActiveMatrix object
  @param newArrSize A Vector2 with the new matrix size
  @since February 29, 2024
**********************************************************************/
void Matrix_SetArrSize(ActiveMatrix active, Vector1 newArrSize){
    active.arrSize = newArrSize;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Adds a row at position 'newRow' of an ActiveMatrix object
  @param active An ActiveMatrix object
  @param colors Array of Matrix Mino colors
  @param newRow A Vector1 with the new row
  @since February 29, 2024
**********************************************************************/
void Matrix_AddRow(ActiveMatrix active, Color colors[active.arrSize], Vector1 newRow){
    active.arrSize += BORDERED_MATRIX_WIDTH;
    active.size[1] += 1;
    Vector1 tempCount = 0;
    Color* newMatrix = (Color*) calloc(active.arrSize, sizeof(Color));
    for (int row = 0; row < active.size[1]; row++){
        for (int col = 0; col < active.size[0]; col++){
            if (row < newRow){
                newMatrix[tempCount] = colors[tempCount];
            }
            else if (row > newRow){
                newMatrix[tempCount] = colors[tempCount-BORDERED_MATRIX_WIDTH];
            }
        }
    }
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Deletes a row from position 'delRow' of an ActiveMatrix object
  @note Copied from Matrix_AddRow, even less sure this will work as only flipped a couple values
  @param active An ActiveMatrix object
  @param colors Array of Matrix Mino colors
  @param delRow A Vector1 with the row to be deleted
  @since February 29, 2024
**********************************************************************/
void Matrix_DeleteRow(ActiveMatrix active, Color colors[active.arrSize], Vector1 delRow){
    active.arrSize -= BORDERED_MATRIX_WIDTH;
    active.size[1] -= 1;
    Vector1 tempCount = 0;
    Color* newMatrix = (Color*) calloc(active.arrSize, sizeof(Color));
    for (int row = 0; row < active.size[1]; row++){
        for (int col = 0; col < active.size[0]; col++){
            if (row < delRow){
                newMatrix[tempCount] = colors[tempCount];
            }
            else if (row > delRow){
                newMatrix[tempCount] = colors[tempCount+BORDERED_MATRIX_WIDTH];
            }
        }
    }
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Main hyper-loop for Tetris App
  @param exitcode Code entered and returned on game exit
  @since February 29, 2024
**********************************************************************/
void tetris_main(uint8_t exitcode){

    // Might need this to activate minomatrix, need to sort this stuff out
    // Tetromino testBlock;
    // memcpy(testBlock, FixedTetrominos, GetTetrominoSize());

    Vector1 startArrSize = sizeof(arrMatrix)/sizeof(arrMatrix[0]);
    ActiveMatrix MinoMatrix = {{0,BORDERED_MATRIX_HEIGHT-2}, {BORDERED_MATRIX_WIDTH, startArrSize/BORDERED_MATRIX_WIDTH}, (startArrSize/BORDERED_MATRIX_WIDTH), startArrSize};
    // ActiveMatrix_Build(MinoMatrix, AM_newOrigin, AM_newSize, AM_newMaxHeight, AM_newArrSize);
    
    
    DrawMatrixBorder();


    while(exitcode){
        Matrix_Draw(MinoMatrix, MinoMatrix.arrSize, arrMatrix);
        SysTick_Wait10ms(100);
    };
};

