#include <stdlib.h>
#include <stdint.h>
#include "FTP_ST7735R.h"
#include "FTP_GFX.h"
#include "SysTick.h"


void extern inline v_Swap(uint8_t a, uint8_t b);

void extern inline v_Clip(uint8_t a, uint8_t b);

int extern inline v_Count(uint16_t x,uint16_t y,uint16_t w,uint16_t h);

/* End of FTP_GFX Inlines*/

const unsigned short colors[] = {
    0x0000,
    0xF800,
    0x001F,
    0x07E0,
    0xFFE0,
    0xF81F,
    0x07FF,
    0xFFFF
};


#define PIXEL_ARR_SIZE 500
#define WIDTH 128
#define HEIGHT 160

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

c-Series: Color-affecting functions, fills and inversions perhaps

t-Series: Testing functions to push the LCD

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

void c_Fill(uint16_t vCnt, uint16_t c1){
  for (int i = 0; i < (vCnt); i++){
    pushColor(c1);
  };
};

void c_Bifill(uint16_t vCnt, uint16_t c1, uint16_t c2){
  for (int i = 0; i < (vCnt); i++){
    ((i % 2) == 0) ? pushColor(c1) : pushColor(c2); 
  };
};

void c_Gradient(void){
  // unknown
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
  @since February 7th, 2024
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
  @since February 7th, 2024
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
  @since February 25th, 2024
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
  @since February 9th, 2024
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
// void dCircle(uint8_t x, uint8_t y, ){
// 
// }




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

//void 
/* printing letters to screen
cursor x,y position
for (x <5)
  
for (y< 8)
  font[i][x] > y
  

*/
