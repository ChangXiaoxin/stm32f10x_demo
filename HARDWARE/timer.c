#include "stm32f10x.h"
#include "timer.h"
#include "task.h"

/*
 * 定时器3初始化
 * 定时器3中断初始化：抢占优先级1，子优先级3
 * 输入：arr:计数   psc:分频
 * modification by
 */
u16 seedCount = 65535;
void TIM3_init(u16 arr,u16 psc){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3

	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2	
	
}

/*
 * 定时器3中断：
 * Reflash task flag.
 * 
 */
void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==1){ //溢出中断
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
		TaskRemaks();
		if(seedCount){
			seedCount--;
		}else{
			seedCount = 65535;
		}
	}
}

//Timer2 initial.
//Output pwm in 4 channel
//
void TIM2_PWM_Init(u16 arr,u16 psc){  
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  	        TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 //Enable timer2 clk
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	 //Enable GPIOA clk
	
        //GPIO Pin.
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ; //TIM2_CH1 TIM2_CH2 TIM2_CH3 TIM2_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //Reuse pushes the output.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
        //Initial TIM2
	TIM_TimeBaseStructure.TIM_Period = arr;                     //Sets the value of the auto-reload register cycle of the next update event mount activity.
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //Set the pre-divider value used as a TIMx clock frequency division. 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //Set the clock split:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM up-count mode.
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //Initial TIM_TimeBaseInitStruct 
	
	//Initial TIM2 Channell Channel2 Channel3 Channel4 PWM mode. 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;           //Select timer mode: TIM pulse width modulation mode 2.	
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Compare the output enable.
	TIM_OCInitStructure.TIM_Pulse= 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //Output polarity: TIM output is highly polar.
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);                    //Initial TIM2 OC1
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);                    //Initial TIM2 OC2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);                    //Initial TIM2 OC3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);                    //Initial TIM2 OC4
	
 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //Enable TIM2 CCR2
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //Enable TIM2 CCR2 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //Enable TIM2 CCR2
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //Enable TIM2 CCR2
 
	TIM_Cmd(TIM2, ENABLE);  //Enable TIM2.

}

void TIM4_PWM_Init(u16 arr,u16 psc){  
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  	        TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	 //Enable timer4 clk
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  	 //Enable GPIOB clk
	
        //GPIO Pin.
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ; //TIM4_CH1 TIM4_CH2 TIM4_CH3 TIM4_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //Reuse pushes the output.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
        //Initial TIM4
	TIM_TimeBaseStructure.TIM_Period = arr;                     //Sets the value of the auto-reload register cycle of the next update event mount activity.
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //Set the pre-divider value used as a TIMx clock frequency division. 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //Set the clock split:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM up-count mode.
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);             //Initial TIM_TimeBaseInitStruct 
	
	//Initial TIM4 Channell Channel2 Channel3 Channel4 PWM mode. 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;           //Select timer mode: TIM pulse width modulation mode 2.	
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Compare the output enable.
	TIM_OCInitStructure.TIM_Pulse= 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //Output polarity: TIM output is highly polar.
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);                    //Initial TIM4 OC1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);                    //Initial TIM4 OC2
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);                    //Initial TIM4 OC3
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);                    //Initial TIM4 OC4
	
 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //Enable TIM4 CCR1
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //Enable TIM4 CCR2 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //Enable TIM4 CCR3
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //Enable TIM4 CCR4
 
	TIM_Cmd(TIM4, ENABLE);  //Enable TIM4.

}


void TIM1_Init(void)		//PA8 Channel3
{	
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//Enable GPIOB clk
	                                                                         	
   //????????????,??TIM4?CH4?PWM????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //AFIO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************** 72MHz / 200 / 7200 = 50Hz*******************/
	TIM_TimeBaseStructure.TIM_Period = 200 - 1; //arr
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1; //psc 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //Set the clock split:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM up-count mode.
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //Initial TIM_TimeBaseInitStruct
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Select timer mode: TIM pulse width modulation mode 2.	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Compare the output enable.
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Output polarity: TIM output is highly polar.
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //Idle output polarity: TIM output is lowly polar
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //Initial TIM1 OC1
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //Enable TIM1 CCR1
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);	//PWM output.
	TIM_Cmd(TIM1, ENABLE);  //TIM4
}

