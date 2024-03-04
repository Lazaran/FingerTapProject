#ifndef FTP_PHYSX_H
#define FTP_PHYSX_H

#include <stdint.h>
#include "FTP_TYPES.h"

void t_Bounce(GameObject *active);

int p_DetectEnterCollision(GameObject *active, GameObject *passive);

uint8_t p_DetectExitCollisionAABB(GameObject *active, GameObject *passive);

void p_CorrectCollision(GameObject *active, uint8_t collisionCode);

void p_UpdateVelocity(GameObject *active);

void p_UpdatePosition(GameObject *active);

void p_SetPosition(GameObject *active, Vector2 newPosition);

void p_SetVelocity(GameObject *active, Vector2 newVelocity);

void p_SetAcceleration(GameObject *active, Vector2 newAcceleration);

#endif
