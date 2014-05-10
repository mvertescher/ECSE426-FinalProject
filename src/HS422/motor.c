#include "motor.h"

float pitch_angle;
float roll_angle;

/*
  MotorControl takes angles Roll and Pitch, both the desired angles and the measured angles.
	It uses these angles to caculate the angle given to the motors based on a simple proportioanl control.
	Saturation elements were also added increase the angles given exceed |+/-90|.
*/
void motorControl(float desired_pitch, float desired_roll, float meas_pitch, float meas_roll) {
  //uint32_t duty;

	/* FEEDBACK HERE */
	float pitch_dc;
	float roll_dc; 
	
	if (desired_pitch > 90 || desired_pitch < -90 || desired_roll > 90 || desired_roll < -90) {
		return;
	}
	
	uint32_t pitch_duty_cycle, roll_duty_cycle;
	
	float pitch_diff = desired_pitch - meas_pitch;
	float roll_diff = desired_roll - meas_roll;
	
	roll_angle += FEEDBACK_FACTOR * roll_diff;
	pitch_angle += FEEDBACK_FACTOR * pitch_diff;
	
	if (pitch_angle > 90)
		pitch_angle = 90;
	else if (pitch_angle < -90)
		pitch_angle = -90;
	
	pitch_duty_cycle = (uint32_t) (175*(pitch_angle+90)/9)+1200;
	roll_duty_cycle = (uint32_t) (190*(roll_angle+90)/9)+1200;
	

	
	TIM_SetCompare1(TIM8, roll_duty_cycle); // PC6 ROLL
	TIM_SetCompare3(TIM8, pitch_duty_cycle); // PC8 PITCH
	
	//printf("MOTOR: pitch angle: %f  roll angle: %f  pitch_diff: %f  roll_diff: %f \n",pitch_angle,roll_angle,pitch_diff,roll_diff);
	//printf("MOTOR: Pitch = %f (%f) Roll = %f (%f) | INPUT: Pitch = %i Roll = %i  | DIFF: Pitch %f Roll %f\n",desired_pitch,meas_pitch,desired_roll,meas_roll,pitch_duty_cycle, roll_duty_cycle,pitch_diff,roll_diff);
	//printf("MOTOR: Pitch = %f (%f) Roll = %f (%f) INPUT: Pitch = %u Roll = %u  DIFF: Pitch %f Roll %f\n",desired_pitch,meas_pitch,desired_roll,meas_roll,pitch_duty_cycle, roll_duty_cycle,pitch_diff,roll_diff);

	
}
/*
  Set up of the GPIO pins, Timers and PWM settings for the Motors
	Roll Motor is on GPIO PC6
	Pitch Motor is on GPIO PC8
*/
void motorEnable(){
  
		roll_angle = 0;
		pitch_angle = 0;
	
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
