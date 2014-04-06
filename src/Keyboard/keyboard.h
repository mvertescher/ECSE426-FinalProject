#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


/*
pin 1 = PD1
pin 2 = PD2
pin 3 = PD3
pin 4 = PD6
pin 5 = PD7
pin 6 = PD8
pin 7 = PD9
pin 8 = PD10
*/



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