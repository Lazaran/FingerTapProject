
#ifndef FTP_PONG_H
#define FTP_PONG_H

#include <stdint.h>
#include "FTP_GAMEOBJ.h"

void step(GameObject *ball, GameObject *paddle, GameObject *viewport);

int pong_main(void);

#endif


