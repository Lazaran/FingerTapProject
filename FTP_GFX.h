#ifndef FTP_GFX_H
#define FTP_GFX_H

#include <stdint.h>
	

void rLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void rRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t c);
	
uint8_t dLine(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t width, uint8_t height);

void dRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t, uint16_t c1, uint8_t cToggle, uint16_t c2);

void dCircle(uint8_t x, uint8_t ys);

#endif