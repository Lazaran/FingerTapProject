#include <stdint.h>
#include <stdlib.h>

#include "FTP_PHYSX.h"
#include "FTP_GAMEOBJ.h"
#include <math.h>



/*
Guy named deftware on reddit, answered the question of someone who was doing pong as well XD
No I just have to figure out why it works and then implement it.
https://www.reddit.com/r/opengl/comments/151kpog/collision_detection/?rdt=63743

Just loop through your objects and compare each one against the rest of the objects.
Each object has an AABB that's defined relative to its position. So like a ball will have an XY position,
and then a fixed AABB relative to that position. A ball that's say 10px large would have an AABB that's (-5,-5)(5,5) for instance.
When comparing AABBs of two objects you just project both of their AABBs into worldspace (i.e. add the object's position to its AABB) and then compare them.
You could just do simple point-in-box checks (i.e. x > minx && x < maxx && y > miny && y < maxy) for all four corners of one AABB against the other AABB, and vice-versa.
For bounce reaction just get the vector between the two objects, normalize it, and then add that vector to the bouncing object's velocity scaled by the dot product between its velocity and the delta vector. 
This will cause the ball to bounce up if it hits toward the top of a paddle, and down if it hits toward the bottom. If you want it to bounce flat and predictably then you just reflect the velocity along the 
axis according to which side it bounced off of which means taking that delta vector and dividing it by the AABB dimensions of the paddle which will normalize it to 'paddle space' and then you just check if the absolute value of the vector's X or Y is larger. 
If X is larger then the ball is bouncing off the sides as opposed to the top/bottom.
If you want to get really crazy you can instead do a segment/AABB intersection test where you check all four corners of your 'ball' in their previous-frame position and current-frame position, 
as four lines, against the paddle's current position. This will allow you to detect collision even if the ball is moving very fast,
because with a simple point-in-aabb check you could end up with the ball being on one side of the paddle and its velocity advances it far enough to pop up on the other side, passing right through the paddle. If your framerate is high enough, 
paddle thick enough, and ball slow enough then you'll never have to worry about it.
You really don't need AABB collision for pong though, just check if the ball is behind the paddle and bounce it - i.e. if its Y position is within the Y of the top/bottom of the paddle, 
and its X position is behind the X position of the paddle, then just invert its X velocity by multiplying by -1.
*/

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Check if a GameObject is attempting to enter another GameObject
  @param  active GameObject checking for collision
  @param  passive GameObject being checked against
  @returns 1 if collision | 0 if no collision
  @since February 25, 2024
**********************************************************************/
int p_DetectEnterCollision(GameObject *active, GameObject *passive){
	// if (((active->x) < (passive->x+passive->w)) && ((active->x+active->w) > (passive->x)) && ((active->y) < (passive->y+passive->h)) && ((active->y+active->h) > (passive->y))) {
    //     return 1;
    // };
    // return 0;
    return (((active->x) < (passive->x+passive->w)) && ((active->x+active->w) > (passive->x)) && ((active->y) < (passive->y+passive->h)) && ((active->y+active->h) > (passive->y))) ? 1 : 0;
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Check if an object is attempting to exit another object
  @param  active GameObject checking for collision
  @param  passive GameObject being checked against
  @returns 1 if collision | 0 if no collision
  @since February 25, 2024
**********************************************************************/
uint8_t p_DetectExitCollisionAABB(GameObject *active, GameObject *passive){
  return ((active->x < passive->x) || (active->y < passive->y) || (active->x + active->w) > (passive->x + passive->w) || (active->y + active->h) > (passive->y + passive->h));
};

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief Test code to do a simple bounce, might be worth expanding
  @param  active GameObject checking for collision
  @since February 27, 2024
**********************************************************************/
void t_Bounce(GameObject *active){
  if (active->xvel != 0){
    active->xvel *= -(active->bounce);
  }
  if (active->yvel != 0){
    active->yvel *= -(active->bounce);
  }
};


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
//void p_CorrectCollision(GameObject *active, GameObject *passive){
//  int xdeltaVector = abs(active->x - passive->x);
//  int ydeltaVector = abs(active->y - passive->y);
//  float magnitude = sqrtf((xdeltaVector^2 + ydeltaVector^2));
//  float xNormal = xdeltaVector / magnitude;
//  float yNormal = ydeltaVector / magnitude;

//  //dot product here for velocity * deltaVector (or maybe velocity magnitude? )
//  

//  float 


//}
  // int8_t xDifference = -1;
  // int8_t yDifference = -1;

  // // Check x difference between objects
  // if (active->xvel != 0){
  //   // If active positive x velocity
  //   if (active->xvel > 0){
  //     xDifference = (active->x+active->w) - (passive->x);
  //   }
  //   // If active negative x velocity
  //   else {
  //     xDifference = (passive->x+passive->w) - (active->x);
  //   };
  // };

  // // Check y difference between objects
  // if (active->xvel != 0){
  //   // If active positive y velocity
  //   if (active->xvel > 0){
  //     yDifference = (active->y+active->h) - (passive->y);
  //   }
  //   // If active negative y velocity
  //   else {
  //     yDifference = (passive->y+passive->h) - (active->y);
  //   };
  // };
  
  // // First check if diffs are equal and if they are non-negative
  // if (xDifference == yDifference && xDifference >= 0){
  //   // Do if diffs aren't 0
  //   if (yDifference != 0){
  //     active->y = passive->y - active->h;
  //   }
  //   if (xDifference != 0){
  //     active->x = passive->x - active->w;
  //   }
  //   // Do regardless
  //   active->xvel *= -(active->bounce*passive->friction);
  //   active->xvel *= -(active->bounce*passive->friction);
  //   return;
  // }
  // // Check if ydiff non-negative and is less than xdiff
  // else if (yDifference >= 0 && yDifference < xDifference){
  //   // Do if diff not 0
  //   if (yDifference != 0){
  //     active->y = passive->y - active->h;
  //   }
  //   // Do regardless
  //   active->xvel *= -(active->bounce*passive->friction);
  //   return;
  // }
  // // Check if xdiff is non-negative and is less than ydiff
  // else if (xDifference >= 0 && xDifference < yDifference){
  //   // Do if diff not 0
  //   if (xDifference != 0){
  //     active->x = passive->x - active->w;
  //   }
  //   // Do regardless
  //   active->xvel *= -(active->bounce*passive->friction);
  //   return;
  // };


  /*list Loop V list loop for the gameobject collision comparison
  have each obejct in a list check for collisions against every other object in the list
  except for itself.
  */ 

//   if ((active->x < passive->x) && (active->xvel < 0)){
//     active->x = passive->x;
//   }
//   else if (((active->x + active->w) < (passive->x + passive->w)) && (active->xvel < 0)){
//     active->x = (passive->x - active->w);
//   }
//   active->xvel *= -(active->bounce);

  
//   if ((active->y < passive->y) && (active->yvel < 0)){
//     active->y = passive->y;
//   }
//   else if (((active->y + active->h) < (passive->y + passive->h)) && (active->yvel < 0)){
//     active->y = (passive->y - active->h);
//   }
//   active->yvel *= -(active->bounce);
// }

/*!*******************************************************************
  @authors Qwyntyn Scurr
  @brief  Update the position of a gameobject using its velocity
  @param  active GameObject to update
  @since February 26, 2024
**********************************************************************/
void p_UpdatePosition(GameObject *active){
    active->y += active->yvel;
    active->x += active->xvel;
};

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
