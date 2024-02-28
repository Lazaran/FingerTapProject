#ifndef FTP_PHYSX_H
#define FTP_PHYSX_H

#include <stdint.h>
#include "FTP_GAMEOBJ.h"

typedef uint8_t Vector2[2];

int p_DetectOutsideCollision(GameObject *active, GameObject *passive);

int p_DetectInsideCollision(GameObject *active, GameObject *passive);

void p_CorrectCollision(GameObject *active, GameObject *passive);

void p_UpdatePosition(GameObject *active);

void p_UpdateVelocity(GameObject *active);

void p_SetPosition(GameObject *active, Vector2 newPosition);

void p_SetVelocity(GameObject *active, Vector2 newVelocity);

void p_SetAcceleration(GameObject *active, Vector2 newAcceleration);

#endif