#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "FTP_INPUT.h"
#include "FTP_GAMEOBJ.h"
#include "FTP_PHYSX.h"
#include "FTP_PONG.h"
#include "FTP_TETRIS.h"
#include "string.h"

Color RefColors[11] = {
    0x0000,
    0xF800,
    0x001F,
    0x07E0,
    0xFFE0,
    0xF81F,
    0x07FF,
    0xFFFF,
    0xFD20,
    0x3FE2,
    0x8410
};

void DrawPenis(Color color){
    //Shaft and Tip
    for (int w = 0; w < 6; w++){
        FixedTetrominos[4].MinoPos[0] = 6;
        FixedTetrominos[4].MinoPos[1] = 2+w;
        DrawTetromino(FixedTetrominos[4], color);
        FixedTetrominos[6].MinoPos[0] = 3;
        FixedTetrominos[6].MinoPos[1] = 2+w;
        DrawTetromino(FixedTetrominos[6], color);
    }
    for (int q = 0; q < 6; q++){
        FixedTetrominos[4].MinoPos[0] = 5;
        FixedTetrominos[4].MinoPos[1] = 4+q;
        DrawTetromino(FixedTetrominos[4], color);
    }

    //Left Nut
    FixedTetrominos[3].MinoPos[0] = 2+0;
    FixedTetrominos[3].MinoPos[1] = 5+8;
    DrawTetromino(FixedTetrominos[3], color);
    FixedTetrominos[3].MinoPos[0] = 3+0;
    FixedTetrominos[3].MinoPos[1] = 5+8;
    DrawTetromino(FixedTetrominos[3], color);
    FixedTetrominos[4].MinoPos[0] = 5+0;
    FixedTetrominos[4].MinoPos[1] = 3+8;
    DrawTetromino(FixedTetrominos[4], color);
    FixedTetrominos[4].MinoPos[0] = 5+0;
    FixedTetrominos[4].MinoPos[1] = 2+8;
    DrawTetromino(FixedTetrominos[4], color);
    FixedTetrominos[5].MinoPos[0] = 2+0;
    FixedTetrominos[5].MinoPos[1] = 1+8;
    DrawTetromino(FixedTetrominos[5], color);
    FixedTetrominos[5].MinoPos[0] = 3+0;
    FixedTetrominos[5].MinoPos[1] = 1+8;
    DrawTetromino(FixedTetrominos[5], color);
    FixedTetrominos[6].MinoPos[0] = 1+0;
    FixedTetrominos[6].MinoPos[1] = 3+8;
    DrawTetromino(FixedTetrominos[6], color);
    FixedTetrominos[6].MinoPos[0] = 1+0;
    FixedTetrominos[6].MinoPos[1] = 2+8;
    DrawTetromino(FixedTetrominos[6], color);
    FixedTetrominos[5].MinoPos[0] = 2+0;
    FixedTetrominos[5].MinoPos[1] = 2+8;
    DrawTetromino(FixedTetrominos[5], color);
    FixedTetrominos[5].MinoPos[0] = 2+0;
    FixedTetrominos[5].MinoPos[1] = 3+8;
    DrawTetromino(FixedTetrominos[5], color);

    //Right Nut
    FixedTetrominos[3].MinoPos[0] = 2+4;
    FixedTetrominos[3].MinoPos[1] = 5+8;
    DrawTetromino(FixedTetrominos[3], color);
    FixedTetrominos[3].MinoPos[0] = 3+4;
    FixedTetrominos[3].MinoPos[1] = 5+8;
    DrawTetromino(FixedTetrominos[3], color);
    FixedTetrominos[4].MinoPos[0] = 5+4;
    FixedTetrominos[4].MinoPos[1] = 3+8;
    DrawTetromino(FixedTetrominos[4], color);
    FixedTetrominos[4].MinoPos[0] = 5+4;
    FixedTetrominos[4].MinoPos[1] = 2+8;
    DrawTetromino(FixedTetrominos[4], color);
    FixedTetrominos[5].MinoPos[0] = 2+4;
    FixedTetrominos[5].MinoPos[1] = 1+8;
    DrawTetromino(FixedTetrominos[5], color);
    FixedTetrominos[5].MinoPos[0] = 3+4;
    FixedTetrominos[5].MinoPos[1] = 1+8;
    DrawTetromino(FixedTetrominos[5], color);
    FixedTetrominos[6].MinoPos[0] = 1+4;
    FixedTetrominos[6].MinoPos[1] = 3+8;
    DrawTetromino(FixedTetrominos[6], color);
    FixedTetrominos[6].MinoPos[0] = 1+4;
    FixedTetrominos[6].MinoPos[1] = 2+8;
    DrawTetromino(FixedTetrominos[6], color);
    FixedTetrominos[5].MinoPos[0] = 2+4;
    FixedTetrominos[5].MinoPos[1] = 2+8;
    DrawTetromino(FixedTetrominos[5], color);
    FixedTetrominos[5].MinoPos[0] = 2+4;
    FixedTetrominos[5].MinoPos[1] = 3+8;
    DrawTetromino(FixedTetrominos[5], color);
};


void DrawArray(uint8_t x, uint8_t y, Vector2 active[], uint8_t arrSize, Color color){
    for (int j = 0; j < arrSize; j++){
        r_Rect((x+active[j][0])*MINO_WIDTH, (y+active[j][1])*MINO_HEIGHT, MINO_WIDTH, MINO_HEIGHT, color);
    }
    // for (int i = 0; i < (sizeof(Alien)/sizeof(Alien[0])); i++){
    //     r_Rect((x+StarshipBlue[i][0])*MINO_WIDTH, (y+StarshipBlue[i][1])*MINO_HEIGHT, MINO_WIDTH, MINO_HEIGHT, ST7735_BLUE);
    // }
    // for (int i = 0; i < (sizeof(Alien)/sizeof(Alien[0])); i++){
    //     r_Rect((x+StarshipRed[i][0])*MINO_WIDTH, (y+StarshipRed[i][1])*MINO_HEIGHT, MINO_WIDTH, MINO_HEIGHT, ST7735_RED);
    // }
}

// void DrawAlien(uint8_t x, uint8_t y){
//     for (int i = 0; i < (sizeof(Alien)/sizeof(Alien[0])); i++){
//         r_Rect((x+Alien[i][0])*MINO_WIDTH, (y+Alien[i][1])*MINO_HEIGHT, MINO_WIDTH, MINO_HEIGHT, ST7735_NEONGREEN);
//     }
// }


int main(void){
    int starX = 2;
    int starY = 2;
    
    int alienX = 5;
    int alienY = 20;
    SysTick_Init();
    PLL_Init();
    ST7735_initR();
    clearScreen(ST77XX_BLACK);
    // DrawMatrixBorder();
    DrawArray(starX, starY, StarshipBlue, 4, ST7735_BLUE);
    DrawArray(starX, starY, StarshipRed, 24, ST7735_RED);
    DrawArray(starX, starY, StarshipWhite, 83, ST7735_WHITE);

    while (1){
        // FixedTetrominos[3].MinoPos[0] = 4;
        // FixedTetrominos[3].MinoPos[1] = 4;
        // DrawTetromino(FixedTetrominos[3], FixedTetrominos[3].MinoColor);
        // SysTick_Wait10ms(250);

        // for (int i = 0; i < 19; i++){
        //     FixedTetrominos[i].MinoPos[0] = 4;
        //     FixedTetrominos[i].MinoPos[1] = 1;
        //     for (int j = 1; j < 16; j++){
        //             DrawTetromino(FixedTetrominos[i], ST7735_BLACK);
        //             FixedTetrominos[i].MinoPos[1] = j;
        //             DrawTetromino(FixedTetrominos[i], FixedTetrominos[i].MinoColor);
        //             SysTick_Wait10ms(250);
        //         };
        //     };
        // };
        // for (int i = 0; i < 11; i++){
        //     DrawPenis(RefColors[i]);
        //     SysTick_Wait10ms(250);
        // }
        int i;
        for (i = 0; i < 15; i++){
            DrawArray(alienX+i-1, alienY, Alien, 46, ST7735_BLACK);
            DrawArray(alienX+i, alienY, Alien, 46, ST7735_NEONGREEN);
            SysTick_Wait10ms(25);
        }
        for (i = 14; i > 0; i--){
            DrawArray(alienX+i+1, alienY, Alien, 46, ST7735_BLACK);
            DrawArray(alienX+i, alienY, Alien, 46, ST7735_NEONGREEN);
            SysTick_Wait10ms(25);
        }

        for (i = 0; i < 15; i++){
            DrawArray(starX+i-1, starY, StarshipBlue, 4, ST7735_BLACK);
            DrawArray(starX+i-1, starY, StarshipRed, 24, ST7735_BLACK);
            DrawArray(starX+i-1, starY, StarshipWhite, 83, ST7735_BLACK);
            DrawArray(starX+i, starY, StarshipBlue, 4, ST7735_BLUE);
            DrawArray(starX+i, starY, StarshipRed, 24, ST7735_RED);
            DrawArray(starX+i, starY, StarshipWhite, 83, ST7735_WHITE);
            SysTick_Wait10ms(50);
        }
        for (i = 14; i > 0; i--){
            DrawArray(starX+i+1, starY, StarshipBlue, 4, ST7735_BLACK);
            DrawArray(starX+i+1, starY, StarshipRed, 24, ST7735_BLACK);
            DrawArray(starX+i+1, starY, StarshipWhite, 83, ST7735_BLACK);
            DrawArray(starX+i, starY, StarshipBlue, 4, ST7735_BLUE);
            DrawArray(starX+i, starY, StarshipRed, 24, ST7735_RED);
            DrawArray(starX+i, starY, StarshipWhite, 83, ST7735_WHITE);
            SysTick_Wait10ms(50);
        }



        





    };

};
