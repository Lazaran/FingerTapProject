/* File FTP_TYPES.h*/
#ifndef FTP_TYPES_H
#define FTP_TYPES_H

#include <stdint.h>

/* General Core Types: Rely on standard types */
typedef uint16_t Color;
typedef uint8_t Vector1;

/* Members: uint8_t X, uint8_t Y */
typedef struct {
    uint8_t x;
    uint8_t y;
} Vector2;

/* App Core Types: Rely on standard types, app-specific*/
/* Tetris */
typedef uint8_t MinoPile[2];
typedef uint8_t Tetromino[4][2];

/* App Mid Types: Rely on core types, app-specific*/
/* Reference for common inclusions, don't need because pong was getting stupid complex*/
typedef struct GameObject{
	Color color;
	Vector2 origin;
    Vector2 size;
    Vector2 velocity;
    Vector2 acceleration;
	Vector1 bounce;
	Vector1 friction;
} GameObject;

typedef struct PositionGameObject{
    Vector2 origin;
} PositionGameObject;

/* Tetris */
typedef struct ActiveTetromino{
    Tetromino minos;
    Color color;
    Vector2 origin;
    Vector2 size;
} ActiveTetromino;

typedef struct ActiveMatrix{
    Vector2 origin;
    Vector2 size;
    Vector1 maxheight;
    Vector1 arrSize;
} ActiveMatrix;

#endif