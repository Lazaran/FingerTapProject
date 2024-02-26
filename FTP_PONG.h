
#ifndef FTP_PONG_H
#define FTP_PONG_H

#include <stdint.h>

void orderedUpdate(struct GameObject *ball, struct GameObject *paddle, struct GameObject *viewport);

int pong_main(void);

#endif


