/*!*******************************************************************
    @authors Qwyntyn Scurr
    @brief Holds custom types used by all programs
    @since Feb 15, 2024
    @version Rev 4
**********************************************************************/

/* File FTP_TYPES.h*/
#ifndef FTP_TYPES_H
#define FTP_TYPES_H

#include <stdint.h>

// Color variable type derived from 2-byte unsigned integer
typedef uint16_t Color;

// 2-Dimensional Vector using 1-byte unsigned integers
typedef struct u8Vector2 {
    uint8_t x;
    uint8_t y;
} u8Vector2;

// 2-Dimensional Vector using 1-byte signed integers
typedef struct s8Vector2 {
    int8_t x;
    int8_t y;
} s8Vector2;

// 2-Dimensional Vector using 2-byte unsigned integers
typedef struct u16Vector2 {
    uint16_t x;
    uint16_t y;
} u16Vector2;

// 2-Dimensional Vector using 2-byte signed integers
typedef struct s16Vector2 {
    int16_t x;
    int16_t y;
} s16Vector2;

#endif
