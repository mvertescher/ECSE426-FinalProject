#include "keyboard.h"

uint8_t readKeyboard(void){
	
	uint8_t output=0;
	enableRowOutput();
	enableColumnInput();
	setRow(0);
	output = readColumn();
	enableRowInput();
	enableColumnOutput();
	setColumn(0);
	output = (output<<4) + readRow();
	enableKeyboard();
	return output;
}
	/*
	 1 = 0xee, 2 = 0xed, 3 = 0xeb, A = 0xe7
	 4 = 0xde, 5 = 0xdd, 6 = 0xdb, B = 0xd7
	 7 = 0xbe, 8 = 0xbd, 9 = 0xbb, C = 0xb7
	 * = 0x7e, 0 = 0x7d, # = 0x7b, D = 0x77
	*/
uint8_t keyToNumber(uint8_t key){
	if(key==0xEE){
		return 1;
	}
	if(key==0xED){
		return 2;
	}
	if(key==0xEB){
		return 3;
	}
	if(key==0xDE){
		return 4;
	}
	if(key==0xDD){
		return 5;
	}
	if(key==0xDB){
		return 6;
	}
	if(key==0xBE){
		return 7;
	}
	if(key==0xBD){
		return 8;
	}
	if(key==0xBB){
		return 9;
	}
	if(key==0x7D){
		return 0;
	}
	return 11;
}
void enableKeyboardINT(void){
	//Enable SYSCFG Bus to gain access to SYSCFG_EXTICRx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  //Enable External Interrupt
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);
	
  //Define EXTI settings
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line1; 
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_Init(&EXTI_InitStruct);
	
  //Define Settings in NVIC and enable the EXTI
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStruct);
	
}
void disableKeyboardINT(){
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStruct);
}
void enableKeyboard(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);					// Enable AHB1 Bus with GPIO D pins connections
	enableRowInput();
	enableColumnOutput();
}
uint8_t readRow(void){
	uint8_t output=0;
	output = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)<<1);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)<<2);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)<<3);
	return output;
}
uint8_t readColumn(void){
	uint8_t output=0;
	output = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)<<1);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9)<<2);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)<<3);
	return output;
}
void setRow(uint8_t value){
	if(value == 0){
		GPIO_ResetBits (GPIOD, GPIO_Pin_1);
		GPIO_ResetBits (GPIOD, GPIO_Pin_2);
		GPIO_ResetBits (GPIOD, GPIO_Pin_3);
		GPIO_ResetBits (GPIOD, GPIO_Pin_6);
	}
	if(value ==1){
		GPIO_SetBits (GPIOD, GPIO_Pin_1);
		GPIO_SetBits (GPIOD, GPIO_Pin_2);
		GPIO_SetBits (GPIOD, GPIO_Pin_3);
		GPIO_SetBits (GPIOD, GPIO_Pin_6);
	}
}
void setColumn(uint8_t value){
	if(value == 0){
		GPIO_ResetBits (GPIOD, GPIO_Pin_7);
		GPIO_ResetBits (GPIOD, GPIO_Pin_8);
		GPIO_ResetBits (GPIOD, GPIO_Pin_9);
		GPIO_ResetBits (GPIOD, GPIO_Pin_10);
	}
	if(value ==1){
		GPIO_SetBits (GPIOD, GPIO_Pin_7);
		GPIO_SetBits (GPIOD, GPIO_Pin_8);
		GPIO_SetBits (GPIOD, GPIO_Pin_9);
		GPIO_SetBits (GPIOD, GPIO_Pin_10);
	}
}
void enableColumnInput(void){
	//Define the Pin settings and set to Output Mode
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void enableColumnOutput(void){
	//Define the Pin settings and set to Output Mode
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void enableRowOutput(void){
	//Define the Pin settings and set to Output Mode
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
		//Initialize All pins with Nessecary Settings
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void enableRowInput(void){
	//Define the Pin settings and set to Output Mode
	GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
		//Initialize All pins with Nessecary Settings
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}