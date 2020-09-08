#ifndef __LASER_H_
#define __LASER_H_

#include "delay.h"

void LaserInit(u32 baudRate);
u16 LaserReadData(u8 ID);
void getLength(void);

#endif


