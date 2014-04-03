/**
  ******************************************************************************
  * @file    cc2500.c
  * @author  Matthew Vertescher
  * @version V1.0.0
  * @date    27-March-2014
  * @brief   This file provides a set of functions needed to manage the 
  *			 CC2500 wireless chip.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cc2500.h"
#include "smartrf_cc2500.h"

__IO uint32_t  CC2500_Timeout = CC2500_FLAG_TIMEOUT;  
/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)
/* */
#define USE_DEFAULT_TIMEOUT_CALLBACK


/** @defgroup CC2500_Private_FunctionPrototypes
  * @{
  */
static uint8_t CC2500_SendByte(uint8_t byte);
static void CC2500_LowLevel_Init(void);

/**
  * @}
  */

/**
  * @brief  Set CC2500 Initialization.
  * @param  CC2500_Config_Struct: pointer to a CC2500_Config_TypeDef structure 
  *         that contains the configuration setting for the CC2500.
  * @retval None
  */
void CC2500_Init(CC2500_InitTypeDef *CC2500_InitStruct)
{
	//uint8_t ctrl = 0x00;
  
    /* Configure the low level interface ---------------------------------------*/
    CC2500_LowLevel_Init();
	
	/* Configure the CC2500 ----------------------------------------------------*/
	//CC2500_SmartRF_Config();
}

/**
  * @brief  Initializes the low level interface used to drive the EZ430_RF2500T
  * @param  None
  * @retval None
  */
static void CC2500_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI periph */
  RCC_APB1PeriphClockCmd(CC2500_SPI_CLK, ENABLE); // WATCH FOR APB1/2

  /* Enable SCK, MOSI and MISO GPIO clocks */
  RCC_AHB1PeriphClockCmd(CC2500_SPI_SCK_GPIO_CLK | 
												 CC2500_SPI_MISO_GPIO_CLK | 
												 CC2500_SPI_MOSI_GPIO_CLK, ENABLE);

  /* Enable CS GPIO clock */
  RCC_AHB1PeriphClockCmd(CC2500_SPI_CS_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(CC2500_SPI_SCK_GPIO_PORT, 
									 CC2500_SPI_SCK_SOURCE, 
									 CC2500_SPI_SCK_AF);
  GPIO_PinAFConfig(CC2500_SPI_MISO_GPIO_PORT, 
									 CC2500_SPI_MISO_SOURCE, 
									 CC2500_SPI_MISO_AF);
  GPIO_PinAFConfig(CC2500_SPI_MOSI_GPIO_PORT, 
									 CC2500_SPI_MOSI_SOURCE,
									 CC2500_SPI_MOSI_AF);

  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN; // Not input, don't pull GPIO_PuPd_DOWN;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 100hz?
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // Check pull NOPULL


  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_SCK_PIN;
  GPIO_Init(CC2500_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  CC2500_SPI_MOSI_PIN;
  GPIO_Init(CC2500_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_MISO_PIN;
  GPIO_Init(CC2500_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(CC2500_SPI);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // Data is valid on clock leading edge
  //SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // SPI frequency is APB1 frequency / 4
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(CC2500_SPI, &SPI_InitStructure);

  /* Enable SPI2  */
  SPI_Cmd(CC2500_SPI, ENABLE);

  /* Configure GPIO PIN for Lis Chip select */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // added 
  GPIO_Init(CC2500_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN);
  
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
  * @brief  Configures the CC2500.
  * @param  None
  * @retval None
  */
void CC2500_SmartRF_Config(void)
{
	uint8_t ctrl; 
	
  ctrl = SMARTRF_SETTING_FSCTRL1;
  CC2500_Write(&ctrl, CC2500_FSCTRL1, 1);
  
  ctrl = SMARTRF_SETTING_FSCTRL0;
  CC2500_Write(&ctrl, CC2500_FSCTRL0, 1);
  
  ctrl = SMARTRF_SETTING_FREQ2;
  CC2500_Write(&ctrl, CC2500_FREQ2, 1);
  
  ctrl = SMARTRF_SETTING_FREQ1;
  CC2500_Write(&ctrl, CC2500_FREQ1, 1);
  
  ctrl = SMARTRF_SETTING_FREQ0;
  CC2500_Write(&ctrl, CC2500_FREQ0, 1);
  
  ctrl = SMARTRF_SETTING_MDMCFG4;
  CC2500_Write(&ctrl, CC2500_MDMCFG4, 1);
  
  ctrl = SMARTRF_SETTING_MDMCFG3;
  CC2500_Write(&ctrl, CC2500_MDMCFG3, 1);
    
  ctrl = SMARTRF_SETTING_MDMCFG2;
  CC2500_Write(&ctrl, CC2500_MDMCFG2, 1);
  
  ctrl = SMARTRF_SETTING_MDMCFG1;
  CC2500_Write(&ctrl, CC2500_MDMCFG1, 1);
  
  ctrl = SMARTRF_SETTING_MDMCFG0;
  CC2500_Write(&ctrl, CC2500_MDMCFG0, 1);
  
  ctrl = SMARTRF_SETTING_CHANNR;
  CC2500_Write(&ctrl, CC2500_CHANNR, 1);
  
  ctrl = SMARTRF_SETTING_DEVIATN;
  CC2500_Write(&ctrl, CC2500_DEVIATN, 1);
  
  ctrl = SMARTRF_SETTING_FREND1;
  CC2500_Write(&ctrl, CC2500_FREND1, 1);
  
  ctrl = SMARTRF_SETTING_FREND0;
  CC2500_Write(&ctrl, CC2500_FREND0, 1);
  
  ctrl = SMARTRF_SETTING_MCSM1; 
  CC2500_Write(&ctrl, CC2500_MCSM1, 1);  

  ctrl = SMARTRF_SETTING_MCSM0; 
  CC2500_Write(&ctrl, CC2500_MCSM0, 1);  
  
  ctrl = SMARTRF_SETTING_FOCCFG; 
  CC2500_Write(&ctrl, CC2500_FOCCFG, 1);
  
  ctrl = SMARTRF_SETTING_BSCFG;
  CC2500_Write(&ctrl, CC2500_BSCFG, 1);
  
  ctrl = SMARTRF_SETTING_AGCCTRL2;
  CC2500_Write(&ctrl, CC2500_AGCCTRL2, 1);
  
  ctrl = SMARTRF_SETTING_AGCCTRL1;
  CC2500_Write(&ctrl, CC2500_AGCCTRL1, 1);
  
  ctrl = SMARTRF_SETTING_AGCCTRL0;
  CC2500_Write(&ctrl, CC2500_AGCCTRL0, 1);
  
  ctrl = SMARTRF_SETTING_FSCAL3;
  CC2500_Write(&ctrl, CC2500_FSCAL3, 1);
  
  ctrl = SMARTRF_SETTING_FSCAL2;
  CC2500_Write(&ctrl, CC2500_FSCAL2, 1);
  
  ctrl = SMARTRF_SETTING_FSCAL1;
  CC2500_Write(&ctrl, CC2500_FSCAL1, 1);
  
  ctrl = SMARTRF_SETTING_FSCAL0;
  CC2500_Write(&ctrl, CC2500_FSCAL0, 1);
  
  ctrl = SMARTRF_SETTING_FSTEST;
  CC2500_Write(&ctrl, CC2500_FSTEST, 1);
  
  ctrl = SMARTRF_SETTING_TEST2;
  CC2500_Write(&ctrl, CC2500_TEST2, 1);
  
  ctrl = SMARTRF_SETTING_TEST1;
  CC2500_Write(&ctrl, CC2500_TEST1, 1);
  
  ctrl = SMARTRF_SETTING_TEST0;
  CC2500_Write(&ctrl, CC2500_TEST0, 1);
  
  ctrl = SMARTRF_SETTING_FIFOTHR;
  CC2500_Write(&ctrl, CC2500_FIFOTHR, 1);
  
  ctrl = SMARTRF_SETTING_IOCFG2;
  CC2500_Write(&ctrl, CC2500_IOCFG2, 1);
  
  ctrl = SMARTRF_SETTING_IOCFG0;
  CC2500_Write(&ctrl, CC2500_IOCFG0, 1);
  
  ctrl = SMARTRF_SETTING_PKTCTRL1;
  CC2500_Write(&ctrl, CC2500_PKTCTRL1, 1);
  
  ctrl = SMARTRF_SETTING_PKTCTRL0;
  CC2500_Write(&ctrl, CC2500_PKTCTRL0, 1);
  
  ctrl = SMARTRF_SETTING_ADDR;
  CC2500_Write(&ctrl, CC2500_ADDR, 1);
  
  ctrl = SMARTRF_SETTING_PKTLEN;
  CC2500_Write(&ctrl, CC2500_PKTLEN, 1);
}

void CC2500_RebootCmd(void) {
    CC2500_CommandProbe(CC2500_READBIT, CC2500_SRES);
}

uint8_t CC2500_CommandProbe(uint8_t rw, uint8_t CmdPrbAddr)
{   
    uint8_t status;
    if (rw) {
        CmdPrbAddr |= (uint8_t)READWRITE_CMD;
    }
	/* Set chip select Low at the start of the transmission */
    CC2500_CS_LOW();
  
    /* Send the Address of the indexed register */
    CC2500_SendByte(CmdPrbAddr);
    
    /* Read the status byte */
    status = CC2500_SendByte(DUMMY_BYTE);
    
    /* Set chip select High at the end of the transmission */ 
    CC2500_CS_HIGH();
    
    return status; 
}


uint8_t CC2500_TransmitBytes(uint8_t* pBuffer, uint16_t NumByteToSend) {
		if (NumByteToSend != CC2500_PACKETLENGTH)
			return 0xFF;
		CC2500_Write(&pBuffer[0], CC2500_FIFOADDR, NumByteToSend); // Write to the TX buffer 
    return CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_STX); // Return the status
}

uint8_t CC2500_RecieveBytes(uint8_t* pBuffer, uint16_t NumByteToRead) {
		CC2500_Read(&pBuffer[0], CC2500_FIFOADDR, NumByteToRead); // Read from the RX buffer 
    return CC2500_CommandProbe(CC2500_READBIT, CC2500_SRX); // Return the status
}


/**
  * @brief  Writes one byte to the CC2500.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the CC2500.
  * @param  WriteAddr : CC2500's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void CC2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
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
  CC2500_CS_LOW();
  
  /* Send the Address of the indexed register */
  CC2500_SendByte(WriteAddr);
  
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    CC2500_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the CC2500.
  * @param  pBuffer : pointer to the buffer that receives the data read from the CC2500.
  * @param  ReadAddr : CC2500's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the CC2500.
  * @retval None
  */
void CC2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
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
  CC2500_CS_LOW();
  
  /* Send the Address of the indexed register */
  CC2500_SendByte(ReadAddr);
  /*pBuffer = EZ430_RF2500T_SendByte(ReadAddr); */
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LIS3DSH (Slave device) */
    *pBuffer = CC2500_SendByte(DUMMY_BYTE);
    //EZ430_RF2500T_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
}


/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t CC2500_SendByte(uint8_t byte)
{
  /* Loop while DR register in not empty */
  CC2500_Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((CC2500_Timeout--) == 0) return CC2500_TIMEOUT_UserCallback();
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(CC2500_SPI, byte);
  
  /* Wait to receive a Byte */
  CC2500_Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((CC2500_Timeout--) == 0) return CC2500_TIMEOUT_UserCallback();
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(CC2500_SPI);
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t CC2500_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
//  while (1)
 // {   
  //}
	return 0;
}

#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

