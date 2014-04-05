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

//! Thread structure for above thread
osThreadDef(transmit_thread, osPriorityNormal, 1, 0);
osThreadDef(receive_thread, osPriorityNormal, 1, 0);
osThreadDef(accelerometer_thread, osPriorityNormal, 1, 0);


void transmit_loop(void); 
void receive_loop(void);

float pitch = .5f;
float roll= .5f;

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
  
  
  if (IS_TRANSMITTER) {    
    init_accelerometer();
    printf("Transmitter starting threads \n");
    tid_transmit = osThreadCreate(osThread(transmit_thread), NULL);
    tid_accelerometer = osThreadCreate(osThread(accelerometer_thread), NULL);
  }
  
  else {
    printf("Receiver starting threads \n");
    tid_receive = osThreadCreate(osThread(receive_thread), NULL);
  }
  
	// The main thread does nothing
	while(1){
		osDelay(osWaitForever);
	}
}

void transmit_loop(void) {
  float pitch = 1.1f;
  float roll = 2.5f;
  while (1) {
    transmit_pitchroll(pitch, roll);
    osDelay(1000);
  }
}

void receive_loop(void) {
  uint8_t status = CC2500_CommandProbe(CC2500_READBIT, CC2500_SRX);
  printf("Moved to RX (%x) \n",status);
  osDelay(500);
  
  float pitch, roll; 
  uint16_t control;
  while (1) { 
    control = receive_pitchroll(&pitch, &roll);
    printf("READ: Pitch = %f  Roll = %f  Control = %x \n",pitch,roll,control);
  }
}


void transmit_thread(void const *argument) {
  while (1) {
    transmit_pitchroll(pitch, roll);
    osDelay(50);
  }
}


void receive_thread(void const *argument) {
    
 
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
      //LIS3DSH_Read(&buffer[0], LIS3DSH_OUT_X_L, 6);
      
      LIS3DSH_Read(&buffer[0], LIS3DSH_OUT_X_L, 1);
			LIS3DSH_Read(&buffer[1], LIS3DSH_OUT_X_H, 1);
			LIS3DSH_Read(&buffer[2], LIS3DSH_OUT_Y_L, 1);
			LIS3DSH_Read(&buffer[3], LIS3DSH_OUT_Y_H, 1);
			LIS3DSH_Read(&buffer[4], LIS3DSH_OUT_Z_L, 1);
			LIS3DSH_Read(&buffer[5], LIS3DSH_OUT_Z_H, 1);

      aggregateResult = (int32_t)(buffer[0] | buffer[1] << 8);
			a_x =(float)(LIS3DSH_SENSITIVITY_2G * (float)aggregateResult);
		
			aggregateResult = (int32_t)(buffer[2] | buffer[3] << 8);
			a_y =(float)(LIS3DSH_SENSITIVITY_2G * (float)aggregateResult);
	
			aggregateResult = (int32_t)(buffer[4] | buffer[5] << 8);
			a_z =(float)(LIS3DSH_SENSITIVITY_2G * (float)aggregateResult);
		
			x_flag = 0;
			y_flag = 0;
			
			if (a_x > 2000) {
				a_x = 4000 - a_x;
				x_flag = 1;
			}
			if (a_y > 2000) {
				a_y = 4000 - a_y;
				y_flag = 1;
			}
			
			if (x_flag)
				raw_pitch = (float) -atan_table(a_x / sqrt(a_y * a_y + a_z * a_z));
			else 
				raw_pitch = (float) atan_table(a_x / sqrt(a_y * a_y + a_z * a_z));
			
			if (y_flag)
				raw_roll = (float) -atan_table(a_y / sqrt(a_x * a_x + a_z * a_z));
			else
				raw_roll = (float) atan_table(a_y / sqrt(a_x * a_x + a_z * a_z));
						
			pitch = filter_point((int) raw_pitch, &pitch_filter);
			roll = filter_point((int) raw_roll, &roll_filter);
			
      //printf("ACCR:  pitch = %f  roll = %f \n",pitch,roll);
      
      osDelay(50);
	}
}
