/* File FTP_GFX.h */
#ifndef FTP_GFX_H
#define FTP_GFX_H

#include <stdint.h>
#include <stdlib.h>
	
void inline v_Swap(uint8_t a, uint8_t b){
  uint8_t t = a; 
  a = b;
  b = t;
};

 void inline v_Clip(uint8_t a, uint8_t b){
  a = b;
};

int inline v_Count(uint16_t x,uint16_t y,uint16_t w,uint16_t h){
  return ((abs(w-x)+1) * (abs(y-h)+1));
};

void r_Pixel(uint8_t x, uint8_t y, uint16_t c);

void clearScreen(uint16_t c);

void c_Fill(uint16_t vCnt, uint16_t c1);

void c_Bifill(uint16_t vCnt, uint16_t c1, uint16_t c2);

void c_Gradient(void);

void r_Line(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void r_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t c);
	
uint8_t d_Line(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t width, uint8_t height);

void d_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t, uint16_t c1, uint8_t cToggle, uint16_t c2);

void d_Circle(uint8_t x, uint8_t ys);

uint8_t d_DrawString(uint8_t x, uint8_t y, char *pt, uint16_t textColor);

void d_DrawCharS(uint8_t x, uint8_t y, char c, uint16_t textColor, uint16_t bgColor, uint8_t size);

#endif /* !FTP_GFX_H */
