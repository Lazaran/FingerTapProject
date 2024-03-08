/* File FTP_TYPES.h*/
#ifndef FTP_TYPES_H
#define FTP_TYPES_H

#include <stdint.h>

/* General Core Types: Rely on standard types */
typedef uint16_t Color;

/* Members: uint8_t X, uint8_t Y */
typedef struct {
    uint8_t x;
    uint8_t y;
} u8Vector2;

typedef struct {
    int8_t x;
    int8_t y;
} s8Vector2;

typedef struct {
    uint16_t x;
    uint16_t y;
} u16Vector2;

typedef struct {
    int16_t x;
    int16_t y;
} s16Vector2;


#endif