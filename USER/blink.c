#include "stm32f10x.h"
#include "blink.h"

/*****************************************
*   Code rule.
*	| | | | | | | | | | | | | | | | | | | | | | | | |
*   *.* *  -  *     *.* *  -  *             *....
*
*   Code.
*	|-.-.|....|.-|-.|--.||-..-|..|.-|---||-..-|..|-.|
*
******************************************/
u8 Blink[]={							//Led blink code table.
	1,1,1, 0, 1, 0, 1,1,1, 0, 1, 0,0,0,   //c
	1, 0, 1, 0, 1, 0, 1, 0,0,0,			  //h
	1, 0, 1,1,1, 0,0,0, 				  //a
	1,1,1, 0, 1, 0,0,0,					  //n
	1,1,1, 0, 1,1,1, 0, 1, 0,0,0,         //g
	
	1,1,1, 0, 1, 0, 1, 0, 1,1,1, 0,0,0,   //x
	1, 0, 1, 0,0,0,                       //i
	1, 0, 1,1,1, 0,0,0,                   //a
	1,1,1, 0, 1,1,1, 0, 1,1,1, 0,0,0,     //o
	
	1,1,1, 0, 1, 0, 1, 0, 1,1,1, 0,0,0,   //x
	1, 0, 1, 0,0,0,                       //i
	1,1,1, 0, 1, 0,0,0,					  //n	
	
	0,0,0,0,					          //-----Gap.
};

u8 BlinkNum;
u16 BlinkIndex;


void BlinkInit(void){	//Blink led Pin initial.
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	
	BlinkNum = sizeof(Blink)/sizeof(Blink[0]);
	
};

void LedOn(void){
	BLINK_LED = 0;
}

void LedOff(void){	
	BLINK_LED = 1;
}



void BlinkTask(void){	//Blink task.     
	if(BlinkIndex < (BlinkNum - 1)){
		BlinkIndex++;
	}else{
		BlinkIndex = 0;
	}
	if(Blink[BlinkIndex]){
		LedOn();
	}
	else{
		LedOff();
	}
	
};





