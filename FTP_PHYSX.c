#include <stdint.h>
#include "FTP_PHYSX.h"
#include "FTP_GAMEOBJ.h"

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Check if a GameObject is attempting to enter another GameObject
  @param  active GameObject checking for collision
  @param  passive GameObject being checked against
  @returns 1 if collision | 0 if no collision
  @since February 25, 2024
**********************************************************************/
int p_DetectOutsideCollision(GameObject *active, GameObject *passive){
	// if (((active->x) < (passive->x+passive->w)) && ((active->x+active->w) > (passive->x)) && ((active->y) < (passive->y+passive->h)) && ((active->y+active->h) > (passive->y))) {
    //     return 1;
    // };
    // return 0;
    return (((active->x) < (passive->x+passive->w)) && ((active->x+active->w) > (passive->x)) && ((active->y) < (passive->y+passive->h)) && ((active->y+active->h) > (passive->y))) ? 1 : 0;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Check if an object is leaving another object
  @param  active GameObject checking for collision
  @param  passive GameObject being checked against
  @returns 1 if collision | 0 if no collision
  @since February 25, 2024
**********************************************************************/
int p_DetectInsideCollision(GameObject *active, GameObject *passive){
    return ( ((active->x) > (passive->x+passive->w)) && ((active->x+active->w) < (passive->x)) && ((active->y) > (passive->y+passive->h)) && ((active->y+active->h) < (passive->y))) ? 1 : 0;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief The whole point of this function is that it isn't called until the active object
          is inside the passive object, so we check values so the difference is positive.
          If only one of the values is positive/0 then that is the colliding axis
          but if both are positive, the lowest value is the colliding axis because it "just" hit the passive
          while the other axis, if it had been the colliding axis, would have been colliding for a while
          because it is so far inside the passive object.
  @param  active GameObject resolving collision
  @param  passive GameObject collision reference
  @returns 1 if collision | 0 if no collision
  @since February 25, 2024
**********************************************************************/
void p_CorrectCollision(GameObject *active, GameObject *passive){
  int8_t xDifference = -1;
  int8_t yDifference = -1;

  // Check x difference between objects
  if (active->xvel != 0){
    // If active positive x velocity
    if (active->xvel > 0){
      xDifference = (active->x+active->w) - (passive->x);
    }
    // If active negative x velocity
    else {
      xDifference = (passive->x+passive->w) - (active->x);
    };
  };

  // Check y difference between objects
  if (active->xvel != 0){
    // If active positive y velocity
    if (active->xvel > 0){
      yDifference = (active->y+active->h) - (passive->y);
    }
    // If active negative y velocity
    else {
      yDifference = (passive->y+passive->h) - (active->y);
    };
  };
  
  // First check if diffs are equal and if they are non-negative
  if (xDifference == yDifference && xDifference >= 0){
    // Do if diffs aren't 0
    if (yDifference != 0){
      active->y = passive->y - active->h;
    }
    if (xDifference != 0){
      active->x = passive->x - active->w;
    }
    // Do regardless
    active->xvel *= -(active->bounce*passive->friction);
    active->xvel *= -(active->bounce*passive->friction);
    return;
  }
  // Check if ydiff non-negative and is less than xdiff
  else if (yDifference >= 0 && yDifference < xDifference){
    // Do if diff not 0
    if (yDifference != 0){
      active->y = passive->y - active->h;
    }
    // Do regardless
    active->xvel *= -(active->bounce*passive->friction);
    return;
  }
  // Check if xdiff is non-negative and is less than ydiff
  else if (xDifference >= 0 && xDifference < yDifference){
    // Do if diff not 0
    if (xDifference != 0){
      active->x = passive->x - active->w;
    }
    // Do regardless
    active->xvel *= -(active->bounce*passive->friction);
    return;
  };
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Update the position of a gameobject using its velocity
  @param  active GameObject to update
  @since February 26, 2024
**********************************************************************/
void p_UpdatePosition(GameObject *active){
    active->y += active->yvel;
    active->x += active->xvel;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Update the velocity of a GameObject using its acceleration
  @param  active GameObject to update
  @since February 26, 2024
**********************************************************************/
void p_UpdateVelocity(GameObject *active){
    active->yvel += active->yaccel; 
    active->xvel += active->xaccel;
}

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set the position of a GameObject from a Vector2
  @param  active GameObject to modify
  @param  newPosition Vector2 to use
  @since February 26, 2024
**********************************************************************/
void p_SetPosition(GameObject *active, Vector2 newPosition){
    active->x = newPosition[0]; 
    active->y = newPosition[1];
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set the velocity of a GameObject from a Vector2
  @param  active GameObject to modify
  @param  newVelocity Vector2 to use
  @since February 26, 2024
**********************************************************************/
void p_SetVelocity(GameObject *active, Vector2 newVelocity){
    active->xvel = newVelocity[0]; 
    active->yvel = newVelocity[1];
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Set the acceleration of a GameObject from a Vector2
  @param  active GameObject to modify
  @param  newAcceleration Vector2 to use
  @since February 26, 2024
**********************************************************************/
void p_SetAcceleration(GameObject *active, Vector2 newAcceleration){
    active->xvel = newAcceleration[0]; 
    active->yvel = newAcceleration[1];
};
