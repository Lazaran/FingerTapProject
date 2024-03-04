#ifndef FTP_GAMEOBJ_H
#define FTP_GAMEOBJ_H

#include <stdint.h> 
#include "FTP_TYPES.h"

void SetGO_x(GameObject *active, uint8_t x);

void SetGO_y(GameObject *active, uint8_t y);

void SetGO_w(GameObject *active, uint8_t w);

void SetGO_h(GameObject *active, uint8_t h);

void SetGO_xvel(GameObject *active, uint8_t xvel);

void SetGO_yvel(GameObject *active, uint8_t yvel);

void SetGO_xaccel(GameObject *active, uint8_t xaccel);

void SetGO_yaccel(GameObject *active, uint8_t yaccel);

void SetGO_color(GameObject *active, uint16_t color);

void SetGO_bounce(GameObject *active, uint8_t bounce);

void SetGO_friction(GameObject *active, uint8_t friction);

#endif
