#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "cmsis_os.h"


/*
Pin Assignment for Keypad
KEYPAD PIN = BOARD PIN
pin 1 = PD1
pin 2 = PD2
pin 3 = PD3
pin 4 = PD6
pin 5 = PD7
pin 6 = PD8
pin 7 = PD9
pin 8 = PD10
*/

/* The hex values of the Keypad keys

	 1 = 0xee, 2 = 0xed, 3 = 0xeb, A = 0xe7
	 4 = 0xde, 5 = 0xdd, 6 = 0xdb, B = 0xd7
	 7 = 0xbe, 8 = 0xbd, 9 = 0xbb, C = 0xb7
	 * = 0x7e, 0 = 0x7d, # = 0x7b, D = 0x77
	 
*/

 #define ONE  0xEE
 #define TWO  0xED
 #define THREE  0xEB
 #define FOUR  0xDE
 #define FIVE  0xDD
 #define SIX  0xDB
 #define SEVEN  0xBE
 #define EIGHT  0xBD
 #define NINE  0xBB
 #define ZERO  0x7D
 #define A  0xE7
 #define B  0xD7
 #define C  0xB7
 #define D  0x77
 #define STAR  0x7E
 #define POUND  0x7B
 
 #define KEYBOARD_DELAY 200
 #define KEYBOARD_STATE_IDLE 0x00
 #define KEYBOARD_STATE_PRT 0x01
 
 

void readValue(float* value, osThreadId* tid_keyboard);
void returnValue(uint8_t keyCurrent, float* value, uint8_t* flag, int8_t* sign);
void enableKeyboard(void);
uint8_t readKeyboard(void);
void enableColumnInput(void);
void enableColumnOutput(void);
void enableRowInput(void);
void enableRowOutput(void);
void setColumn(uint8_t value);
void setRow(uint8_t value);
uint8_t readRow(void);
uint8_t readColumn(void);
void enableKeyboardINT(void);
uint8_t keyToNumber(uint8_t key);
void disableKeyboardINT(void);