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
} uVector2;

typedef struct {
    int8_t x;
    int8_t y;
} sVector2;


#endif