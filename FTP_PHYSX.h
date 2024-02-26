#ifndef FTP_PHYSX_H
#define FTP_PHYSX_H

#include <stdint.h>
#include "FTP_GAMEOBJ.h"

void p_Bounce(GameObject *a, GameObject *p);

void p_AABBCollide(GameObject *a, GameObject *p);

void p_ViewCollide(GameObject *a, GameObject *viewport);

void p_PosUpdate(GameObject *a);

void p_VelUpdate(GameObject *a);

void p_PosMod(GameObject *a, uint8_t mod, uint8_t dim);

void p_VelMod(GameObject *a, uint8_t mod, uint8_t dim);

void p_AccelMod(GameObject *a, uint8_t mod, uint8_t dim);

#endif