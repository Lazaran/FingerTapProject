#ifndef FTP_GAMEOBJ_H
#define FTP_GAMEOBJ_H

#include <stdint.h>

typedef struct  {
	uint8_t X;
	uint8_t Y;
	uint8_t W;
	uint8_t H;
	uint8_t XVEL;
	uint8_t YVEL;
    uint8_t XACCEL;
    uint8_t YACCEL;
	uint16_t COLOR;
} GameObject;

void Set_GOX(GameObject *active, uint8_t x);

void Set_GOY(GameObject *active, uint8_t y);

void Set_GOW(GameObject *active, uint8_t w);

void Set_GOH(GameObject *active, uint8_t h);

void Set_GOXVEL(GameObject *active, uint8_t xVel);

void Set_GOYVEL(GameObject *active, uint8_t yVel);

void Set_GOXACCEL(GameObject *active, uint8_t xAccel);

void Set_GOYACCEL(GameObject *active, uint8_t yAccel);

void Set_GOCOLOR(GameObject *active, uint16_t color);

#endif