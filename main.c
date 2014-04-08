#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include <stdio.h>
#include "smartrf_cc2500.h"
#include "cc2500.h"
#include "wireless.h"

#include "filter.h"
#include "lis3dsh.h"
#include "atan_LUT.h"
#include "accelerometer.h"
#include "keyboard.h"
#include "motor.h"

#define PITCH_FILTER_SIZE   16
#define ROLL_FILTER_SIZE    16

#define IS_TRANSMITTER      1

/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void transmit_thread(void const * argument);
void receive_thread(void const *argument);
void accelerometer_thread(void const *argument);
void keyboard_thread(void const *argument);
void motor_thread(void const *argument);

//! Thread structure for above thread
osThreadDef(transmit_thread, osPriorityNormal, 1, 0);
osThreadDef(receive_thread, osPriorityNormal, 1, 0);
osThreadDef(accelerometer_thread, osPriorityNormal, 1, 0);
osThreadDef(keyboard_thread, osPriorityNormal, 1, 0);
osThreadDef(motor_thread, osPriorityNormal, 1, 0);

float pitch = .5f;
float roll= .5f;

int transmit_locked; 

//Global Struct Containing Display Values
struct Values{
    float pitch;
    float roll;
    uint8_t follow;
    float rollIncrement;
    float pitchIncrement;
    float time;
    char line1[16];
    char line2[16];
};

struct Values measurements = {0};

//Mutex to Protect Display Values
osMutexId measureUpdate;
osMutexDef(measure_update);
//OS Timer
void Motor_Timer_Callback(void const *arg);
osTimerDef(MotorTimer, Motor_Timer_Callback);
uint32_t exec;
osTimerId timerid_motor;

osThreadId tid_keyboard;  
osThreadId tid_motor;

/*!
 @brief Program entry point
 */
int main (void) {
  // ID for thread
  osThreadId tid_transmit;
  osThreadId tid_receive;
  osThreadId tid_accelerometer;    
  
  
  // Start wireless 
  uint8_t status;
  status = init_wireless();
  printf("CC2500 Ready (%x) \n",status);
  osDelay(1000);
  
  measureUpdate = osMutexCreate(osMutex(measure_update));
  
  if (IS_TRANSMITTER) {    
    init_accelerometer();
    transmit_locked = 0;
    enableKeyboard();
    enableKeyboardINT();
    
    printf("Transmitter starting threads \n");
    tid_transmit = osThreadCreate(osThread(transmit_thread), NULL);
    tid_accelerometer = osThreadCreate(osThread(accelerometer_thread), NULL);
    tid_keyboard = osThreadCreate(osThread(keyboard_thread), &measurements);
  }
  
  else {
    motorEnable();
    measurements.follow = 1;
    printf("Receiver starting threads \n");
    
    timerid_motor = osTimerCreate(osTimer(MotorTimer),osTimerPeriodic,&exec);
    tid_motor = osThreadCreate(osThread(motor_thread), &measurements);
    tid_receive = osThreadCreate(osThread(receive_thread), NULL);
  }
  
	// The main thread does nothing
	while(1){
		osDelay(osWaitForever);
	}
}


void transmit_thread(void const *argument) {
  while (1) {
    if (!transmit_locked)
      transmit_pitchroll(pitch, roll);
    osDelay(20);
  }
}


void receive_thread(void const *argument) {
  uint8_t status = CC2500_CommandProbe(CC2500_READBIT, CC2500_SRX);
  printf("Moved to RX (%x) \n",status);
  osDelay(500);
  
  float pitch, roll, value; 
  uint16_t control;
  uint8_t ctrl = 0;
  while (1) { 
    control = receive_pitchroll(&pitch, &roll);
    if (control == PACKET_CTRL1_BEGIN) {
      while (ctrl != PACKET_CTRL1_END) {
        receive_keypad(&ctrl, &value);
        switch(ctrl) {
            case PACKET_CTRL1_PITCH: 
              measurements.pitchIncrement = value; 
              break;
            case PACKET_CTRL1_ROLL: 
              measurements.rollIncrement = value; 
              break;
            case PACKET_CTRL1_TIME: 
              measurements.time = value; 
              break;
        } 
      } 
      printf("EX MOVE pitchIncrement = %f, rollIncrement = %f, time = %f \n",measurements.pitchIncrement,measurements.rollIncrement,measurements.time);
      measurements.follow = 0;
      osTimerStart(timerid_motor,100);
      while (!measurements.follow) {
        osDelay(500);
      }  
      ctrl = 0; // reset control
    }
    else if (control == PACKET_CTRL1_PR) {
      measurements.pitch = pitch;
      measurements.roll = roll;
    }
		
		else if (control == PACKET_CTRL1_RECORD_BEGIN) {
			float pitchBuffer[256];
			float rollBuffer[256];
			receive_record_sequence(pitchBuffer,rollBuffer);
			
		}	
			
    if (control == 1)
      printf("READ: Pitch = %f  Roll = %f  Control = %x \n",pitch,roll,control);
  }
}

void accelerometer_thread(void const *argument) {
	float raw_pitch, raw_roll;
	float a_x, a_y, a_z;
	int32_t aggregateResult;
	uint8_t buffer[6];
	uint8_t x_flag, y_flag; 
	
	ring_buffer_t pitch_filter; 
	int size = PITCH_FILTER_SIZE; 
	int pitch_buffer[size];
	init_buffer(&pitch_filter,pitch_buffer,size);
	
	ring_buffer_t roll_filter; 
	size = ROLL_FILTER_SIZE; 
	int roll_buffer[size];
	init_buffer(&roll_filter,roll_buffer,size);
	
	while(1) {		
			get_pitch_roll(&raw_pitch, &raw_roll);
			pitch = -filter_point((int) raw_pitch, &pitch_filter); //FIX
			roll= filter_point((int) raw_roll, &roll_filter);
      measurements.pitch = pitch;
      measurements.roll = roll;
      
      osDelay(20);
	}
}

void motor_thread(void const *argument) {
  struct Values *values = (struct Values*)argument;
  char string[5]= {0};
  float roll, pitch;
  uint32_t mscount=0;

	while(1){
		if(values->follow == 1){
			
      osMutexWait(measureUpdate, osWaitForever);
      
			motorControl(values->pitch, values->roll);
			
      /*strcpy(values->line1, "");
      floatToString(values->pitch, string);
      strcat(values->line1,"Pitch is ");
      strcat(values->line1,string);
      strcat(values->line1,"  ");
      
      strcpy(values->line2, "");
      floatToString(values->roll, string);
      strcat(values->line2,"Roll is  ");
      strcat(values->line2,string);
      strcat(values->line2,"  ");
        */    
      osMutexRelease(measureUpdate);
      
		}
		if(values->follow==0){
      osSignalWait(0x02, osWaitForever);
      osMutexWait(measureUpdate, osWaitForever);
      roll = roll+values->rollIncrement;
      pitch = pitch+values->pitchIncrement;
      motorControl(roll,pitch);
      ++mscount;
      // Done seq
      if (mscount >= values->time) {
        osTimerStop(timerid_motor);
        mscount=0;
        // Delay 
        osDelay(1000);
        // Move back to follow
        measurements.follow = 1;
      }
      osMutexRelease(measureUpdate);
		}
		osDelay(10);
	}
}

void keyboard_thread(void const *argument) {
  /*
	 1 = 0xee, 2 = 0xed, 3 = 0xeb, A = 0xe7
	 4 = 0xde, 5 = 0xdd, 6 = 0xdb, B = 0xd7
	 7 = 0xbe, 8 = 0xbd, 9 = 0xbb, C = 0xb7
	 * = 0x7e, 0 = 0x7d, # = 0x7b, D = 0x77
	*/
	struct Values *values = (struct Values*)argument;
	
  //Flag Variables
	uint8_t roll = 0;
	uint8_t pitch = 0;
	uint8_t time=0;
	int8_t sign=0;
	uint8_t keyCount=0;
  
  float currentRoll=0;
  float currentPitch=0;
  float previousRoll=0;
  float previousPitch=0;
  uint32_t mstime=0;
  
	uint8_t keyCurrent = 0xFF;
	
	while(1){
      
      // Wait for keypad INT
      osSignalWait(0x08, osWaitForever);
			
			// Read the current key 
      keyCurrent = readKeyboard(); 
      
      transmit_locked = 1; // Lock transmitter
   
			
      printf("Keyboard Value 0x%x\n", keyCurrent);
			if(keyCurrent==0xE7){
        osMutexWait(measureUpdate, osWaitForever);
				values->follow=0;
        osMutexRelease(measureUpdate);
				currentRoll=0;
        previousRoll= values->roll;
        previousPitch = values->pitch;
				roll=1;
        
        int i; 
        for (i = 0; i < 1; i++)
          transmit_keypad_begin();
        
			}
			if(roll==1){
              osMutexWait(measureUpdate, osWaitForever);
              strcpy(values->line1, " Please Enter a ");
              strcpy(values->line2, "   Roll Angle   ");
              osMutexRelease(measureUpdate);
					if(keyCurrent==0x7E){
						sign=1;
					}
					if(keyCurrent==0x7B){
						sign=-1;
					}
					if((sign!=0)&(keyToNumber(keyCurrent)<10)){
                        currentRoll =currentRoll*10+keyToNumber(keyCurrent);
						printf("Roll %f\n", currentRoll);
						++keyCount;
					}
					if(keyCurrent == 0x77){
						keyCount=0;
                        currentRoll = currentRoll*sign;
                        sign=0;
						roll=0;
                        printf("Angle %f\n", currentRoll);
					}
			}
			if(keyCurrent==0xd7){
				osMutexWait(measureUpdate, osWaitForever);
				values->follow=0;
                osMutexRelease(measureUpdate);
				currentPitch=0;
				pitch=1;
			}
			if(pitch==1){
          osMutexWait(measureUpdate, osWaitForever);
          strcpy(values->line1, " Please Enter a ");
          strcpy(values->line2, "  Pitch Angle   ");
          osMutexRelease(measureUpdate);
					if(keyCurrent==0x7E){
						sign=1;
					}
					if(keyCurrent==0x7B){
						sign=-1;
					}
					if((sign!=0)&(keyToNumber(keyCurrent)<10)){
            currentPitch = currentPitch*10+keyToNumber(keyCurrent);
						printf("Pitch %f\n", currentPitch);
						++keyCount;
					}
					if(keyCurrent == 0x77){
						currentPitch = currentPitch*sign;
                        printf("Angle %f\n", currentPitch);
						keyCount=0;
						sign=0;
						pitch=0;
					}
			}
			if(keyCurrent==0xB7){
				values->follow=0;
				values->time=0;
				time=1;
			}
			if(time==1){
              osMutexWait(measureUpdate, osWaitForever);
              strcpy(values->line1, " Please Enter a ");
              strcpy(values->line2, "    Time in s   ");
              osMutexRelease(measureUpdate);
					if(keyToNumber(keyCurrent)<10){
						mstime=mstime*10+keyToNumber(keyCurrent);
						printf("Time %ums\n", mstime);
						++keyCount;
					}
					if(keyCurrent == 0x77){
						keyCount=0;
						time=0;
            osMutexWait(measureUpdate, osWaitForever);
            values->time= (float)(mstime/100);
            values->rollIncrement = (currentRoll-previousRoll)/(mstime/100);
            values->pitchIncrement = (currentPitch-previousPitch)/(mstime/100);
            printf("RollInc %f PitchInc %f\n", values->rollIncrement, values->pitchIncrement);
            
            // Transmit 
            printf("SENDING pitchIncrement = %f \n",values->pitchIncrement);
            transmit_keypad_pitch(values->pitchIncrement);
            osDelay(100);
            printf("SENDING rollIncrement = %f \n",values->rollIncrement);
            transmit_keypad_roll(values->rollIncrement);
            osDelay(100);
            printf("SENDING time = %f \n",values->time);
            transmit_keypad_time(values->time);
            osDelay(100);
            printf("SENDING END \n");
            transmit_keypad_end();
            osDelay(100);
            osMutexRelease(measureUpdate);
            //osTimerStart(timerid_motor,100);
            printf("Time %f\n", values->time);           
            
            // Start transmitting again after delay
            osDelay(6000);
            transmit_locked = 0; // unlock transmit
            mstime = 0; // reset mstime
					}
    }
		osDelay(200);
    osSignalClear(tid_keyboard, 0x08);
	}
  
}

//KeyBoard handlers
void EXTI1_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
    EXTI_ClearITPendingBit(EXTI_Line1); 
    osSignalSet(tid_keyboard, 0x08);
  }
}
void EXTI2_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
    EXTI_ClearITPendingBit(EXTI_Line2);     
    osSignalSet(tid_keyboard, 0x08);
  }
}
void EXTI3_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
    EXTI_ClearITPendingBit(EXTI_Line3);     
		osSignalSet(tid_keyboard, 0x08);    
    }
}
void EXTI9_5_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
    EXTI_ClearITPendingBit(EXTI_Line6);     
		osSignalSet(tid_keyboard, 0x08);     
    }
}
void Motor_Timer_Callback(void const *arg){
  osSignalSet(tid_motor,0x02);
}

