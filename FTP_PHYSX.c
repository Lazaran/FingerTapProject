#include <stdint.h>
#include "FTP_GAMEOBJ.h"

// Regular collision checks
#define RIGHT_COLL ((a->X) < (p->X+p->W))
#define LEFT_COLL ((a->X+a->W) > (p->X))
#define TOP_COLL ((a->Y) < (p->Y+p->H))
#define BOTTOM_COLL ((a->Y+a->H) > (p->Y))

// Inverted collision checks
#define RIGHT_COLLINV ((a->X) > (p->X+p->W))
#define LEFT_COLLINV ((a->X+a->W) < (p->X))
#define TOP_COLLINV ((a->Y) > (p->Y+p->H))
#define BOTTOM_COLLINV ((a->Y+a->H) < (p->Y))

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Check if an object is entering another object
  @param  a Moving object checking for collisions
  @param  p Object being collided with
  @returns 1 if collision | 0 if no collision
  @since February 25, 2024
**********************************************************************/
int p_ColDetect(GameObject *a, GameObject *p){
	// if (((a->X) < (p->X+p->W)) && ((a->X+a->W) > (p->X)) && ((a->Y) < (p->Y+p->H)) && ((a->Y+a->H) > (p->Y))) {
    //     return 1;
    // };
    // return 0;
    return (RIGHT_COLL && LEFT_COLL && TOP_COLL && BOTTOM_COLL) ? 1 : 0;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Checks if an object is leaving another object
  @param  a Moving object checking for collisions
  @param  p Object being collided with
  @returns 1 if collision | 0 if no collision
  @since February 25, 2024
**********************************************************************/
int p_ColDetectInv(GameObject *a, GameObject *p){
    return (RIGHT_COLLINV && LEFT_COLLINV && TOP_COLLINV && BOTTOM_COLLINV) ? 1 : 0;
}

void p_ColCorrect()



/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Inverts velocity if moving object bounced
  @param  a Moving object checking for collisions
  @param  dim 0 flips X velocity vector | 1 flips Y velocity vector
  @since February 25, 2024
**********************************************************************/
void p_Bounce(GameObject *a, GameObject *p){
    if ((a->X <= p->X) || ((a->X+a->W) >= p->W)){
        a->XVEL *= -1;
        // a->XACCEL *= -1;
    };
    if ((a->Y <= p->Y) || ((a->Y+a->H) >= p->H)){
        a->YVEL *= -1;
        // a->YACCEL *= -1;
    };
};



/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Resolve collision between two objects by moving the a 
            out of the p, determined by its velocity
  @param  a Active object colliding
  @param  p Passive object being collided with
  @since February 25, 2024
**********************************************************************/
void p_CollisionCorrect(GameObject *a, GameObject *p) {   
    // Setting X from incoming direction
    if (a->XVEL > 0){
        a->X = p->X - a->W;
    }
    else if (a->XVEL < 0){
        a->X = p->X + p->W;
    }
    // Setting Y from incoming direction
    if (a->YVEL > 0){
        a->Y = p->Y - a->H;
    }
    else if (a->YVEL < 0){
        a->Y = p->Y + p->H;
    };
};


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Updates a gameobjects' position using its velocity
  @param  a Some GameObject
  @since February 25, 2024
**********************************************************************/
void p_PosUpdate(GameObject *a){
    a->Y += a->YVEL;
    a->X += a->XVEL;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Updates a gameobjects' velocity using its acceleration
  @param  a Some GameObject
  @since February 25, 2024
**********************************************************************/
void p_VelUpdate(GameObject *a){
    a->YVEL += a->YACCEL; 
    a->XVEL += a->XACCEL;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Updates a gameobjects' position using a vector modifier
  @param  a Some GameObject
  @param  vector A vector modifier 
  @since February 25, 2024
**********************************************************************/
void p_PosMod(GameObject *a, uint8_t *vector){
    a->X += vector[0]; 
    a->Y += vector[1];
}


/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Updates a gameobjects' velocity using a vector modifier
  @param  a Some GameObject
  @param  vector A vector modifier 
  @since February 25, 2024
**********************************************************************/
void p_VelMod(GameObject *a, uint8_t *vector){
    a->XVEL += vector[0]; 
    a->YVEL += vector[1];
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Updates a gameobjects' acceleration using a vector modifier
  @param  a Some GameObject
  @param  mod A vector modifier 
  @param  dim X or Y select
  @since February 25, 2024
**********************************************************************/
void p_AccelMod(GameObject *a, uint8_t *vector){
    a->XVEL += vector[0]; 
    a->YVEL += vector[1];
}
