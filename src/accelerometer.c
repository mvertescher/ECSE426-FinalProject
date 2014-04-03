#include "accelerometer.h"


void init_accelerometer(void) {	
	LIS3DSH_InitTypeDef lis_init_s;
	lis_init_s.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100; 			/* OUT data rate 100 Hz / 400 Hz */
	//lis_init_s.Axes_Enable = LIS3DSH_Y_ENABLE;                    		/* Axes enable */
	lis_init_s.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;	
	//lis_init_s.AA_Filter_BW = LIS3DSH_AA_BW_50;
	lis_init_s.Full_Scale = LIS3DSH_FULLSCALE_2;                  			/* Full scale */
	lis_init_s.Self_Test = LIS3DSH_SELFTEST_NORMAL;  // NORMAL,P,M      /* Self test */
	LIS3DSH_Init(&lis_init_s); //this also configures the low-level interface	
	
	uint8_t ctrl = 0x67; 
	LIS3DSH_Write(&ctrl, LIS3DSH_CTRL_REG4, 1);
	
	ctrl = 0xC8; // INT1
  //ctrl = 0xD0; // INT2	
	LIS3DSH_Write(&ctrl, LIS3DSH_CTRL_REG3, 1);
	
	//ctrl = 0x20; // LIS3DSH_SENSITIVITY_16G
	ctrl = 0xC0; // 50Hz AA filter,  2G sense
	//ctrl = 0xC8; // 50Hz AA filter,  4G sense
	//ctrl = 0xC4; // 50Hz AA filter,  6G sense
	LIS3DSH_Write(&ctrl, LIS3DSH_CTRL_REG5, 1);
	
	//ctrl = 0x10; // Set ADD_INC bit: Register address automatically incremented during a multiple byte access with a serial interface 
	//LIS3DSH_Write(&ctrl, 0x25, 1); // LIS3DSH_CTRL_REG6
	
	ctrl = OFFSET_X;
	LIS3DSH_Write(&ctrl, LIS3DSH_OFF_X, 1);
	ctrl = OFFSET_Y;
	LIS3DSH_Write(&ctrl, LIS3DSH_OFF_Y, 1);
	ctrl = OFFSET_Z;
	LIS3DSH_Write(&ctrl, LIS3DSH_OFF_Z, 1);
}

void init_mems_interrupt(void) {	
	/*
	Write 0x04 in CTRL_REG_3 to enable interrupt generation on data ready with the following config:
	Active High
	Push-Pull
	I2CFG0 bit set high
	
	The data-ready interrupt signal goes high when a new set of acceleration data has been generated in the data registers of the accelerometer. 
	The interrupt goes low when the system has read the high bytes of the data registers of all enabled axes. 
	*/
	//uint8_t buffer = 0x04;
	//LIS3DSH_Write(&buffer, LIS3DSH_CTRL_REG3, 1); 
	//LIS3DSH_Write(&buffer, LIS3DSH_CTRL_REG3_ADDR, 1); 
	
	//enable the clocks
	RCC_AHB1PeriphClockCmd(MEMSI_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//configure the GPIO pin
	GPIO_InitTypeDef   GPIO_init_s;
	GPIO_init_s.GPIO_Mode = GPIO_Mode_IN;
	GPIO_init_s.GPIO_PuPd = GPIO_PuPd_DOWN; //using pull-down is safer because we are using active high
	GPIO_init_s.GPIO_Pin = MEMSI_PIN;
	GPIO_init_s.GPIO_OType = GPIO_OType_PP;
	GPIO_init_s.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(MEMSI_GPIO_PORT, &GPIO_init_s);

	//connect EXTI to pin
	SYSCFG_EXTILineConfig(MEMSI_EXTI_PORT_SOURCE, MEMSI_EXTI_PIN_SOURCE);

	EXTI_InitTypeDef EXTI_init_s;
	EXTI_init_s.EXTI_Line = MEMSI_EXTI_LINE;
	EXTI_init_s.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_init_s.EXTI_Trigger = EXTI_Trigger_Rising; //triggers on rising edge
	EXTI_init_s.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_init_s);
	EXTI_ClearITPendingBit(MEMSI_EXTI_LINE);
	
	//NVIC is defined in misc.h and misc.c
	NVIC_InitTypeDef   NVIC_init_s;
	NVIC_init_s.NVIC_IRQChannel = MEMSI_NVIC_IRQ_CHANNEL;
	NVIC_init_s.NVIC_IRQChannelPreemptionPriority = MEMSI_PRIORITY;
	NVIC_init_s.NVIC_IRQChannelSubPriority = MEMSI_PRIORITY;
	NVIC_init_s.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_init_s);
	
}

