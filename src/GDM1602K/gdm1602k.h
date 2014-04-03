
#ifndef __gdm1602k
#define __gdm1602k
#endif

#include <stdio.h>
#include "stm32f4xx.h"
#include "cmsis_os.h" // for now, should be some other delay system

#define GDM1602K_GPIO_CLK1         		RCC_AHB1Periph_GPIOA
#define GDM1602K_GPIO_CLK2         		RCC_AHB1Periph_GPIOB

#define GDM1602K_Rs_GPIO_PORT       	GPIOA                       /* GPIOA */
#define GDM1602K_Rs_PIN								GPIO_Pin_1

#define GDM1602K_RW_GPIO_PORT        	GPIOA                       /* GPIOA */
#define GDM1602K_RW_PIN								GPIO_Pin_2

#define GDM1602K_E_GPIO_PORT        	GPIOA                       /* GPIOA */
#define GDM1602K_E_PIN								GPIO_Pin_3

#define GDM1602K_DB0_GPIO_PORT        GPIOA                       /* GPIOA */
#define GDM1602K_DB0_PIN							GPIO_Pin_4

#define GDM1602K_DB1_GPIO_PORT        GPIOB                       /* GPIOB */
#define GDM1602K_DB1_PIN							GPIO_Pin_0

#define GDM1602K_DB2_GPIO_PORT        GPIOB                       /* GPIOB */
#define GDM1602K_DB2_PIN							GPIO_Pin_1

#define GDM1602K_DB3_GPIO_PORT        GPIOB                       /* GPIOB */
#define GDM1602K_DB3_PIN							GPIO_Pin_2

#define GDM1602K_DB4_GPIO_PORT        GPIOA                       /* GPIOA */
#define GDM1602K_DB4_PIN							GPIO_Pin_15

#define GDM1602K_DB5_GPIO_PORT        GPIOA                       /* GPIOA */
#define GDM1602K_DB5_PIN							GPIO_Pin_8

#define GDM1602K_DB6_GPIO_PORT        GPIOA                       /* GPIOA */
#define GDM1602K_DB6_PIN							GPIO_Pin_9

#define GDM1602K_DB7_GPIO_PORT        GPIOA                       /* GPIOA */
#define GDM1602K_DB7_PIN							GPIO_Pin_10

#define GDM1602K_BF_GPIO_PORT        	GPIOB                       /* GPIOB */
#define GDM1602K_BF_PIN							 	GPIO_Pin_5									/* GPIO_Pin_5 */


void gdm1602k_init(void);
void gdm1602k_display_on(uint8_t D, uint8_t C, uint8_t B);
void gdm1602k_clear(void);
void gdm1602k_return_home(void);
void gdm1602k_function_set(uint8_t DL,uint8_t N,uint8_t F);
void gdm1602k_set_cgram_addr(uint8_t b);
void gdm1602k_set_ddram_addr(uint8_t b);

void gdm1602k_write(uint8_t b);
void gdm1602k_display_uint8(uint8_t b);
void gdm1602k_space(void);
void gdm1602k_shift(uint8_t b);
void gdm1602k_display(char *str);
void gdm1602k_display_float(float f);
void gdm1602k_display_int(int i);

