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



