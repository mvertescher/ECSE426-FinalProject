#include "led.h"

void enableBlueLED(void){
  //Setting Up RED LED for PWM
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//Setting PinD15 to Alternate Function of TIM4
  GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
  
  //Enabling TIM4 Clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  //Set Prescaler to 84
  uint32_t prescaler = 840-1; //100khz
  
  //TIMER SETUP
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = 1000-1; 
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler; 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //Do not Divide Clock
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  //PWM SETUP 
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Set Timer to PWM
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Enable Output
  TIM_OCInitStructure.TIM_Pulse = 0; //Initial CCR Reg is 0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Active High PWM Signal
  
  /* PWM1 Mode configuration: Channel3 (GPIOD Pin 14)*/
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //Enable Preload of CCR Reg
  TIM_Cmd(TIM4, ENABLE); //Enable/Start Timer
}

void enableRedLED(void){
  //Setting Up RED LED for PWM
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//Setting PinD14 to Alternate Function of TIM4
  GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
  
  //Enabling TIM4 Clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  //Set Prescaler to 84
  uint32_t prescaler = 840-1; //100khz
  
  //TIMER SETUP
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = 1000-1; 
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler; 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //Do not Divide Clock
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  //PWM SETUP 
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Set Timer to PWM
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Enable Output
  TIM_OCInitStructure.TIM_Pulse = 0; //Initial CCR Reg is 0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Active High PWM Signal
  
  /* PWM1 Mode configuration: Channel3 (GPIOD Pin 14)*/
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //Enable Preload of CCR Reg
  TIM_Cmd(TIM4, ENABLE); //Enable/Start Timer
}
void enableOrangeLED(void){
  //Setting Up RED LED for PWM
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//Setting PinD14 to Alternate Function of TIM4
  GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
  
  //Enabling TIM4 Clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  //Set Prescaler to 84
  uint32_t prescaler = 840-1; //100khz
  
  //TIMER SETUP
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = 1000-1; 
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler; 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //Do not Divide Clock
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  //PWM SETUP 
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Set Timer to PWM
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Enable Output
  TIM_OCInitStructure.TIM_Pulse = 0; //Initial CCR Reg is 0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Active High PWM Signal
  
  /* PWM1 Mode configuration: Channel3 (GPIOD Pin 14)*/
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //Enable Preload of CCR Reg
  TIM_Cmd(TIM4, ENABLE); //Enable/Start Timer
}
void enableGreenLED(void){
  //Setting Up RED LED for PWM
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//Setting PinD14 to Alternate Function of TIM4
  GPIO_InitTypeDef GPIO_InitStructure;			
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
  
  //Enabling TIM4 Clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  //Set Prescaler to 84
  uint32_t prescaler = 840-1; //100khz
  
  //TIMER SETUP
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = 1000-1; 
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler; 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //Do not Divide Clock
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  //PWM SETUP 
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Set Timer to PWM
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Enable Output
  TIM_OCInitStructure.TIM_Pulse = 0; //Initial CCR Reg is 0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Active High PWM Signal
  
  /* PWM1 Mode configuration: Channel3 (GPIOD Pin 14)*/
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //Enable Preload of CCR Reg
  TIM_Cmd(TIM4, ENABLE); //Enable/Start Timer
}
void set_Blue_LED_Intensity(uint8_t value){
    if(value<=100){
        TIM_SetCompare4(TIM4, 10*value);
    }
    else{
        TIM_SetCompare4(TIM4, 999);
    }
}
void set_Red_LED_Intensity(uint8_t value){
    if(value<=100){
        TIM_SetCompare3(TIM4, 10*value);
    }
    else{
        TIM_SetCompare3(TIM4, 999);
    }
}
void set_Orange_LED_Intensity(uint8_t value){
    if(value<=100){
        TIM_SetCompare2(TIM4, 10*value);
    }
    else{
        TIM_SetCompare2(TIM4, 999);
    }
}
void set_Green_LED_Intensity(uint8_t value){
    if(value<=100){
        TIM_SetCompare1(TIM4, 10*value);
    }
    else{
        TIM_SetCompare1(TIM4, 999);
    }
}
void enableLEDS(void){
  enableBlueLED();
  enableRedLED();
  enableOrangeLED();
  enableGreenLED();
}
void showSignalStrength(float signal){
  set_Blue_LED_Intensity((uint8_t)(LED_GRADIENT*signal+LED_OFFSET));
  set_Red_LED_Intensity((uint8_t)(LED_GRADIENT*signal+LED_OFFSET));
  set_Green_LED_Intensity((uint8_t)(LED_GRADIENT*signal+LED_OFFSET));
  set_Orange_LED_Intensity((uint8_t)(LED_GRADIENT*signal+LED_OFFSET));
}