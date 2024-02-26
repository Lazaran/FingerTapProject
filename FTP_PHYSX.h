#ifndef FTP_PHYSX_H
#define FTP_PHYSX_H

#include <stdint.h>
#include "FTP_GAMEOBJ.h"

void p_Bounce(GameObject *active, GameObject *passive);

void p_AABBCollide(GameObject *active, GameObject *passive);

void p_ViewCollide(GameObject *active, GameObject *viewport);

void p_PosUpdate(GameObject *active);

void p_VelUpdate(GameObject *active);

void p_PosMod(GameObject *active, uint8_t mod, uint8_t dim);

void p_VelMod(GameObject *active, uint8_t mod, uint8_t dim);

void p_AccelMod(GameObject *active, uint8_t mod, uint8_t dim);

#endif