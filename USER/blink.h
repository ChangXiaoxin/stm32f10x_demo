#ifndef __BLINK_H__
#define __BLINK_H__
#include "stm32f10x.h"
#include "sys.h"

#define BLINK_LED PBout(12)		//blink led line to PA12.


void BlinkInit(void);		//blink led gpio pin initial
void BlinkTask(void);		//250ms reflash request.

#endif
