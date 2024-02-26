#include <stdint.h>
#include "FTP_GAMEOBJ.h"

// Mostly pointless as objects have to be passed into functions either way, have to use arrow notation regardless
// Separate setter functions just add overhead

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set x position of some GameObject
  @param  active Some GameObject
  @param  x  Some X position
  @since February 25, 2024
**********************************************************************/
void Set_GOX(GameObject *active, uint8_t x){
		active->X = x;
};


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set y position of some GameObject
  @param  active Some GameObject
  @param  x  Some Y position
  @since February 25, 2024
**********************************************************************/
void Set_GOY(GameObject *active, uint8_t y){
		active->Y = y;
};


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set the width of some GameObject
  @param  active Some GameObject
  @param  w  Some width
  @since February 25, 2024
**********************************************************************/
void Set_GOW(GameObject *active, uint8_t w){
    active->W = w;
};


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set height of some GameObject
  @param  active Some GameObject
  @param  h  Some height
  @since February 25, 2024
**********************************************************************/
void Set_GOH(GameObject *active, uint8_t h){
	active->H = h;
};


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set x velocity of some GameObject
  @param  active Some GameObject
  @param  xVel Some X velocity
  @since February 25, 2024
**********************************************************************/
void Set_GOXVEL(GameObject *active, uint8_t xVel){
		active->XVEL = xVel;
};


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set y velocity of some GameObject
  @param  active Some GameObject
  @param  yVel Some Y velocity
  @since February 25, 2024
**********************************************************************/
void Set_GOYVEL(GameObject *active, uint8_t yVel){
		active->YVEL = yVel;
};


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set x acceleration of some GameObject
  @param  active Some GameObject
  @param  xAccel Some X acceleration
  @since February 25, 2024
**********************************************************************/
void Set_GOXACCEL(GameObject *active, uint8_t xAccel){
		active->XACCEL = xAccel;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set y acceleration of some GameObject
  @param  active Some GameObject
  @param  yAccel Some Y acceleration
  @since February 25, 2024
**********************************************************************/
void Set_GOYACCEL(GameObject *active, uint8_t yAccel){
		active->YACCEL = yAccel;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set color of some GameObject
  @param  active Some GameObject
  @param  color Some 5-6-5 color
  @since February 25, 2024
**********************************************************************/
void Set_GOCOLOR(GameObject *active, uint16_t color){
		active->COLOR = color;
};

