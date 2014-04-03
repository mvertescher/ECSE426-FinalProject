/**
  ******************************************************************************
  * @file    cc2500.h
  * @author  Matthew Vertescher
  * @version V1.0.0
  * @date    27-March-2014
  * @brief   This file contains all the functions prototypes for the cc2500.c
  *          firmware driver.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CC2500_H
#define __CC2500_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

/* CC2500  struct */
typedef struct
{
  uint8_t on;
}CC2500_InitTypeDef;

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define CC2500_FLAG_TIMEOUT         ((uint32_t)0xF000) //  ((uint32_t)0x1000)
	 
/**
  * @brief  CC2500 SPI Interface pins
  */

#define CC2500_SPI                       SPI2
#define CC2500_SPI_CLK                   RCC_APB1Periph_SPI2

/* EZ430_RF2500T PIN 16 */
#define CC2500_SPI_SCK_PIN               GPIO_Pin_13                 /* PB.13 */
#define CC2500_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define CC2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define CC2500_SPI_SCK_SOURCE            GPIO_PinSource13
#define CC2500_SPI_SCK_AF                GPIO_AF_SPI2

/* EZ430_RF2500T PIN 15 */
#define CC2500_SPI_MISO_PIN              GPIO_Pin_14                 /* PB.14 */
#define CC2500_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define CC2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define CC2500_SPI_MISO_SOURCE           GPIO_PinSource14
#define CC2500_SPI_MISO_AF               GPIO_AF_SPI2

/* EZ430_RF2500T PIN 18 */
#define CC2500_SPI_MOSI_PIN              GPIO_Pin_15                  /* PA.15 */
#define CC2500_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define CC2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define CC2500_SPI_MOSI_SOURCE           GPIO_PinSource15
#define CC2500_SPI_MOSI_AF               GPIO_AF_SPI2

/* EZ430_RF2500T PIN 17 */
#define CC2500_SPI_CS_PIN                GPIO_Pin_3                  /* PE.03 */
#define CC2500_SPI_CS_GPIO_PORT          GPIOE                       /* GPIOE */
#define CC2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE



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


/* Command strobes */ 
#define CC2500_SRES       0x30    /* Chip reset */
#define CC2500_SFSTXON    0x31
#define CC2500_SXOFF      0x32
#define CC2500_SCAL       0x33    /* Calibrate frequency synthesizer and turn it off */
#define CC2500_SRX        0x34    /* Enable RX */
#define CC2500_STX        0x35    /* In idle state: enable tx */
#define CC2500_SIDLE      0x36  
#define CC2500_SWOR       0x38
#define CC2500_SPWD       0x39
#define CC2500_SFRX       0x3A
#define CC2500_SFTX       0x3B
#define CC2500_SWORRST    0x3C
#define CC2500_SNOP       0x3D    /* No operation */

/* Configuration Registers */ 
#define CC2500_IOCFG2     0x00    /* GDO2 Output Pin Configuration */
#define CC2500_IOCFG1     0x01
#define CC2500_IOCFG0     0x02
#define CC2500_FIFOTHR    0x03
#define CC2500_SYNC1      0x04
#define CC2500_SYNC0      0x05
#define CC2500_PKTLEN     0x06
#define CC2500_PKTCTRL1   0x07
#define CC2500_PKTCTRL0   0x08
#define CC2500_ADDR       0x09
#define CC2500_CHANNR     0x0A
#define CC2500_FSCTRL1    0x0B 
#define CC2500_FSCTRL0    0x0C    
#define CC2500_FREQ2      0x0D 
#define CC2500_FREQ1      0x0E 
#define CC2500_FREQ0      0x0F 
#define CC2500_MDMCFG4    0x10 
#define CC2500_MDMCFG3    0x11 
#define CC2500_MDMCFG2    0x12 
#define CC2500_MDMCFG1    0x13    
#define CC2500_MDMCFG0    0x14
#define CC2500_DEVIATN    0x15
#define CC2500_MCSM2      0x16
#define CC2500_MCSM1      0x17
#define CC2500_MCSM0      0x18
#define CC2500_FOCCFG     0x19
#define CC2500_BSCFG      0x1A
#define CC2500_AGCCTRL2   0x1B
#define CC2500_AGCCTRL1   0x1C
#define CC2500_AGCCTRL0   0x1D
#define CC2500_WOREVT1    0x1E
#define CC2500_WOREVT0    0x1F
#define CC2500_WORCTRL    0x20
#define CC2500_FREND1     0x21
#define CC2500_FREND0     0x22
#define CC2500_FSCAL3     0x23
#define CC2500_FSCAL2     0x24
#define CC2500_FSCAL1     0x25
#define CC2500_FSCAL0     0x26
#define CC2500_RCCTRL1    0x27
#define CC2500_RCCTRL0    0x28
#define CC2500_FSTEST     0x29
#define CC2500_PTEST      0x2A
#define CC2500_AGCTEST    0x2B      /* R/W, 0x3F by default */ 
#define CC2500_TEST2      0x2C      /* R/W, 0x88 by default */ 
#define CC2500_TEST1      0x2D      /* R/W, 0x31 by default */ 
#define CC2500_TEST0      0x2E      /* R/W, 0x0B by default 000010 11 = B */ 

/* Status Registers */
#define CC2500_PARTNUM    0xF0      /* Read only, 0x80 default */
#define CC2500_VERSION    0xF1      /* Read only, 0x03 default */


/* FIFO Buffer Access Headers */ // not needed addr = 0x3F
#define CC2500_TX_SINGLE  0x3F // transmit 1
#define CC2500_TX_BURST   0x7F // transmit many
#define CC2500_RX_SINGLE  0xBF // recieve 1
#define CC2500_RX_BURST   0xFF // recieve many 

#define CC2500_FIFOADDR   0x3F // 
#define CC2500_TX_ADDR    0x3F //
#define CC2500_RX_ADDR    0xBF //

#define CC2500_WRITEBIT   0x01
#define CC2500_READBIT    0x01

/* States  */
#define CC2500_CHIP_RDYn                0x80
#define CC2500_STATE_IDLE               0x00
#define CC2500_STATE_RX                 0x10
#define CC2500_STATE_TX                 0x20
#define CC2500_STATE_FSTXON             0x30
#define CC2500_STATE_CALIBRATE          0x40
#define CC2500_STATE_SETTLING           0x50
#define CC2500_STATE_RXFIFO_OVERFLOW    0x60
#define CC2500_STATE_TXFIFO_UNDERFLOW   0x70

/* */ 
#define CC2500_RXBYTES                  0xFB

#define CC2500_PACKETLENGTH							SMARTRF_SETTING_PKTLEN


/** @defgroup CC2500_Exported_Macros
  * @{
  */
#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
/**
  * @}
  */ 



void CC2500_Init(CC2500_InitTypeDef *CC2500_InitStruct);
void CC2500_SmartRF_Config(void);
//void LIS302DL_InterruptConfig(LIS302DL_InterruptConfigTypeDef *LIS302DL_InterruptConfigStruct);
//void LIS302DL_FilterConfig(LIS302DL_FilterConfigTypeDef *LIS302DL_FilterConfigStruct);
//void LIS302DL_LowpowerCmd(uint8_t LowPowerMode);
//void LIS302DL_FullScaleCmd(uint8_t FS_value);
//void LIS302DL_DataRateCmd(uint8_t DataRateValue);
void CC2500_RebootCmd(void);
//void LIS302DL_ReadACC(int32_t* out);
uint8_t CC2500_CommandProbe(uint8_t rw, uint8_t CmdPrbAddr);
uint8_t CC2500_TransmitBytes(uint8_t* pBuffer, uint16_t NumByteToSend);
uint8_t CC2500_RecieveBytes(uint8_t* pBuffer, uint16_t NumByteToRead);

void CC2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void CC2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);


uint32_t CC2500_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __EZ430_RF2500T_H */

