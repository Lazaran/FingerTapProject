 /*!*******************************************************************
    @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
    @brief Corresponding H file for FTP_GFX.c, holds macros and function
            prototypes
    @since March 21, 2024
    @version Rev 4
**********************************************************************/
/* File FTP_GFX.h*/
#ifndef FTP_GFX_H
#define FTP_GFX_H

// Includes
#include <stdint.h>

// Macros
#define PIXEL_ARR_SIZE 500
#define WIDTH 128
#define HEIGHT 160
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0xF800
#define ST7735_RED     0x001F
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0xFFE0
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0x07FF
#define ST7735_WHITE   0xFFFF
#define ST7735_ORANGE  0xFD20
#define ST7735_NEONGREEN 0x3FE2
#define ST7735_GRAY 0x8410
	
// Inline Definitions
 void inline v_Clip(uint8_t a, uint8_t b){
  a = b;
};

int inline v_Count(uint16_t x,uint16_t y,uint16_t w,uint16_t h){
  return ((abs(w-x)+1) * (abs(y-h)+1));
};

// Prototypes
void r_Pixel(uint8_t x, uint8_t y, uint16_t c);

void clearScreen(uint16_t c);

void c_Fill(uint16_t vCnt, uint16_t c1);

void r_Line(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void r_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t c);
	
// uint8_t d_Line(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t width, uint8_t height);

void d_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t, uint16_t c1, uint8_t cToggle, uint16_t c2);

// void d_Circle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t borderColor, uint8_t borderThickness, uint16_t fillColor);

uint8_t d_DrawString(uint8_t x, uint8_t y, char *pt, uint16_t textColor);

void d_DrawCharS(uint8_t x, uint8_t y, char c, uint16_t textColor, uint16_t bgColor, uint8_t size);

void format_dec_text(uint8_t x, uint8_t y, int num, char* text);

void format_text_dec(uint8_t x, uint8_t y, int num, char* text);

void under_construction(void);

#endif /* File FTP_GFX.h*/
