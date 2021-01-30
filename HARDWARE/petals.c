#include "petals.h"
#include "timer.h"
//Petals pin:

//TIM2	-ch1  PA0
//TIM2	-ch2  PA1
//TIM2	-ch3  PA2
//TIM2	-ch4  PA3

//TIM4	-ch1  PB6
//TIM4	-ch2  PB7
//TIM4	-ch3  PB8
//TIM4	-ch4  PB9


void PetalsInit(void){
	TIM2_PWM_Init(TIM2_ARR, TIM2_PSC);	//Initial Timer2 PWM. 
	TIM4_PWM_Init(TIM4_ARR, TIM4_PSC);	//Initial Timer4 PWM.
}
	
void PetalsPWMSet(u8 duty){
	TIM_SetCompare1(TIM2, duty);			//Set PWM duty.
	TIM_SetCompare2(TIM2, duty);
	TIM_SetCompare3(TIM2, duty);
	TIM_SetCompare4(TIM2, duty);
	
	TIM_SetCompare1(TIM4, duty);			//Set PWM duty.
	TIM_SetCompare2(TIM4, duty);
	TIM_SetCompare3(TIM4, duty);
	TIM_SetCompare4(TIM4, duty);
}

