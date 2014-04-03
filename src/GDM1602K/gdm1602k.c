#include "gdm1602k.h"

void gdm1602k_init(void) 
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(GDM1602K_GPIO_CLK1, ENABLE);
	RCC_AHB1PeriphClockCmd(GDM1602K_GPIO_CLK2, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_Rs_PIN; // LED pin 4 -- Rs
	GPIO_Init(GDM1602K_Rs_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_E_PIN;  // LED pin 5 -- R / W
	GPIO_Init(GDM1602K_E_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_RW_PIN; // LED pin 6 -- E
	GPIO_Init(GDM1602K_RW_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_DB0_PIN; // LED pin 7 -- DB0
	GPIO_Init(GDM1602K_DB0_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_DB1_PIN; // LED pin 8 -- DB1
	GPIO_Init(GDM1602K_DB1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_DB2_PIN; // LED pin 9 -- DB2
	GPIO_Init(GDM1602K_DB2_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_DB3_PIN; // LED pin 10 -- DB3
	GPIO_Init(GDM1602K_DB3_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_DB4_PIN; // LED pin 11 -- DB4
	GPIO_Init(GDM1602K_DB4_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_DB5_PIN; // LED pin 12 -- DB5
	GPIO_Init(GDM1602K_DB5_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_DB6_PIN; // LED pin 13 -- DB6
	GPIO_Init(GDM1602K_DB6_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GDM1602K_DB7_PIN; // LED pin 14 -- DB7
	GPIO_Init(GDM1602K_DB7_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_RW_GPIO_PORT, GDM1602K_RW_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_DB0_GPIO_PORT, GDM1602K_DB0_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_DB1_GPIO_PORT, GDM1602K_DB1_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, Bit_RESET);
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_RESET);
}

// D : Display ON/OFF control bit
// C : Cursor ON/OFF control bit
// B : Cursor Blink ON/OFF control bit
void gdm1602k_display_on(uint8_t D, uint8_t C, uint8_t B)
{
	GPIO_WriteBit(GDM1602K_DB0_GPIO_PORT, GDM1602K_DB0_PIN, (BitAction) B);     
	GPIO_WriteBit(GDM1602K_DB1_GPIO_PORT, GDM1602K_DB1_PIN, (BitAction) C);	   
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, (BitAction) D);   	
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, Bit_SET);   	
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, Bit_RESET);   
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, Bit_RESET);   
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, Bit_RESET); 	
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_RESET);   
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET); // Select command
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); 		// Enable high
	osDelay(1);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); 	// Enable low
	osDelay(1); // delay
}

void gdm1602k_clear(void)
{
	GPIO_WriteBit(GDM1602K_DB0_GPIO_PORT, GDM1602K_DB0_PIN, Bit_SET);     // LED pin 7 -- DB0
	GPIO_WriteBit(GDM1602K_DB1_GPIO_PORT, GDM1602K_DB1_PIN, Bit_RESET);   // LED pin 8 -- DB1
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, Bit_RESET);   // LED pin 9 -- DB2
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, Bit_RESET);   // LED pin 10 -- DB3
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, Bit_RESET);   // LED pin 11 -- DB4
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, Bit_RESET);   // LED pin 12 -- DB5
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, Bit_RESET); 		 // LED pin 13 -- DB6
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_RESET);  // LED pin 14 -- DB7
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET); // Select reg 1, command
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); // Enable high
	osDelay(1);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); // Enable low
	osDelay(2); // need a 1.53ms delay for the display to clear 
}

void gdm1602k_return_home(void)
{
	GPIO_WriteBit(GDM1602K_DB1_GPIO_PORT, GDM1602K_DB1_PIN, Bit_SET);	   // LED pin 8 -- DB1
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, Bit_RESET);   // LED pin 9 -- DB2
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, Bit_RESET);   // LED pin 10 -- DB3
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, Bit_RESET);   // LED pin 11 -- DB4
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, Bit_RESET);   // LED pin 12 -- DB5
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, Bit_RESET); 	 // LED pin 13 -- DB6
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_RESET);   // LED pin 14 -- DB7
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET); // Select reg 1, command
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); // Enable high
	osDelay(1);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); // Enable low
	osDelay(2); // need a 1.53ms delay for the display to return home
}

// DL: Interface data length control bit 
//	DL = 1, 8-bit bus mode, 
//	DL = 0, 4-bit bus mode,
// N: Display line number control bit
//	N = 0, 1-line display mode
// 	N = 1, 2-line display mode
// F: Display line number control bit
//	F = 0, 5x8 dots format display mode is set
//	F = 1, 5x11 dots format display mode
void gdm1602k_function_set(uint8_t DL,uint8_t N,uint8_t F)
{
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, (BitAction) F);   	
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, (BitAction) N);   	
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, (BitAction) DL);   
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, Bit_SET);   				
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, Bit_RESET); 		 		
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_RESET);  			
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET); // Select reg 1, command
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); // Enable high
	osDelay(1);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); // Enable low
	osDelay(1); // need a 39us delay for the display to clear 
}

void gdm1602k_set_cgram_addr(uint8_t b)
{
	GPIO_WriteBit(GDM1602K_DB0_GPIO_PORT, GDM1602K_DB0_PIN, (BitAction) (b&0x01));   
	GPIO_WriteBit(GDM1602K_DB1_GPIO_PORT, GDM1602K_DB1_PIN, (BitAction) (b&0x02));   
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, (BitAction) (b&0x04));   
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, (BitAction) (b&0x08));   
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, (BitAction) (b&0x10));   
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, (BitAction) (b&0x20));   
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, Bit_SET); 		 						
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_RESET);  						
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET); 							// Select reg 1, command
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); 									// Enable high
	osDelay(1);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); 								// Enable low
	osDelay(1); // need a 39us delay for the display to clear 
}

// Will change the position of the cursor
void gdm1602k_set_ddram_addr(uint8_t b)
{
	GPIO_WriteBit(GDM1602K_DB0_GPIO_PORT, GDM1602K_DB0_PIN, (BitAction) (b&0x01));   
	GPIO_WriteBit(GDM1602K_DB1_GPIO_PORT, GDM1602K_DB1_PIN, (BitAction) (b&0x02));   
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, (BitAction) (b&0x04));   
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, (BitAction) (b&0x08));   
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, (BitAction) (b&0x10));   
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, (BitAction) (b&0x20));   
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, (BitAction) (b&0x40)); 	 
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_SET);  							 
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET); 							// Select reg 1, command
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); 								// Enable high
	osDelay(1);																									// Delay
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); 							// Enable low
	osDelay(1); // need a 39us delay for the display to clear 
}


void gdm1602k_write(uint8_t b)
{
	GPIO_WriteBit(GDM1602K_DB0_GPIO_PORT, GDM1602K_DB0_PIN, (BitAction) (b&0x01));   
	GPIO_WriteBit(GDM1602K_DB1_GPIO_PORT, GDM1602K_DB1_PIN, (BitAction) (b&0x02)); 
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, (BitAction) (b&0x04));   
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, (BitAction) (b&0x08));   
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, (BitAction) (b&0x10));   
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, (BitAction) (b&0x20));   
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, (BitAction) (b&0x40));   
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, (BitAction) (b&0x80));   
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_SET); // Select reg 1, data
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); // Enable high
	int i; for(i = 0; i < 0x8FFF; i++); //osDelay(1);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); // Enable low
	osDelay(1); // need a 43us delay to write to reg
}
/*
uint8_t GDM1602K_IsBusy(uint8_t b)
{ 
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_RESET); // BF flag
	
	GPIO_WriteBit(GDM1602K_RW_GPIO_PORT, GDM1602K_RW_PIN, Bit_SET); // for read
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET); // Select reg 1, instr
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); // Enable high
	osDelay(1);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); // Enable low
	//osDelay(1); // need a 0us delay to write to reg
	GPIO_WriteBit(GDM1602K_RW_GPIO_PORT, GDM1602K_RW_PIN, Bit_RESET); // for write 
	return 1;
}*/


void gdm1602k_display_uint8(uint8_t b)
{
	uint8_t hund_place = b / 100;
	b = b - (hund_place * 100);
	uint8_t tens_place = b / 10;
	b = b - (tens_place * 10);
	uint8_t ones_place = b;
	
	gdm1602k_write(0x30|hund_place); // 0x30 for digit
	gdm1602k_write(0x30|tens_place);
	gdm1602k_write(0x30|ones_place);
}

void gdm1602k_space(void)
{
	//gdm1602k_write(0xA0);
	gdm1602k_shift(1); 
}


// shift display or cursor: 
// S/C | R/L | Meaning 
//	0	 |	0  | Shift cursor to left
//	0	 |	1  | Shift cursor to right
//	1	 |	0  | Shift all the display to the left
//	1	 |	1  | Shift all the display to the right
void gdm1602k_shift(uint8_t b) 
{
	GPIO_WriteBit(GDM1602K_DB2_GPIO_PORT, GDM1602K_DB2_PIN, (BitAction) (b&0x01));   
	GPIO_WriteBit(GDM1602K_DB3_GPIO_PORT, GDM1602K_DB3_PIN, (BitAction) (b&0x02));   
	GPIO_WriteBit(GDM1602K_DB4_GPIO_PORT, GDM1602K_DB4_PIN, Bit_SET);   
	GPIO_WriteBit(GDM1602K_DB5_GPIO_PORT, GDM1602K_DB5_PIN, Bit_RESET);   
	GPIO_WriteBit(GDM1602K_DB6_GPIO_PORT, GDM1602K_DB6_PIN, Bit_RESET);   
	GPIO_WriteBit(GDM1602K_DB7_GPIO_PORT, GDM1602K_DB7_PIN, Bit_RESET);   
	
	GPIO_WriteBit(GDM1602K_Rs_GPIO_PORT, GDM1602K_Rs_PIN, Bit_RESET); // Select reg 1, data
	
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_SET); // Enable high
	osDelay(1);
	GPIO_WriteBit(GDM1602K_E_GPIO_PORT, GDM1602K_E_PIN, Bit_RESET); // Enable low
	osDelay(1); // 39us delay to shift command
}

void gdm1602k_display(char *str)
{
	int i = 0;
	char c;
	while (1) {
		c = str[i];
		if (c == '\0')
			break;
		gdm1602k_write((uint8_t) c);
		i += 1;
	}	
}

void gdm1602k_print_str(char *str, int length)
{
	int i;
	for (i = 0; i < length; i++) 
		gdm1602k_write((uint8_t) str[i]);
}


void gdm1602k_display_float(float f)
{
	char str[10];
	sprintf(str, "%f", f);
	gdm1602k_print_str(str, 5);
}	

void gdm1602k_display_int(int i)
{
	char str[10];
	sprintf(str, "%i", i);
	gdm1602k_print_str(str, 4);
}

