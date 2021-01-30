#ifndef __FLOWER_H__
#define __FLOWER_H__
#include "stm32f10x.h"
#define MODE_SLEEPING 0
#define MODE_BLOOM 3
#define MODE_BLOOMING 4
#define MODE_BLOOMED 5
#define MODE_FADE 6
#define MODE_FADING 7
#define MODE_FADED 8
#define MODE_FALLINGASLEEP 9
#define MODE_RAINBOW 90
typedef u8 byte;

void FlowerInit(void);
void FlowerTask(void);
void ChangeMode(byte NewMode);

#endif



