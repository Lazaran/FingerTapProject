#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "stdint.h"
#include "stdlib.h"

#define PIXEL_ARR_SIZE 500
#define WIDTH 128
#define HEIGHT 160

#ifndef vSwap
#define vSwap(a, b){uint8_t t = a; a = b; b = t;};
#endif

#ifndef vClip
#define vClip(a, b){a = b;};
#endif

/* !!!!! IMPORTANT ABOUT THICKNESS !!!!!
  THICKNESS SHOULD ALWAYS GO TOWARD CENTER OF SCREEN
  THIS WAY ONLY THE 4 DIMENSIONS NEED TO BE CONSIDERED
  TO CHECK SHAPE BOUNDARIES
*/

/*
r-Series: Raw primitives, at the moment only includes rLine and rRect
          These shapes only draw from the coordinates provided
          They do no error checking or clipping, that is the job of d-Series

d-Series: 'Drawn' Higher order primitives, include error checking and clipping
          in their functions then call the needed r-Series functions
          to generate the desired shape

v-Series: Vertex-affecting functions, includes clipping, bounds checking
          and anything else I come up with of the ilk

rRect: Creates a rectangle filled with color c from point x,y to w,h
rLine: Creates a line with color c from point x,y to w,h
dRect: Create a rectangle with a border of color c1, border thickness t,
        infill color c2, from point x,y to w,h
        Checks x,y are in bounds and clips w,h if needed
dLine: Draw a line of thickness t and color c from point x,y to w,h
        Checks x,y are in bounds and clips w,h if needed
dCircle: Draw a circle with border thickness y, border color c1, infill color c2,
          with a radius of r and centerpoint x,y
*/


/*!*******************************************************************
  @authors Qwyntyn Scurr, Jonathan W. Valvano 
  @brief Writes the supplied 16-bit color to the ST7735R in two transmissions
  @param c 16-bit color in 5-6-5 format
  @remarks Modified writeData call to fit with Glove_ST7735
  @since February 8, 2024
**********************************************************************/
void pushColor(uint16_t c) {
  writeData((uint8_t)(c >> 8));
  writeData((uint8_t)c);
}


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
  @since February 7th, 2024
**********************************************************************/
void rLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h){

  uint8_t bitmap[2][500] = 0;
};


/*!*******************************************************************
  @author Qwyntyn Scurr
  @brief Draws a rectangle filled with color c from point x,y to w,h
  @param x X position of the first point
  @param y Y position of the first point
  @param w W position of the second point
  @param h H position of the second point
  @param c Color of the rectangle
  @since February 7th, 2024
**********************************************************************/
void rRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t c){
  // Calculate the total number of pixels to iterate over
  int pCnt = (abs(w-x)+1) * (abs(y-h)+1);
  // Set the pixel window to modify
  setAddrWindow(x,y,w,h);
  // Iterate pCnt times, setting the color of each pixel to c
  for (int i = 0; i < (pCnt); i++){
    pushColor(c);
  };
  return 0;
};


/*!*******************************************************************  @author Qwyntyn Scurr
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
**********************************************************************/
uint8_t dLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t width, uint8_t height){
  // Checks dimensions are valid, returns error code
  int oCode = dimCheck(x,y,w,h,width,height);
  if (oCode != 0){
    return oCode;
  };
  rLine(x,y,w,h);
  return 0;
};


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
  @since February 9th, 2024
**********************************************************************/
void dRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t, uint16_t c1, uint8_t cToggle, uint16_t c2){
  // Clipping shape to screen if needed
  if (h >= HEIGHT){ vClip(h,HEIGHT-1) };
  if (w >= WIDTH){ vClip(w,WIDTH-1) };
  if (x < 0){ vClip(x, 0) };
  if (y < 0){ vClip(y, 0) };

  // Draw top border, offset down by t
  rRect(x,y,w,(y+t),c1);
  // Draw bottom border, offset up by t
  rRect(x,(h-t),w,h,c1);
  // Draw left border, offset right by t
  rRect(x,y,(x+t),h,c1);
  // Draw right border, offset left by t
  rRect((w-t),y,w,h,c1);
  // Draw center fill if toggled
  if (cToggle){
    rRect((x+t+1),(y+t+1),(w-t-1),(h-t-1),c2);
  }
}


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
  @since February 9th, 2024
**********************************************************************/
void dCircle(uint8_t x, uint8_t y, ){

}




/*!*******************************************************************
  @deprecated Don't need to check if outside bounds if always in bounds
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
**********************************************************************/
// int vScreenCheck(int x, int y, int w, int h, int width, int height){
//   if (x < 0 || x > width){
//     return 1;
//   }
//   if (w < 0 || w > width){
//     return 2;
//   }
//   if (y < 0 || y > height){
//     return 3;
//   }
//   if (h < 0 || h > height){
//     return 4;
//   }
// }


