#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "stm32f10x.h"

typedef u8 byte;
extern byte mode;

void TouchExitInit(void);
void EXTI15_10_IRQHandler(void);

#endif


