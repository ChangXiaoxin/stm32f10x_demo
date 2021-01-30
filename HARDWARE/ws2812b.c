#include "stm32f10x.h"
#include "ws2812b.h"


/*****************************************
*   *	WS2812B driver.
*   * 	G7:G0/R7:R0/B7:B0
*	|first 24bit|second 24bit|third 24bit|....
*	Note: Follow the order of GRB to sent data and the high bit sent at first.
*	           _____
*	  0 code: | T0H |__T0L__|	T0H:  400ns 29nop  T0L:  850ns  61nop  Error: +-150ns
*              _______
*	  1 code: |  T1H  |_T1L_|	T1H:  800ns 58nop  T1L:  450ns  32nop  Error: +-150ns
*               
*	RET code: |______T______|	  T:  >50us
*
******************************************/

void Ws2812Delay(unsigned int delay){
	while(delay--);
}

void Ws2812bInit(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	
	
};

void Code0(void){			//Write 0 code.
	WSLED = 1;
	Ws2812Delay(1);
	/*
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); 			
												//29
	*/
	WSLED = 0;
	Ws2812Delay(2);
	/*
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP();					
		*/
		//61
}

void Code1(void){				//Write 1 code.
	WSLED = 1;
	Ws2812Delay(2);
	/*
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); 
												//58
	*/
	WSLED = 0;
	Ws2812Delay(1);
	/*
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
    __NOP(); __NOP(); 
			*/
		//32
}

void RGB_WByte(u8 byte){				//Write byte code(one color).
	u8 i;
	for(i = 0; i < 8; i++){
		if((byte<<i)&0x80){
			Code1();
		}else{
			Code0();
		}	
	}
}

void Write24Bit(struct RGB_Color_Struct color){
	RGB_WByte(color.green);
	RGB_WByte(color.red);
	RGB_WByte(color.blue);
	
}

void SetOneRgb(struct RGB_Color_Struct* color, u8 index, u8 r, u8 g ,u8 b){
	color[index].red = r;
	color[index].green = g;
	color[index].blue = b;
	
}
void SetOneColor(struct RGB_Color_Struct* color, u8 index, struct RGB_Color_Struct color0){
	color[index] = color0; 
	
}
void SetAllColor(struct RGB_Color_Struct* color, struct RGB_Color_Struct color0){
	for(u8 i = 0; i < sizeof(color)/sizeof(color0); i++){
		SetOneColor(color, i, color0);
	}
};


void ReflashRGB(struct RGB_Color_Struct* color){
	u8 ColorLength = sizeof(color)/sizeof(color[0]);
	u8 i;
	for(i = 0; i < ColorLength; i++){
		Write24Bit(color[i]);
	}
}
