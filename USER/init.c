#include "init.h"
#include "timer.h"
#include "blink.h"

void TopInit(void){

	BlinkInit();					  //Show my logo by led.	
	//**********************************************************//
	
	//**********************************************************//
	TIM3_init(TIM3_ARR,TIM3_PSC);	  //通用定时器时钟72M，分频系数72，计数频率72M/72=1000Khz，计数100次，0.1ms
	
};

