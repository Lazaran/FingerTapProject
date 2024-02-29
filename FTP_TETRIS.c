#include <stdint.h>
#include <string.h>
#include "SysTick.h"
#include "FTP_TETRIS.h"
#include "FTP_GFX.h"
#include "FTP_GAMEOBJ.h"
#include "FTP_ST7735R.h"
#include "FTP_PHYSX.h"

Color TetrominoColors[7] = {
    0xFFE0,
    0x07FF,
    0xF81F,
    0xF800,
    0xFD20,
    0x3FE2,
    0x001F
};

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
    {{{0,0}, {0,1}, {0,2}, {0,3}}, 0xF800, {0,0}, {0,3}},
    // Tetromino:{0,0} L | Orientation: 0
    {{{0,0}, {1,0}, {1,1}, {1,2}}, 0xFD20, {0,0}, {1,2}},
    // Tetromino:{0,0} L | Orientation: 1
    {{{2,0}, {0,1}, {1,1}, {2,1}}, 0xFD20, {0,0}, {2,1}},
    // Tetromino:{0,0} L | Orientation: 2
    {{{0,0}, {0,1}, {0,2}, {1,2}}, 0xFD20, {0,0}, {1,2}},
    // Tetromino:{0,0} L | Orientation: 3
    {{{0,0}, {1,0}, {2,0}, {0,1}}, 0xFD20, {0,0}, {2,1}},
    // Tetromino:{0,0} S | Orientation: 0
    {{{1,0}, {2,0}, {0,1}, {1,1}}, 0x3FE2, {0,0}, {2,1}},
    // Tetromino:{0,0} S | Orientation: 1
    {{{0,0}, {0,1}, {1,1}, {1,2}}, 0x3FE2, {0,0}, {1,2}},
    // Tetromino:{0,0} Z | Orientation: 0
    {{{0,0}, {1,0}, {1,1}, {2,1}}, 0x001F, {0,0}, {2,1}},
    // Tetromino:{0,0} Z | Orientation: 1
    {{{1,0}, {0,1}, {1,1}, {0,2}}, 0x001F, {0,0}, {1,2}}    
};
/*
    Ensure that rotation can only occur above the BlockMass boundary line 
*/ 

/*
Tetromino will consist of 1 color, and 4 positions relative to the position
To keep the en
*/

/* The following array is filled in order according to the reference list at the bottom
*/


// Tetromino testTetromino = {{0x00,0x1F}, {1,0}, {0,1}, {1,1}, {0,2}};

//Tetromino *SelectTetromino(uint8_t index){
//    Tetromino Selection = FixedTetrominos[index];
//    return Selection;
//};

Vector2 Alien[46] = {
               {2,0},                              {8,0},
                     {3,1},                  {7,1}, 
               {2,2},{3,2},{4,2},{5,2},{6,2},{7,2},{8,2}, 
         {1,3},{2,3},      {4,3},{5,3},{6,3},      {8,3},{9,3},
   {0,4},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4},{7,4},{8,4},{9,4},{10,4}, 
   {0,5},      {2,5},{3,5},{4,5},{5,5},{6,5},{7,5},{8,5},      {10,5}, 
   {0,6},      {2,6},                              {8,6},      {10,6}, 
                     {3,7},{4,7},      {6,7},{7,7},
};

Vector2 StarshipWhite[83] = {
                                                 {7,0},  
                                                 {7,1},  
                                                 {7,2},  
                                         {6,3},  {7,3}, {8,3},              
                                         {6,3},  {7,3}, {8,3},                   
                                         {6,4},  {7,4}, {8,4},                   
                     {3,5},        {5,5},{6,5},  {7,5}, {8,5}, {9,5},         {11,5},              
                     {3,6},        {5,6},{6,6},         {8,6}, {9,6},         {11,6},              
{0,7},                     {4,7},  {5,7},                      {9,7}, {10,7},                         {14,7},   
{0,8},               {3,8},{4,8},  {5,8},        {7,8},        {9,8}, {10,8}, {11,8},                 {14,8},   
{0,9},        {2,9}, {3,9},{4,9},  {5,9},{6,9},  {7,9}, {8,9}, {9,9}, {10,9}, {11,9}, {12,9},         {14,9},   
{0,10},{1,10},{2,10},{3,10},{4,10},      {6,10}, {7,10},{8,10},       {10,10},{11,10},{12,10},{13,10},{14,10},   
{0,11},{1,11},{2,11},                    {6,11}, {7,11},{8,11},                       {12,11},{13,11},{14,11},   
{0,12},{1,12},                                   {7,12},                                      {13,12},{14,12},   
{0,13},                                          {7,13},                                              {14,13},   
};

Vector2 StarshipRed[24] = {
{3,3},
{3,4},
{11,3},
{11,4},
{0,5},
{0,6},
{14,5},
{14,6},
{7,6},
{6,7},{7,7},{8,7},
{6,8},
{8,8},
{4,12},{5,12},
{4,11},{5,11},
{5,10},
{9,12},{10,12},
{9,11},{10,11},
{9,10}
};

Vector2 StarshipBlue[4] = {
    {4,6},{3,7},{10,6},{11,7}
};



Color MinoMatrix[20] = {
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK,
    ST7735_BLACK
};

uint8_t GetTetrominoSize(void){
    return sizeof(FixedTetrominos[0]);
}

void DrawMatrixBorder(){
    for (int i = 0; i < MATRIX_WIDTH; i++){
        for (int j = 0; j < MATRIX_HEIGHT; j++){
            if ((i != 0 && i != (MATRIX_WIDTH-1)) && (j != 0 && j != (MATRIX_HEIGHT-1))){
                continue;
            }
            r_Rect((i*MINO_WIDTH),(j*MINO_HEIGHT),MINO_WIDTH,MINO_HEIGHT,ST7735_GRAY);
        };
    };
};

void DrawTetromino(ActiveTetromino active, Color color){
    if (((active.MinoPos[0]+active.MinoMax[0]) > BORDERED_MATRIX_WIDTH) || ((active.MinoPos[1]+active.MinoMax[1]) > BORDERED_MATRIX_HEIGHT)){
        return;
    }
    for (int i = 0; i < 4; i++){
        r_Rect((active.MinoPos[0]+active.Minos[i][0])*MINO_WIDTH,(active.MinoPos[1]+active.Minos[i][1])*MINO_HEIGHT,MINO_WIDTH,MINO_HEIGHT,color);
    };
};

// void DrawMinoMatrix(){
//     for (int i = 1; i <= sizeof(MinoMatrix)/sizeof(MinoMatrix[0]); i++){
//         if (i % 10)
//     }
// }




void tetris_main(uint8_t exitcode){
    Tetromino testBlock;
    memcpy(testBlock, FixedTetrominos, GetTetrominoSize());

    while(exitcode){
        SysTick_Wait10ms(100);


    };
};

