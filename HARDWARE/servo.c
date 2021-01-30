#include "servo.h"
#include "timer.h"

//Servo pin:
//TIM1	-ch1  PA8



void ServoInit(void){
	TIM1_Init();	//Initial TIM1 Channel1 output 50Hz PWM.
}
	
void ServoControl(uint16_t angle){
	float temp;
	temp = (1.0 / 9.0) * angle + 5.0;
	TIM_SetCompare1(TIM1, (uint16_t)temp);
}

