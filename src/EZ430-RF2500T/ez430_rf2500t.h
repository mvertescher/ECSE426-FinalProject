/**
  ******************************************************************************
  * @file    ez430_rf2500t.h
  * @author  Matthew Vertescher
  * @version V1.0.0
  * @date    27-March-2014
  * @brief   This file contains all the functions prototypes for the ez430_rf2500t.c
  *          firmware driver.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EZ430_RF2500T_H
#define __EZ430_RF2500T_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

/* EZ430_RF2500T  struct */
typedef struct
{
  uint8_t on;
}EZ430_RF2500T_InitTypeDef;

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define EZ430_RF2500T_FLAG_TIMEOUT         ((uint32_t)0x1000)
	 
/**
  * @brief  EZ430_RF2500T SPI Interface pins
  */
#define EZ430_RF2500T_SPI                       SPI2
#define EZ430_RF2500T_SPI_CLK                   RCC_APB1Periph_SPI2

#define EZ430_RF2500T_SPI_SCK_PIN               GPIO_Pin_13                 /* PB.13 */
#define EZ430_RF2500T_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define EZ430_RF2500T_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define EZ430_RF2500T_SPI_SCK_SOURCE            GPIO_PinSource13
#define EZ430_RF2500T_SPI_SCK_AF                GPIO_AF_SPI2

#define EZ430_RF2500T_SPI_MISO_PIN              GPIO_Pin_14                 /* PB.14 */
#define EZ430_RF2500T_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define EZ430_RF2500T_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define EZ430_RF2500T_SPI_MISO_SOURCE           GPIO_PinSource14
#define EZ430_RF2500T_SPI_MISO_AF               GPIO_AF_SPI2

#define EZ430_RF2500T_SPI_MOSI_PIN              GPIO_Pin_15                  /* PA.15 */
#define EZ430_RF2500T_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define EZ430_RF2500T_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define EZ430_RF2500T_SPI_MOSI_SOURCE           GPIO_PinSource15
#define EZ430_RF2500T_SPI_MOSI_AF               GPIO_AF_SPI2

#define EZ430_RF2500T_SPI_CS_PIN                GPIO_Pin_3                  /* PE.03 */
#define EZ430_RF2500T_SPI_CS_GPIO_PORT          GPIOE                       /* GPIOE */
#define EZ430_RF2500T_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE

//#define EZ430_RF2500T_SPI_INT1_PIN              GPIO_Pin_0                  /* PE.00 */
//#define EZ430_RF2500T_SPI_INT1_GPIO_PORT        GPIOE                       /* GPIOE */
//#define EZ430_RF2500T_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOE
//#define EZ430_RF2500T_SPI_INT1_EXTI_LINE        EXTI_Line0
//#define EZ430_RF2500T_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
//#define EZ430_RF2500T_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource0
//#define EZ430_RF2500T_SPI_INT1_EXTI_IRQn        EXTI0_IRQn 

//#define EZ430_RF2500T_SPI_INT2_PIN              GPIO_Pin_1                  /* PE.01 */
//#define EZ430_RF2500T_SPI_INT2_GPIO_PORT        GPIOE                       /* GPIOE */
//#define EZ430_RF2500T_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOE
//#define EZ430_RF2500T_SPI_INT2_EXTI_LINE        EXTI_Line1
//#define EZ430_RF2500T_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
//#define EZ430_RF2500T_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
//#define EZ430_RF2500T_SPI_INT2_EXTI_IRQn        EXTI1_IRQn 	 
	 
	 
void EZ430_RF2500T_Init(EZ430_RF2500T_InitTypeDef *EZ430_RF2500T_InitStruct);
//void LIS302DL_InterruptConfig(LIS302DL_InterruptConfigTypeDef *LIS302DL_InterruptConfigStruct);
//void LIS302DL_FilterConfig(LIS302DL_FilterConfigTypeDef *LIS302DL_FilterConfigStruct);
//void LIS302DL_LowpowerCmd(uint8_t LowPowerMode);
//void LIS302DL_FullScaleCmd(uint8_t FS_value);
//void LIS302DL_DataRateCmd(uint8_t DataRateValue);
//void LIS302DL_RebootCmd(void);
//void LIS302DL_ReadACC(int32_t* out);
void LIS302DL_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void LIS302DL_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);


uint32_t EZ430_RF2500T_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __EZ430_RF2500T_H */

