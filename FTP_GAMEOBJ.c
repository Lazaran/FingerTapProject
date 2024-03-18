#include <stdint.h>
#include "FTP_GAMEOBJ.h"

void UpdateVelocity(GameObject *active){
    active->yvel += active->yaccel; 
    active->xvel += active->xaccel;
}

// Mostly pointless as objects have to be passed into functions either way, have to use arrow notation regardless
// Separate setter functions just add overhead

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set x position of a GameObject
  @param  active GameObject to modify
  @param  x Value for new x position
  @since February 25, 2024
**********************************************************************/
void SetGO_x(GameObject *active, uint8_t x){
		active->x = x;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set y position of a GameObject
  @param  active GameObject to modify
  @param  y Value for new y position
  @since February 25, 2024
**********************************************************************/
void SetGO_y(GameObject *active, uint8_t y){
		active->y = y;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set width of a GameObject
  @param  active GameObject to modify
  @param  w  Value for new width
  @since February 25, 2024
**********************************************************************/
void SetGO_w(GameObject *active, uint8_t w){
    active->w = w;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set height of a GameObject
  @param  active GameObject to modify
  @param  h Value for new height
  @since February 25, 2024
**********************************************************************/
void SetGO_h(GameObject *active, uint8_t h){
	active->h = h;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set x velocity of a GameObject
  @param  active GameObject to modify
  @param  xvel Value for new x velocity
  @since February 25, 2024
**********************************************************************/
void SetGO_xvel(GameObject *active, uint8_t xvel){
		active->xvel = xvel;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set y velocity of a GameObject
  @param  active GameObject to modify
  @param  yvel Value for new y velocity
  @since February 25, 2024
**********************************************************************/
void SetGO_yvel(GameObject *active, uint8_t yvel){
		active->yvel = yvel;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set x acceleration of a GameObject
  @param  active GameObject to modify
  @param  xaccel Value for new x acceleration
  @since February 25, 2024
**********************************************************************/
void SetGO_xaccel(GameObject *active, uint8_t xaccel){
		active->xaccel = xaccel;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set y acceleration of a GameObject
  @param  active GameObject to modify
  @param  yaccel Value for new y acceleration
  @since February 25, 2024
**********************************************************************/
void SetGO_yaccel(GameObject *active, uint8_t yaccel){
		active->yaccel = yaccel;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set color of a GameObject
  @param  active GameObject to modify
  @param  color Value for new 5-6-5 color
  @since February 25, 2024
**********************************************************************/
void SetGO_color(GameObject *active, uint16_t color){
		active->color = color;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set bounce of a GameObject
  @param  active GameObject to modify
  @param  bounce Value for new bounce coefficient
  @since February 25, 2024
**********************************************************************/
void SetGO_bounce(GameObject *active, uint8_t bounce){
		active->bounce = bounce;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set friction of a GameObject
  @param  active GameObject to modify
  @param  friction Value for new friction coefficient
  @since February 25, 2024
**********************************************************************/
void SetGO_friction(GameObject *active, uint8_t friction){
		active->friction = friction;
};



