#ifndef __PETALS_H__
#define __PETALS_H__
#include "stm32f10x.h"


#define TIM2_ARR (255-1) 		//72分频，72000_000/72  = 1000Khz的计数频率，计数到255为4KHz
#define TIM2_PSC (72-1)

#define TIM4_ARR (255-1) 		//72分频，72000_000/72  = 1000Khz的计数频率，计数到255为4KHz
#define TIM4_PSC (72-1)

void PetalsInit(void);
void PetalsPWMSet(u8 duty);

#endif


