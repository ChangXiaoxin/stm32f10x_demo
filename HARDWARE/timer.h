#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"

//------------------------------------
#define TIMERBASE				  100	   //单位为us

#define TIMER_1MS				  (1000/TIMERBASE)
#define TIMER_2MS				  (2000/TIMERBASE)
#define TIMER_5MS				  (5000/TIMERBASE)
#define TIMER_200MS				  (200000/TIMERBASE)
#define TIMER_500MS				· (500000/TIMERBASE)
#define TIMER_1S				  (1000000/TIMERBASE)
#define TIMER_2S				  (2000000/TIMERBASE)
#define TIMER_3S				  (3000000/TIMERBASE)
#define TIMER_5S				  (5000000/TIMERBASE)
#define TIMER_10S				  (10000000/TIMERBASE)

#define TIM3_ARR (100-1) 		//通用定时器时钟72M，分频系数72，计数频率72M/72=1000Khz，计数100次，0.1ms
#define TIM3_PSC (72-1)

void TIM3_init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM1_Init(void);

#endif

