#include "keyboard.h"

/*
	Reads Numeric values from keypad and returns the value once the Key has been pressed
	Input:
	float* value = pointer to the key that has been pressed
	osThreadId* tid_keyboard = pointer to keyboard thread ID used for signal waiting
*/
void readValue(float* value, osThreadId* tid_keyboard){
  uint8_t flag =1;
  int8_t sign = 0;
  uint8_t keyCurrent=0xFF;

  while(flag){
    osSignalWait(0x08, osWaitForever);
    keyCurrent = readKeyboard();
    returnValue(keyCurrent, value, &flag, &sign);
    osDelay(200);
    osSignalClear(*tid_keyboard, 0x08);
  }
}
/*
	Reads a numeric value from the keypad. Returns the value once the Key D has been pressed.
	Input:
	uint8_t keyCurrent = current key that has been pressed
	float* value = value we are reading from keypad, i.e. pitch,roll or time
	uint8_t* flag = flag used to indicate when the read is done, as this function will be used in a loop 
	int8_t* sign = sign of the number, positive or negative. 
*/
void returnValue(uint8_t keyCurrent, float* value, uint8_t* flag, int8_t* sign){

  if(keyCurrent==STAR){
						*sign=1;
					}
	if(keyCurrent==POUND){
						*sign=-1;
					}
	if((sign!=0)&(keyToNumber(keyCurrent)<10)){
            *value =*value*10+keyToNumber(keyCurrent);
					}
	if(keyCurrent == D){
      *value = (*value)*(*sign);
      *flag = 0;
      *sign = 0;
		}
}
/*
	Reads a Key from the keypad.
	Returns the 8bit value corresponding to the key
*/
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
	Converts the value read from the Key to a numeric value.
	Returns 11 if the key that was pressed was not a number
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
/*
	Enables the Keyboard Pins' Interrupts
*/
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
/*
	Disables Interrupts for the Keyboard
*/
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
/*
	Enables the Keyboard
*/
void enableKeyboard(void){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);					// Enable AHB1 Bus with GPIO D pins connections
	enableRowInput();
	enableColumnOutput();
}
/*
	Read the Rows of the Keyboard
	Returns an 8bit value
*/
uint8_t readRow(void){
	uint8_t output=0;
	output = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)<<1);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)<<2);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)<<3);
	return output;
}
/*
	Read the Columns of the Keyboard
	Returns an 8bit value
*/
uint8_t readColumn(void){
	uint8_t output=0;
	output = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)<<1);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9)<<2);
	output = output + (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)<<3);
	return output;
}
/*
	Sets the Values of the Rows to either high or low
	depending on the input
*/
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
/*
	Sets the Values of the Columns to either high or low
	depending on the input
*/
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
/*
	Set Columns to Read Mode
*/
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
/*
	Set Columns to Write Mode
*/
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
/*
	Set Rows to Write Mode
*/
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
/*
	Set Rows to Read Mode
*/
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