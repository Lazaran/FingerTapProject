/*!*******************************************************************
    @authors Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
    @brief Interacts with the ST7735R by setting windows of pixels to modify
            Functions build on each other to render more complicated shapes
            to the screen
    @since March 21, 2024
    @version Rev 4
**********************************************************************/

// Includes
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "SysTick.h"

// Inline Declarations
void extern inline v_Clip(uint8_t a, uint8_t b);

int extern inline v_Count(uint16_t x,uint16_t y,uint16_t w,uint16_t h);

// Library Arrays
const unsigned short colors[8] = {
    0x0000,
    0xF800,
    0x001F,
    0x07E0,
    0xFFE0,
    0xF81F,
    0x07FF,
    0xFFFF
};

// standard ascii 5x7 font
// originally from glcdfont.c from Adafruit project
static const uint8_t Font[] = {
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
  0x18, 0x3C, 0x7E, 0x3C, 0x18,
  0x1C, 0x57, 0x7D, 0x57, 0x1C,
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
  0x00, 0x18, 0x3C, 0x18, 0x00,
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
  0x00, 0x18, 0x24, 0x18, 0x00,
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
  0x30, 0x48, 0x3A, 0x06, 0x0E,
  0x26, 0x29, 0x79, 0x29, 0x26,
  0x40, 0x7F, 0x05, 0x05, 0x07,
  0x40, 0x7F, 0x05, 0x25, 0x3F,
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
  0x7F, 0x3E, 0x1C, 0x1C, 0x08,
  0x08, 0x1C, 0x1C, 0x3E, 0x7F,
  0x14, 0x22, 0x7F, 0x22, 0x14,
  0x5F, 0x5F, 0x00, 0x5F, 0x5F,
  0x06, 0x09, 0x7F, 0x01, 0x7F,
  0x00, 0x66, 0x89, 0x95, 0x6A,
  0x60, 0x60, 0x60, 0x60, 0x60,
  0x94, 0xA2, 0xFF, 0xA2, 0x94,
  0x08, 0x04, 0x7E, 0x04, 0x08,
  0x10, 0x20, 0x7E, 0x20, 0x10,
  0x08, 0x08, 0x2A, 0x1C, 0x08,
  0x08, 0x1C, 0x2A, 0x08, 0x08,
  0x1E, 0x10, 0x10, 0x10, 0x10,
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
  0x30, 0x38, 0x3E, 0x38, 0x30,
  0x06, 0x0E, 0x3E, 0x0E, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x5F, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x07, 0x00,
  0x14, 0x7F, 0x14, 0x7F, 0x14,
  0x24, 0x2A, 0x7F, 0x2A, 0x12,
  0x23, 0x13, 0x08, 0x64, 0x62,
  0x36, 0x49, 0x56, 0x20, 0x50,
  0x00, 0x08, 0x07, 0x03, 0x00,
  0x00, 0x1C, 0x22, 0x41, 0x00,
  0x00, 0x41, 0x22, 0x1C, 0x00,
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
  0x08, 0x08, 0x3E, 0x08, 0x08,
  0x00, 0x80, 0x70, 0x30, 0x00,
  0x08, 0x08, 0x08, 0x08, 0x08,
  0x00, 0x00, 0x60, 0x60, 0x00,
  0x20, 0x10, 0x08, 0x04, 0x02,
  0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
  0x00, 0x42, 0x7F, 0x40, 0x00, // 1
  0x72, 0x49, 0x49, 0x49, 0x46, // 2
  0x21, 0x41, 0x49, 0x4D, 0x33, // 3
  0x18, 0x14, 0x12, 0x7F, 0x10, // 4
  0x27, 0x45, 0x45, 0x45, 0x39, // 5
  0x3C, 0x4A, 0x49, 0x49, 0x31, // 6
  0x41, 0x21, 0x11, 0x09, 0x07, // 7
  0x36, 0x49, 0x49, 0x49, 0x36, // 8
  0x46, 0x49, 0x49, 0x29, 0x1E, // 9
  0x00, 0x00, 0x14, 0x00, 0x00,
  0x00, 0x40, 0x34, 0x00, 0x00,
  0x00, 0x08, 0x14, 0x22, 0x41,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x00, 0x41, 0x22, 0x14, 0x08,
  0x02, 0x01, 0x59, 0x09, 0x06,
  0x3E, 0x41, 0x5D, 0x59, 0x4E,
  0x7C, 0x12, 0x11, 0x12, 0x7C, // A
  0x7F, 0x49, 0x49, 0x49, 0x36, // B
  0x3E, 0x41, 0x41, 0x41, 0x22, // C
  0x7F, 0x41, 0x41, 0x41, 0x3E, // D
  0x7F, 0x49, 0x49, 0x49, 0x41, // E
  0x7F, 0x09, 0x09, 0x09, 0x01, // F
  0x3E, 0x41, 0x41, 0x51, 0x73, // G
  0x7F, 0x08, 0x08, 0x08, 0x7F, // H
  0x00, 0x41, 0x7F, 0x41, 0x00, // I
  0x20, 0x40, 0x41, 0x3F, 0x01, // J
  0x7F, 0x08, 0x14, 0x22, 0x41, // K
  0x7F, 0x40, 0x40, 0x40, 0x40, // L
  0x7F, 0x02, 0x1C, 0x02, 0x7F, // M
  0x7F, 0x04, 0x08, 0x10, 0x7F, // N
  0x3E, 0x41, 0x41, 0x41, 0x3E, // O
  0x7F, 0x09, 0x09, 0x09, 0x06, // P
  0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
  0x7F, 0x09, 0x19, 0x29, 0x46, // R
  0x26, 0x49, 0x49, 0x49, 0x32, // S
  0x03, 0x01, 0x7F, 0x01, 0x03, // T
  0x3F, 0x40, 0x40, 0x40, 0x3F, // U
  0x1F, 0x20, 0x40, 0x20, 0x1F, // V
  0x3F, 0x40, 0x38, 0x40, 0x3F, // W
  0x63, 0x14, 0x08, 0x14, 0x63, // X
  0x03, 0x04, 0x78, 0x04, 0x03, // Y
  0x61, 0x59, 0x49, 0x4D, 0x43, // Z
  0x00, 0x7F, 0x41, 0x41, 0x41,
  0x02, 0x04, 0x08, 0x10, 0x20,
  0x00, 0x41, 0x41, 0x41, 0x7F,
  0x04, 0x02, 0x01, 0x02, 0x04,
  0x40, 0x40, 0x40, 0x40, 0x40,
  0x00, 0x03, 0x07, 0x08, 0x00,
  0x20, 0x54, 0x54, 0x78, 0x40, // a
  0x7F, 0x28, 0x44, 0x44, 0x38, // b
  0x38, 0x44, 0x44, 0x44, 0x28, // c
  0x38, 0x44, 0x44, 0x28, 0x7F, // d
  0x38, 0x54, 0x54, 0x54, 0x18, // e
  0x00, 0x08, 0x7E, 0x09, 0x02, // f
  0x18, 0xA4, 0xA4, 0x9C, 0x78, // g
  0x7F, 0x08, 0x04, 0x04, 0x78, // h
  0x00, 0x44, 0x7D, 0x40, 0x00, // i
  0x20, 0x40, 0x40, 0x3D, 0x00, // j
  0x7F, 0x10, 0x28, 0x44, 0x00, // k
  0x00, 0x41, 0x7F, 0x40, 0x00, // l
  0x7C, 0x04, 0x78, 0x04, 0x78, // m
  0x7C, 0x08, 0x04, 0x04, 0x78, // n
  0x38, 0x44, 0x44, 0x44, 0x38, // o
  0xFC, 0x18, 0x24, 0x24, 0x18, // p
  0x18, 0x24, 0x24, 0x18, 0xFC, // q
  0x7C, 0x08, 0x04, 0x04, 0x08, // r
  0x48, 0x54, 0x54, 0x54, 0x24, // s
  0x04, 0x04, 0x3F, 0x44, 0x24, // t
  0x3C, 0x40, 0x40, 0x20, 0x7C, // u
  0x1C, 0x20, 0x40, 0x20, 0x1C, // v
  0x3C, 0x40, 0x30, 0x40, 0x3C, // w
  0x44, 0x28, 0x10, 0x28, 0x44, // x
  0x4C, 0x90, 0x90, 0x90, 0x7C, // y
  0x44, 0x64, 0x54, 0x4C, 0x44, // z
  0x00, 0x08, 0x36, 0x41, 0x00,
  0x00, 0x00, 0x77, 0x00, 0x00,
  0x00, 0x41, 0x36, 0x08, 0x00,
  0x02, 0x01, 0x02, 0x04, 0x02,
  0x3C, 0x26, 0x23, 0x26, 0x3C,
  0x1E, 0xA1, 0xA1, 0x61, 0x12,
  0x3A, 0x40, 0x40, 0x20, 0x7A,
  0x38, 0x54, 0x54, 0x55, 0x59,
  0x21, 0x55, 0x55, 0x79, 0x41,
  0x21, 0x54, 0x54, 0x78, 0x41,
  0x21, 0x55, 0x54, 0x78, 0x40,
  0x20, 0x54, 0x55, 0x79, 0x40,
  0x0C, 0x1E, 0x52, 0x72, 0x12,
  0x39, 0x55, 0x55, 0x55, 0x59,
  0x39, 0x54, 0x54, 0x54, 0x59,
  0x39, 0x55, 0x54, 0x54, 0x58,
  0x00, 0x00, 0x45, 0x7C, 0x41,
  0x00, 0x02, 0x45, 0x7D, 0x42,
  0x00, 0x01, 0x45, 0x7C, 0x40,
  0xF0, 0x29, 0x24, 0x29, 0xF0,
  0xF0, 0x28, 0x25, 0x28, 0xF0,
  0x7C, 0x54, 0x55, 0x45, 0x00,
  0x20, 0x54, 0x54, 0x7C, 0x54,
  0x7C, 0x0A, 0x09, 0x7F, 0x49,
  0x32, 0x49, 0x49, 0x49, 0x32,
  0x32, 0x48, 0x48, 0x48, 0x32,
  0x32, 0x4A, 0x48, 0x48, 0x30,
  0x3A, 0x41, 0x41, 0x21, 0x7A,
  0x3A, 0x42, 0x40, 0x20, 0x78,
  0x00, 0x9D, 0xA0, 0xA0, 0x7D,
  0x39, 0x44, 0x44, 0x44, 0x39,
  0x3D, 0x40, 0x40, 0x40, 0x3D,
  0x3C, 0x24, 0xFF, 0x24, 0x24,
  0x48, 0x7E, 0x49, 0x43, 0x66,
  0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
  0xFF, 0x09, 0x29, 0xF6, 0x20,
  0xC0, 0x88, 0x7E, 0x09, 0x03,
  0x20, 0x54, 0x54, 0x79, 0x41,
  0x00, 0x00, 0x44, 0x7D, 0x41,
  0x30, 0x48, 0x48, 0x4A, 0x32,
  0x38, 0x40, 0x40, 0x22, 0x7A,
  0x00, 0x7A, 0x0A, 0x0A, 0x72,
  0x7D, 0x0D, 0x19, 0x31, 0x7D,
  0x26, 0x29, 0x29, 0x2F, 0x28,
  0x26, 0x29, 0x29, 0x29, 0x26,
  0x30, 0x48, 0x4D, 0x40, 0x20,
  0x38, 0x08, 0x08, 0x08, 0x08,
  0x08, 0x08, 0x08, 0x08, 0x38,
  0x2F, 0x10, 0xC8, 0xAC, 0xBA,
  0x2F, 0x10, 0x28, 0x34, 0xFA,
  0x00, 0x00, 0x7B, 0x00, 0x00,
  0x08, 0x14, 0x2A, 0x14, 0x22,
  0x22, 0x14, 0x2A, 0x14, 0x08,
  0xAA, 0x00, 0x55, 0x00, 0xAA,
  0xAA, 0x55, 0xAA, 0x55, 0xAA,
  0x00, 0x00, 0x00, 0xFF, 0x00,
  0x10, 0x10, 0x10, 0xFF, 0x00,
  0x14, 0x14, 0x14, 0xFF, 0x00,
  0x10, 0x10, 0xFF, 0x00, 0xFF,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x14, 0x14, 0x14, 0xFC, 0x00,
  0x14, 0x14, 0xF7, 0x00, 0xFF,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x14, 0x14, 0xF4, 0x04, 0xFC,
  0x14, 0x14, 0x17, 0x10, 0x1F,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0x1F, 0x00,
  0x10, 0x10, 0x10, 0xF0, 0x00,
  0x00, 0x00, 0x00, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0xF0, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x10,
  0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x10, 0xFF, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x14,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x00, 0x00, 0x1F, 0x10, 0x17,
  0x00, 0x00, 0xFC, 0x04, 0xF4,
  0x14, 0x14, 0x17, 0x10, 0x17,
  0x14, 0x14, 0xF4, 0x04, 0xF4,
  0x00, 0x00, 0xFF, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0xF7, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x17, 0x14,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0xF4, 0x14,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x00, 0x00, 0x1F, 0x10, 0x1F,
  0x00, 0x00, 0x00, 0x1F, 0x14,
  0x00, 0x00, 0x00, 0xFC, 0x14,
  0x00, 0x00, 0xF0, 0x10, 0xF0,
  0x10, 0x10, 0xFF, 0x10, 0xFF,
  0x14, 0x14, 0x14, 0xFF, 0x14,
  0x10, 0x10, 0x10, 0x1F, 0x00,
  0x00, 0x00, 0x00, 0xF0, 0x10,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xFF,
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
  0x38, 0x44, 0x44, 0x38, 0x44,
  0x7C, 0x2A, 0x2A, 0x3E, 0x14,
  0x7E, 0x02, 0x02, 0x06, 0x06,
  0x02, 0x7E, 0x02, 0x7E, 0x02,
  0x63, 0x55, 0x49, 0x41, 0x63,
  0x38, 0x44, 0x44, 0x3C, 0x04,
  0x40, 0x7E, 0x20, 0x1E, 0x20,
  0x06, 0x02, 0x7E, 0x02, 0x02,
  0x99, 0xA5, 0xE7, 0xA5, 0x99,
  0x1C, 0x2A, 0x49, 0x2A, 0x1C,
  0x4C, 0x72, 0x01, 0x72, 0x4C,
  0x30, 0x4A, 0x4D, 0x4D, 0x30,
  0x30, 0x48, 0x78, 0x48, 0x30,
  0xBC, 0x62, 0x5A, 0x46, 0x3D,
  0x3E, 0x49, 0x49, 0x49, 0x00,
  0x7E, 0x01, 0x01, 0x01, 0x7E,
  0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
  0x44, 0x44, 0x5F, 0x44, 0x44,
  0x40, 0x51, 0x4A, 0x44, 0x40,
  0x40, 0x44, 0x4A, 0x51, 0x40,
  0x00, 0x00, 0xFF, 0x01, 0x03,
  0xE0, 0x80, 0xFF, 0x00, 0x00,
  0x08, 0x08, 0x6B, 0x6B, 0x08,
  0x36, 0x12, 0x36, 0x24, 0x36,
  0x06, 0x0F, 0x09, 0x0F, 0x06,
  0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x00, 0x10, 0x10, 0x00,
  0x30, 0x40, 0xFF, 0x01, 0x01,
  0x00, 0x1F, 0x01, 0x01, 0x1E,
  0x00, 0x19, 0x1D, 0x17, 0x12,
  0x00, 0x3C, 0x3C, 0x3C, 0x3C,
  0x00, 0x00, 0x00, 0x00, 0x00,
};


/* !!!!! IMPORTANT ABOUT THICKNESS !!!!!
  THICKNESS SHOULD ALWAYS GO TOWARD CENTER OF SCREEN
  THIS WAY ONLY THE 4 DIMENSIONS NEED TO BE CONSIDERED
  TO CHECK SHAPE BOUNDARIES

r-Series: Raw primitives, at the moment only includes rLine and rRect
          These shapes only draw from the coordinates provided
          They do no error checking or clipping, that is the job of d-Series

d-Series: 'Drawn' Higher order primitives, include error checking and clipping
          in their functions then call the needed r-Series functions
          to generate the desired shape

v-Series: Vertex-affecting functions, includes clipping, bounds checking
          and anything else I come up with of the ilk

c-Series: Color-affecting functions, fills and inversions perhaps

t-Series: Testing functions to push the LCD

rRect: Creates a rectangle filled with color c from point x,y to w,h

rLine: Creates a line with color c from point x,y to w,h

dRect: Create a rectangle with a border of color c1, border thickness t,
        infill color c2, from point x,y to w,h
        Checks x,y are in bounds and clips w,h if needed

dLine: Draw a line of thickness t and color c from point x,y to w,h
        Checks x,y are in bounds and clips w,h if needed
        INCOMPLETE
dCircle: Draw a circle with border thickness y, border color c1, infill color c2,
          with a radius of r and centerpoint x,y 
          INCOMPLETE
*/

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Fills a number of pixels with the provided color
  @param vCnt Number of pixels to fill with a color
  @param c A 16-bit color in 5-6-5 format to fill the pixels with
  @since March 21, 2024
**********************************************************************/
void c_Fill(uint16_t vCnt, uint16_t c){
  for (int i = 0; i < (vCnt); i++){
    pushColor(c);
  };
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Clears the screen by selecting the entire screen and writing a single color to it
  @param c 16-bit color in 5-6-5 format
  @since February 24, 2024
**********************************************************************/
void clearScreen(uint16_t c){
  setAddrWindow(ScreenX,ScreenY,ScreenW,ScreenH);  
  c_Fill(v_Count(ScreenX,ScreenY,ScreenW,ScreenH),c);
};

/*!*******************************************************************
  @author Qwyntyn Scurr
  @brief Draws a line on the ST7735 from point (x,y) to (w,h)
  @note The width and height variables are temporary until I integrate
        current screen rotation into macros or similar so they're
        builtin variables
  @param x X position of the first point
  @param y Y position of the first point
  @param w W position of the second point
  @param h H position of the second point
  @since February 7, 2024
**********************************************************************/
void r_Line(uint8_t x, uint8_t y, uint8_t w, uint8_t h){

  uint8_t bitmap[2][500] = {0,0};
};


/*!*******************************************************************
  @author Qwyntyn Scurr
  @brief Draws a rectangle filled with color c from point x,y to w,h
  @param x X position of the first point
  @param y Y position of the first point
  @param w W position of the second point
  @param h H position of the second point
  @param c Color of the rectangle
  @since February 7, 2024
**********************************************************************/
void r_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t c){
  setAddrWindow(x,y,x+w,y+h);
  c_Fill(v_Count(x,y,x+w,y+h),c);
};

/*!*******************************************************************
  @author Qwyntyn Scurr
  @brief Draws a color to the pixel at (x,y)
  @param x X position of the first point
  @param y Y position of the first point
  @param c Color of the rectangle
  @since February 25, 2024
**********************************************************************/
void r_Pixel(uint8_t x, uint8_t y, uint16_t c){
  setAddrWindow(x,y,x,y);
  pushColor(c);
};

/*!*******************************************************************
  @authors StackOverflow
  @brief  Non-uniform random number from min to max N
  @param  min_n Bottom limit
  @param  max_n Top limit
  @since February 25, 2024
**********************************************************************/
int t_Random(int min_n, int max_n)
{
    return rand() % (max_n - min_n + 1) + min_n;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Draws a random color pixel to each position of the LCD
  @since February 25, 2024
**********************************************************************/
void t_RandomPixels (){
  int maxC = sizeof(colors) / sizeof(colors[0]);
	for (int x = 0; x < ST7735_TFTWIDTH; x++){
		for (int y = 0; y < ST7735_TFTHEIGHT; y++){
			r_Pixel(x,y,colors[t_Random(0,maxC)]);
			SysTick_Wait10ms(1);
		};
	};
};

/*!*******************************************************************  
  @author Qwyntyn Scurr
  @brief Calls rLine, but first ensures that (x,y) and (w,h) are valid coordinates on the LCD screen
  @note The width and height variables are temporary until I integrate
        current screen rotation into macros or similar so they're
        builtin variables
  @note Has a buggering HUGE array inside it because VSCode refuses to acknowledge
        VLAs for now and I can't be bothered to go digging atm, so temp mega array.
  @param x X position of the first point
  @param y Y position of the first point
  @param w W position of the second point
  @param h H position of the second point
  @since February 7, 2024
**********************************************************************/
//uint8_t dLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t width, uint8_t height){
//  // Checks dimensions are valid, returns error code
//  int oCode = dimCheck(x,y,w,h,width,height);
//  if (oCode != 0){
//    return oCode;
//  };
//  rLine(x,y,w,h);
//  return 0;
//};

/*!*******************************************************************
  @author Qwyntyn Scurr
  @brief Draw a rectangle with a border of color c1, border thickness t,
        infill color c2, from point x,y to w,h.
        Clips w,h if necessary
  @param x X position of first point
  @param y Y position of first point
  @param w X position of second point
  @param h Y position of second point
  @param t Border thickness
  @param c1 Border color
  @param cToggle Infill color toggle
  @param c2 Infill color
  @since February 9, 2024
**********************************************************************/
void d_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t, uint16_t c1, uint8_t cToggle, uint16_t c2){
  // Clipping shape to screen if needed
  if (y+h >= HEIGHT){ v_Clip((y+h),HEIGHT-1); };
  if (w+x >= WIDTH){ v_Clip((x+w),WIDTH-1); };
  if (x < 0){ v_Clip(x, 0); };
  if (y < 0){ v_Clip(y, 0); };

  // Draw top border, offset down by t
  r_Rect(x,y,w,t,c1);
  // Draw bottom border, offset up by t
  r_Rect(x,(y+h-t),w,t,c1);
  // Draw left border, offset right by t
  r_Rect(x,y,t,h,c1);
  // Draw right border, offset left by t
  r_Rect((x+w-t),y,t,h,c1);
  // Draw center fill if toggled
  if (cToggle){
    r_Rect((x+t),(y+t),(w-(2*t)),(h-(2*t)),c2);
  }
}

/*!*******************************************************************
  @author  Jesse Donat, Qwyntyn Scurr
  @brief Draws a circle to the screen with a centerpoint at (x0,y0), variable
          borderwith, optional toggle for infill/border only drawing
  @param x0 X position of the centerpoint of the circle
  @param y0 Y position of the centerpoint of the circle
  @param radius X position of second point
  @param borderColor 16-bit 5-6-5 color for the border
  @param borderThickness Border thickness in pixels
  @param fillColor 16-bit 5-6-5 color to fill circle with
  @since March 15, 2024
**********************************************************************/
// void d_Circle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t borderColor, uint8_t borderThickness, uint16_t fillColor) {
//     // Clipping shape to screen if needed
//     if (x0 + radius >= WIDTH) { v_Clip((x0 + radius), WIDTH - 1); }
//     if (y0 + radius >= HEIGHT) { v_Clip((y0 + radius), HEIGHT - 1); }
//     if (x0 - radius < 0) { v_Clip(x0 - radius, 0); }
//     if (y0 - radius < 0) { v_Clip(y0 - radius, 0); }

//     // Number of slices to approximate the circle
//     int numSlices = 10;

//     // Draw the circle's border
//     for (int i = 0; i < numSlices; i++) {
//         // Calculate the angle for this slice
//         int angle = floor(2 * 3.14 * i / numSlices);

//         // Calculate the height of the rectangle for this slice
//         int height = radius * cos(angle);

//         // Calculate the width of the rectangle for this slice
//         int width = radius * sin(angle);

//         // Draw the rectangle for this slice
//         r_Rect(x0 - width, y0 - height, width * 2, height * 2, borderColor);
//     }

//     // Fill the circle's interior if fillColor is different from borderColor
//     if (fillColor != borderColor) {
//         for (int i = 0; i < numSlices; i++) {
//             // Calculate the angle for this slice
//             int angle = floor(2 * 3.14 * i / numSlices);

//             // Calculate the height of the rectangle for this slice
//             int height = radius * cos(angle);

//             // Calculate the width of the rectangle for this slice
//             int width = radius * sin(angle);

//             // Draw the rectangle for this slice
//             r_Rect(x0 - width + borderThickness, y0 - height + borderThickness, width * 2 - borderThickness * 2, height * 2 - borderThickness * 2, fillColor);
//         }
//     }
// }

/*!*******************************************************************  
  @author Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @brief String draw function. 16 rows (0 to 15) and 21 characters (0 to 20)
          Requires (11 + size*size*6*8) bytes of transmission for each character
  @param x columns from the left edge (0 to 20)
  @param y rows from the top edge (0 to 15)
  @param pt pointer to a null terminated string to be printed
  @param textColor 16-bit color of the characters
  @returns num of characters printed
  @since March 3, 2024
**********************************************************************/
uint8_t d_DrawString(uint8_t x, uint8_t y, char *pt, uint16_t textColor){
  int8_t count = 0;
  if(y>15) return 0;
  while(*pt){
    d_DrawCharS(x*6, y*10, *pt, textColor, ST7735_BLACK, 1);
    pt++;
    x = x+1;
    if(x>20) return count;  // number of characters printed
    count++;
  }
  return count;  // number of characters printed
}

/*!*******************************************************************  
  @author Qwyntyn Scurr, Limor Fried/Ladyada, Jonathan W. Valvano 
  @brief Simple character draw function.  This is the same function from
          Adafruit_GFX.c but adapted for this processor.  However, each call
          to ST7735_DrawPixel() calls setAddrWindow(), which needs to send
          many extra data and commands.  If the background color is the same
          as the text color, no background will be printed, and text can be
          drawn right over existing images without covering them with a box.
          Requires (11 + 2*size*size)*6*8 (image fully on screen; textcolor != bgColor)
  @param x horizontal position of the top left corner of the character, columns from the left edge
  @param y vertical position of the top left corner of the character, rows from the top edge
  @param c character to be printed
  @param textColor 16-bit color of the character
  @param bgColor 16-bit color of the background
  @param size number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
  @since March 3, 2024
**********************************************************************/
void d_DrawCharS(uint8_t x, uint8_t y, char c, uint16_t textColor, uint16_t bgColor, uint8_t size){
  uint8_t line = 0x00; // vertical column of pixels of character in font
  uint8_t col, row;
  if((x >= ScreenW)            || // Clip right
     (y >= ScreenH)           || // Clip bottom
     ((x + 5 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (col = 0; col < 6; col++) {
    line = 0x00;
    if (col != 5){
      line = Font[(c*5)+col];
    };

    for (row = 0; row < 8; row++) {
      if (line & 0x1) {
        if (size != 1){
          r_Rect(x+(col*size), y+(row*size), size, size, textColor); // Big Size
          continue;
        };
        r_Pixel(x+col, y+row, textColor); // Default Size
      } else if (bgColor != textColor) {
        if (size != 1){
          r_Rect(x+col*size, y+row*size, size, size, bgColor); // Big Size
          continue;
        };
        r_Pixel(x+col, y+row, bgColor); // Default Size
      };
      line >>= 1;
    };
  };
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Prints a number and then a string at a position on the LCD
  @param x X position for the string to draw at
  @param y Y position for the string to draw at
  @param num Number to format into drawn string
  @param text String to format into drawn string
  @since March 3, 2024
**********************************************************************/
void format_dec_text(uint8_t x, uint8_t y, int num, char* text){
	char buffer[100];
//    if (sizeof(text)/sizeof(text[0]) <= 10){
        // Format the inputs number and string for display
        snprintf(buffer, 100, "%d %s", num, text);
        // Draw formatted string to LCD
        d_DrawString(x,y,buffer,ST7735_WHITE);
//		};
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Prints a string and then a number at a position on the LCD
  @param x X position for the string to draw at
  @param y Y position for the string to draw at
  @param num Number to format into drawn string
  @param text String to format into drawn string
  @since March 3, 2024
**********************************************************************/
void format_text_dec(uint8_t x, uint8_t y, int num, char* text){
	char buffer[100];
//    if (sizeof(text)/sizeof(text[0]) <= 10){
        // Format the inputs number and string for display
        snprintf(buffer, 100, "%s %d", text, num);
        // Draw formatted string to LCD
        d_DrawString(x,y,buffer,ST7735_WHITE);
//		};
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Prints the under construction logo and text to the screen
  @since March 21, 2024
**********************************************************************/
void under_construction(void){
    d_Rect((ScreenW/8),(ScreenH/8),ScreenW/2, ScreenH/2,3,ST7735_CYAN,0,ST7735_BLACK);
    d_Rect((ScreenW/2),(ScreenH/8),ScreenW/2, ScreenH/2,3,ST7735_RED,0,ST7735_BLACK);
    d_Rect((ScreenW/8),(ScreenH/2),ScreenW/2, ScreenH/2,3,ST7735_YELLOW,0,ST7735_BLACK);
    d_Rect((ScreenW/2),(ScreenH/2),ScreenW/2, ScreenH/2,3,ST7735_GREEN,0,ST7735_BLACK);
    d_DrawString(11,5,"Under",ST7735_WHITE);
    d_DrawString(8,6,"Construction",ST7735_WHITE);
    SysTick_Wait10ms(2000);
};
