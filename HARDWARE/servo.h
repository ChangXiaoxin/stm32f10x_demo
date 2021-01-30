#ifndef __SERVO_H__
#define __SERVO_H__
#include "stm32f10x.h"


void ServoInit(void);
void ServoControl(uint16_t angle);

#endif


