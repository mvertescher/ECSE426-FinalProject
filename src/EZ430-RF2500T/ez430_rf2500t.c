/**
  ******************************************************************************
  * @file    ez430_rf2500t.c
  * @author  Matthew Vertescher
  * @version V1.0.0
  * @date    27-March-2014
  * @brief   This file provides a set of functions needed to manage the 
	*					 EZ430-RF2500T wireless target board.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ez430_rf2500t.h"

__IO uint32_t  EZ430_RF2500T_Timeout = EZ430_RF2500T_FLAG_TIMEOUT;  
/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)
/* */
#define USE_DEFAULT_TIMEOUT_CALLBACK


/** @defgroup EZ430-RF2500T_Private_FunctionPrototypes
  * @{
  */
static uint8_t EZ430_RF2500T_SendByte(uint8_t byte);
static void EZ430_RF2500T_LowLevel_Init(void);
/**
  * @}
  */

/**
  * @brief  Set EZ430_RF2500T Initialization.
  * @param  EZ430_RF2500T_Config_Struct: pointer to a EZ430_RF2500T_Config_TypeDef structure 
  *         that contains the configuration setting for the EZ430_RF2500T.
  * @retval None
  */
void EZ430_RF2500T_Init(EZ430_RF2500T_InitTypeDef *EZ430_RF2500T_InitStruct)
{
	uint8_t ctrl = 0x00;
  
  /* Configure the low level interface ---------------------------------------*/
  EZ430_RF2500T_LowLevel_Init();
}

/**
  * @brief  Initializes the low level interface used to drive the EZ430_RF2500T
  * @param  None
  * @retval None
  */
static void EZ430_RF2500T_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI periph */
  RCC_APB2PeriphClockCmd(EZ430_RF2500T_SPI_CLK, ENABLE);

  /* Enable SCK, MOSI and MISO GPIO clocks */
  RCC_AHB1PeriphClockCmd(EZ430_RF2500T_SPI_SCK_GPIO_CLK | 
												 EZ430_RF2500T_SPI_MISO_GPIO_CLK | 
												 EZ430_RF2500T_SPI_MOSI_GPIO_CLK, ENABLE);

  /* Enable CS GPIO clock */
  RCC_AHB1PeriphClockCmd(EZ430_RF2500T_SPI_CS_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(EZ430_RF2500T_SPI_SCK_GPIO_PORT, 
									 EZ430_RF2500T_SPI_SCK_SOURCE, 
									 EZ430_RF2500T_SPI_SCK_AF);
  GPIO_PinAFConfig(EZ430_RF2500T_SPI_MISO_GPIO_PORT, 
									 EZ430_RF2500T_SPI_MISO_SOURCE, 
									 EZ430_RF2500T_SPI_MISO_AF);
  GPIO_PinAFConfig(EZ430_RF2500T_SPI_MOSI_GPIO_PORT, 
									 EZ430_RF2500T_SPI_MOSI_SOURCE,
									 EZ430_RF2500T_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = EZ430_RF2500T_SPI_SCK_PIN;
  GPIO_Init(EZ430_RF2500T_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  EZ430_RF2500T_SPI_MOSI_PIN;
  GPIO_Init(EZ430_RF2500T_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = EZ430_RF2500T_SPI_MISO_PIN;
  GPIO_Init(EZ430_RF2500T_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(EZ430_RF2500T_SPI);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(EZ430_RF2500T_SPI, &SPI_InitStructure);

  /* Enable SPI2  */
  SPI_Cmd(EZ430_RF2500T_SPI, ENABLE);

  /* Configure GPIO PIN for Lis Chip select */
  GPIO_InitStructure.GPIO_Pin = EZ430_RF2500T_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(EZ430_RF2500T_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  GPIO_SetBits(EZ430_RF2500T_SPI_CS_GPIO_PORT, EZ430_RF2500T_SPI_CS_PIN);
  
  /* Configure GPIO PINs to detect Interrupts 
  GPIO_InitStructure.GPIO_Pin = LIS302DL_SPI_INT1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LIS302DL_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = LIS302DL_SPI_INT2_PIN;
  GPIO_Init(LIS302DL_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);*/
}

/**
  * @brief  Writes one byte to the EZ430_RF2500T.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the EZ430_RF2500T.
  * @param  WriteAddr : EZ430_RF2500T's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void EZ430_RF2500T_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  EZ430_RF2500T_CS_LOW();
  
  /* Send the Address of the indexed register */
  EZ430_RF2500T_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    EZ430_RF2500T_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  EZ430_RF2500T_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the EZ430_RF2500T.
  * @param  pBuffer : pointer to the buffer that receives the data read from the EZ430_RF2500T.
  * @param  ReadAddr : EZ430_RF2500T's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the EZ430_RF2500T.
  * @retval None
  */
void EZ430_RF2500T_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  EZ430_RF2500T_CS_LOW();
  
  /* Send the Address of the indexed register */
  EZ430_RF2500T_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LIS3DSH (Slave device) */
    *pBuffer = EZ430_RF2500T_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  EZ430_RF2500T_CS_HIGH();
}


/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t EZ430_RF2500T_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  EZ430_RF2500T_Timeout = EZ430_RF2500T_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(EZ430_RF2500T_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((EZ430_RF2500T_Timeout--) == 0) return EZ430_RF2500T_TIMEOUT_UserCallback();
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(EZ430_RF2500T_SPI, byte);
  
  /* Wait to receive a Byte */
  EZ430_RF2500T_Timeout = EZ430_RF2500T_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(EZ430_RF2500T_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((EZ430_RF2500T_Timeout--) == 0) return EZ430_RF2500T_TIMEOUT_UserCallback();
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(EZ430_RF2500T_SPI);
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t EZ430_RF2500T_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
//  while (1)
 // {   
  //}
	return 0;
}

#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

