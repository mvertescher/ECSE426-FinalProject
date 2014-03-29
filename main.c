#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include <stdio.h>
#include "ez430_rf2500t.h"
#include "smartrf_cc2500.h"

/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void thread(void const * argument);

//! Thread structure for above thread
osThreadDef(thread, osPriorityNormal, 1, 0);

/*!
 @brief Program entry point
 */
int main (void) {
	// ID for thread
	osThreadId tid_thread1;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	EZ430_RF2500T_InitTypeDef EZ430_RF2500T_InitStruct;
	EZ430_RF2500T_Init(&EZ430_RF2500T_InitStruct);
	
	printf("Start thread\n");
  
  //***************************************************************
  // Configure the CC2500, should be in cc2500.c
  /*uint8_t ctrl; 
	
  ctrl = SMARTRF_SETTING_FSCTRL1;
  EZ430_RF2500T_Write(&ctrl, CC2500_FSCTRL1, 1);
  
  ctrl = SMARTRF_SETTING_FSCTRL0;
  EZ430_RF2500T_Write(&ctrl, CC2500_FSCTRL0, 1);
  
  ctrl = SMARTRF_SETTING_FREQ2;
  EZ430_RF2500T_Write(&ctrl, CC2500_FREQ2, 1);
  
  ctrl = SMARTRF_SETTING_FREQ1;
  EZ430_RF2500T_Write(&ctrl, CC2500_FREQ1, 1);
  
  ctrl = SMARTRF_SETTING_FREQ0;
  EZ430_RF2500T_Write(&ctrl, CC2500_FREQ0, 1);
  
  ctrl = SMARTRF_SETTING_MDMCFG4;
  EZ430_RF2500T_Write(&ctrl, CC2500_MDMCFG4, 1);
  
  ctrl = SMARTRF_SETTING_MDMCFG3;
  EZ430_RF2500T_Write(&ctrl, CC2500_MDMCFG3, 1);
    
  ctrl = SMARTRF_SETTING_MDMCFG2;
  EZ430_RF2500T_Write(&ctrl, CC2500_MDMCFG2, 1);
  
  ctrl = SMARTRF_SETTING_MDMCFG1;
  EZ430_RF2500T_Write(&ctrl, CC2500_MDMCFG1, 1);
  
  ctrl = SMARTRF_SETTING_MDMCFG0;
  EZ430_RF2500T_Write(&ctrl, CC2500_MDMCFG0, 1);
  
  ctrl = SMARTRF_SETTING_CHANNR;
  EZ430_RF2500T_Write(&ctrl, CC2500_CHANNR, 1);
  
  ctrl = SMARTRF_SETTING_DEVIATN;
  EZ430_RF2500T_Write(&ctrl, CC2500_DEVIATN, 1);
  
  ctrl = SMARTRF_SETTING_FREND1;
  EZ430_RF2500T_Write(&ctrl, CC2500_FREND1, 1);
  
  ctrl = SMARTRF_SETTING_FREND0;
  EZ430_RF2500T_Write(&ctrl, CC2500_FREND0, 1);
   
  ctrl = SMARTRF_SETTING_MCSM0; 
  EZ430_RF2500T_Write(&ctrl, CC2500_MCSM0, 1);  
  
  ctrl = SMARTRF_SETTING_FOCCFG; 
  EZ430_RF2500T_Write(&ctrl, CC2500_FOCCFG, 1);
  
  ctrl = SMARTRF_SETTING_BSCFG;
  EZ430_RF2500T_Write(&ctrl, CC2500_BSCFG, 1);
  
  ctrl = SMARTRF_SETTING_AGCCTRL2;
  EZ430_RF2500T_Write(&ctrl, CC2500_AGCCTRL2, 1);
  
  ctrl = SMARTRF_SETTING_AGCCTRL1;
  EZ430_RF2500T_Write(&ctrl, CC2500_AGCCTRL1, 1);
  
  ctrl = SMARTRF_SETTING_AGCCTRL0;
  EZ430_RF2500T_Write(&ctrl, CC2500_AGCCTRL0, 1);
  
  ctrl = SMARTRF_SETTING_FSCAL3;
  EZ430_RF2500T_Write(&ctrl, CC2500_FSCAL3, 1);
  
  ctrl = SMARTRF_SETTING_FSCAL2;
  EZ430_RF2500T_Write(&ctrl, CC2500_FSCAL2, 1);
  
  ctrl = SMARTRF_SETTING_FSCAL1;
  EZ430_RF2500T_Write(&ctrl, CC2500_FSCAL1, 1);
  
  ctrl = SMARTRF_SETTING_FSCAL0;
  EZ430_RF2500T_Write(&ctrl, CC2500_FSCAL0, 1);
  
  ctrl = SMARTRF_SETTING_FSTEST;
  EZ430_RF2500T_Write(&ctrl, CC2500_FSTEST, 1);
  
  ctrl = SMARTRF_SETTING_TEST2;
  EZ430_RF2500T_Write(&ctrl, CC2500_TEST2, 1);
  
  ctrl = SMARTRF_SETTING_TEST1;
  EZ430_RF2500T_Write(&ctrl, CC2500_TEST1, 1);
  
  ctrl = SMARTRF_SETTING_TEST0;
  EZ430_RF2500T_Write(&ctrl, CC2500_TEST0, 1);
  
  ctrl = SMARTRF_SETTING_FIFOTHR;
  EZ430_RF2500T_Write(&ctrl, CC2500_FIFOTHR, 1);
  
  ctrl = SMARTRF_SETTING_IOCFG2;
  EZ430_RF2500T_Write(&ctrl, CC2500_IOCFG2, 1);
  
  ctrl = SMARTRF_SETTING_IOCFG0;
  EZ430_RF2500T_Write(&ctrl, CC2500_IOCFG0, 1);
  
  ctrl = SMARTRF_SETTING_PKTCTRL1;
  EZ430_RF2500T_Write(&ctrl, CC2500_PKTCTRL1, 1);
  
  ctrl = SMARTRF_SETTING_PKTCTRL0;
  EZ430_RF2500T_Write(&ctrl, CC2500_PKTCTRL0, 1);
  
  ctrl = SMARTRF_SETTING_ADDR;
  EZ430_RF2500T_Write(&ctrl, CC2500_ADDR, 1);
  
  ctrl = SMARTRF_SETTING_PKTLEN;
  EZ430_RF2500T_Write(&ctrl, CC2500_PKTLEN, 1);*/
  
  
  //***************************************************************

  
  
  osDelay(100);
  
  uint8_t bffr = 0x00;
  printf("BffrSTX before: %x \n",bffr);
  EZ430_RF2500T_Read(&bffr, CC2500_STX, 1);
  printf("BffrSTX after: %x %i \n\n",bffr,bffr);
  
	// Start thread
	tid_thread1 = osThreadCreate(osThread(thread), NULL);



	// The below doesn't really need to be in a loop
	while(1){
		osDelay(osWaitForever);
	}
}

void thread (void const *argument) {
	
  while(1){
		osDelay(5);
		
		uint8_t bffr = 0x00; 
		//uint8_t addr = CC2500_SNOP; // Read the part num reg 
	
    // addr = 0x30 should be 0x80, 128, but is 0xA0
    // addr = 0x31 should be 0x03, but is 0x9F
    
		//printf("Sending byte \n");
    
    printf("Bffr before: %x \n",bffr);
		EZ430_RF2500T_Read(&bffr, CC2500_SNOP, 1);
    printf("Bffr after: %x %i \n\n",bffr,bffr);
    
    
		//GPIOD->BSRRL = GPIO_Pin_12;
		osDelay(500);
		//GPIOD->BSRRH = GPIO_Pin_12;
	}
}
