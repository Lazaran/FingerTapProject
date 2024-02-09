#include "Glove_ST7735.h"
#include "Glove_GFX.h"
#include "stdint.h"
#include "stdlib.h"

#define PIXEL_ARR_SIZE 500

// From Adafruit_GFX
#ifndef _swap_uint8_t
#define _swap_uint8_t(a, b)                                                    \
  {                                                                            \
    uint8_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

// From Adafruit_GFX
#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b)                                                    \
  {                                                                            \
    uint16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

/* !!!!! IMPORTANT ABOUT THICKNESS !!!!!
  THICKNESS SHOULD ALWAYS GO TOWARD CENTER OF SCREEN
  THIS WAY ONLY THE 4 DIMENSIONS NEED TO BE CONSIDERED
  TO CHECK SHAPE BOUNDARIES
*/



/**************************************************************************/
/*!
  @authors Qwyntyn Scurr, Jonathan W. Valvano 
  @brief Writes the supplied 16-bit color to the ST7735R in two transmissions
  @param c 16-bit color in 5-6-5 format
  @remarks Modified writeData call to fit with Glove_ST7735
  @since February 8, 2024
*/
/**************************************************************************/
void pushColor(uint16_t c) {
  writeData((uint8_t)(c >> 8));
  writeData((uint8_t)c);
}


/*!
  @note Make the border color the inverse of the infill for now, going to have to redesign the FRAM registers to accomodate colors
  @param x
  @param y
  @param w
  @param h
  @param t  Border thickness

*/

void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t){

  // Draw top line, offset down by t
  drawLine(x,y,w,(y+t));
  // Draw bottom line
  // Draw left line
  // Draw right line
  // Draw center fill if needed
}


/*!
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
  @since February 7th, 2024
*/
uint8_t dLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t width, uint8_t height){
  // Checks dimensions are valid, returns error code
  int oCode = dimCheck(x,y,w,h,width,height);
  if (oCode != 0){
    return oCode;
  };
  rLine(x,y,w,h)
  return 0;
}

/**************************************************************************/
/*!
  @author Qwyntyn Scurr
  @brief Draws a line on the ST7735 from point (x,y) to (w,h)
  @note The width and height variables are temporary until I integrate
        current screen rotation into macros or similar so they're
        builtin variables
  @param x X position of the first point
  @param y Y position of the first point
  @param w W position of the second point
  @param h H position of the second point
  @since February 7th, 2024
*/
/**************************************************************************/
void rLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h){

  uint8_t bitmap[2][500] = 
};

/**************************************************************************/
/*!
  @author Qwyntyn Scurr
  @brief Draws a rectangle on the ST7735 from point (x,y) to (w,h)
  @note The width and height variables are temporary until I integrate
        current screen rotation into macros or similar so they're
        builtin variables
  @param x X position of the first point
  @param y Y position of the first point
  @param w W position of the second point
  @param h H position of the second point
  @param c Color of the rectangle
  @since February 7th, 2024
  @returns 0 if all is well
*/
/**************************************************************************/

int rRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t c){
  int pCnt = (abs(w-x)+1) * (abs(y-h)+1);
  setAddrWindow(x,y,w,h);
  for (int i = 0; i < (pCnt); i++){
    pushColor(c);
  };
  return 0;
}


/**************************************************************************/
/*!
  @author Qwyntyn Scurr
  @brief Checks to ensure provided shape bounding-box coordinates
          are within the boundaries of the screen 
  @note The width and height variables are temporary until I integrate
        current screen rotation into macros or similar so they're
        builtin variables
  @param x  X value to boundary check
  @param y  Y value to boundary check
  @param w  W value to boundary check
  @param h  H value to boundary check
  @param width  Screen width
  @param height Screen height
  @since February 7, 2024
*/
/**************************************************************************/
int vScreenCheck(int x, int y, int w, int h, int width, int height){
  if (x < 0 || x > width){
    return 1;
  }
  if (w < 0 || w > width){
    return 2;
  }
  if (y < 0 || y > height){
    return 3;
  }
  if (h < 0 || h > height){
    return 4;
  }
}

