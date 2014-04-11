#include "motor.h"
/*
  MotorControl takes angles Roll and Pitch, but since we have only one motor, Roll is the 
  only variable that updates the duty Cycle of our PWM signal
*/
void motorControl(float pitch, float roll){
  uint32_t duty;
  roll = (190*(roll+90)/9)+1200;
	pitch = (175*(pitch+90)/9)+1200;
  TIM_SetCompare1(TIM8, roll); // PC6
	TIM_SetCompare3(TIM8, pitch); // PC8
}
/*
  Set up of the GPIO pin, Timer and PWM settings for the Motor
*/
void motorEnable(){
  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    //Setting PinC6 to Alternate Function of TIM8
    GPIO_InitTypeDef GPIO_InitStructure;			
    GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
	
	//Setting PinC8 to Alternate Function of TIM8		
	GPIO_StructInit (&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
  
    //Enabling TIM8 Clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    //Set Prescaler to 84
    uint32_t prescaler = (uint16_t)((SystemCoreClock/2)/1000000)-1; //Clock Counts at 1Mhz
  
    //TIMER SETUP
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 20000-1; //Pulse given at every 20ms or 50hz
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //Do not Divide Clock
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
  
  //PWM SETUP 
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Set Timer to PWM
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Enable Output
  TIM_OCInitStructure.TIM_Pulse = 0; //Initial CCR Reg is 0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Active High PWM Signal
  
  /* PWM1 Mode configuration: Channel1 (GPIOC Pin 6/8)*/
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);	//Enable Preload of CCR Reg
	TIM_ARRPreloadConfig(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE); //Enable PWM Output to Motor
  TIM_Cmd(TIM8, ENABLE); //Enable/Start Timer
   
}