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

#define CODE0 0xf800
#define CODE1 0xffe0

void Ws2812Delay(unsigned int delay){
	while(delay--);
}

//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);						//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);						//使能SPI1时钟
 
	//GPIOFB3,4,5初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;				//PB3,4,5复用功能输出,3-SCK脚 4-MISO脚 5-MOSI脚	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;										//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;										//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);													//初始化
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//84/32=2.6Mhz  一个时钟周期时间为384ns
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_CalculateCRC(SPI1,DISABLE);
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设 
		 
} 
void Ws2812bInit(void){
	SPI1_Init();
	// GPIO_InitTypeDef GPIO_InitStructure;
	
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	// GPIO_Init(GPIOB, &GPIO_InitStructure);
	// GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	
	
};

/* 发送一个帧数据,同时接收一个帧数据 */
uint8_t SPI_SendData( uint8_t data)
{	
	uint16_t timeout=0x2710;   //10,000
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET) //寄存器的状态读取可以随时就行，这个不受SPI是否在传输数据的影响
		if((timeout--)==0) return printf("发送等待失败!\n");

	SPI_I2S_SendData(SPIx, data);
	timeout=0x2710;          //10,000次循环无果后为失败
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)==RESET)
		if((timeout--)==0) return printf("接收等待失败!\n");
		
	return SPI_I2S_ReceiveData(SPIx);
}


void Code0(void){
	SPI_SendData((uint8_t)CODE0>>8);
	SPI_SendData((uint8_t)CODE0);
}

void Code1(void){
	SPI_SendData((uint8_t)CODE1>>8);
	SPI_SendData((uint8_t)CODE1);
}
// void Code0(void){			//Write 0 code.
	// WSLED = 1;
	// Ws2812Delay(1);
	// /*
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); 			
												29
	// */
	// WSLED = 0;
	// Ws2812Delay(2);
	// /*
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP();					
		// */
		// 61
// }

// void Code1(void){				//Write 1 code.
	// WSLED = 1;
	// Ws2812Delay(2);
	// /*
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); 
												58
	// */
	// WSLED = 0;
	// Ws2812Delay(1);
	// /*
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
	// __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    // __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	
    // __NOP(); __NOP(); 
			// */
		32
// }

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
