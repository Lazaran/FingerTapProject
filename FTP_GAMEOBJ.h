#ifndef FTP_GAMEOBJ_H
#define FTP_GAMEOBJ_H

#include <stdint.h>

typedef struct GameObject{
	uint16_t color;
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;
	uint8_t xvel;
	uint8_t yvel;
    uint8_t xaccel;
    uint8_t yaccel;
	uint8_t bounce;
	uint8_t friction;
} GameObject;

void SetGO_x(GameObject *active, uint8_t x);

void SetGO_y(GameObject *active, uint8_t y);

void SetGO_w(GameObject *active, uint8_t w);

void SetGO_h(GameObject *active, uint8_t h);

void SetGO_xvel(GameObject *active, uint8_t xvel);

void SetGO_yvel(GameObject *active, uint8_t yvel);

void SetGO_xaccel(GameObject *active, uint8_t xaccel);

void SetGO_yaccel(GameObject *active, uint8_t yaccel);

void SetGO_x(GameObject *active, uint16_t color);

#endif