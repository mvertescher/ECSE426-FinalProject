#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include <stdio.h>
#include "smartrf_cc2500.h"
#include "cc2500.h"

/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void transmit_thread(void const * argument);
void receive_thread(void const *argument);

//! Thread structure for above thread
osThreadDef(transmit_thread, osPriorityNormal, 1, 0);
osThreadDef(receive_thread, osPriorityNormal, 1, 0);

/*!
 @brief Program entry point
 */
int main (void) {
	// ID for thread
	osThreadId tid_transmit;
    osThreadId tid_receive;
	
    /*GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure); */

	CC2500_InitTypeDef CC2500_InitStruct;
	CC2500_Init(&CC2500_InitStruct);
	
    uint8_t bffr = 0x00; 
    uint8_t arr[4];
    
    // Starting sequence
    bffr = CC2500_CommandProbe(CC2500_READBIT, CC2500_SRES);
    printf("Reseting = %x \n",bffr);
    
    bffr = CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
    printf("First status = %x \n",bffr);
    
    CC2500_SmartRF_Config();
    
    osDelay(1000);
    
    //CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_STX);
    //printf("Moving to TX (%x) \n",bffr);
    // Need to wait for CALIBRATE state to finish when moveing to RX
    //osDelay(1000);
    
    int i; 
    uint8_t pkt[10];
    for (i = 0; i < 10; i++) pkt[i] = 1+i;
    // Transmit loop 
    while (0) {
        // Reset
        //CC2500_Read(&bffr, CC2500_SRES, 1);
        //bffr = CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_SNOP);
        bffr = CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
        printf("Status = %x \n",bffr);
        osDelay(1000);
        
        //CC2500_CommandProbe(CC2500_READBIT, CC2500_FIFOADDR);
        printf("Writing %x to the tx buffer \n",bffr);
        CC2500_Write(&pkt[0], CC2500_FIFOADDR, 10); // Write to the TX buffer 
        osDelay(500);
        bffr = CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
        printf("Status = %x \n",bffr);
        osDelay(500);
        bffr = CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_STX);
        printf("Moving to TX (%x) \n",bffr);
        
        
        
        
        osDelay(8000);
        
        //while ((bffr & 0x70) != CC2500_STATE_TXFIFO_UNDERFLOW) // Wait to enter the underflow state
        //    bffr = CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
        //printf("Done waiting for underflow (%x) \n",bffr);
        
        //bffr = CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_SIDLE);
        //printf("Moving to idol (%x) \n",bffr);
        
        //printf("\n");
        
        
        /*
        CC2500_Read(&bffr, CC2500_SNOP, 1);
        printf("Read status = %x \n",bffr);
        osDelay(2000);*/
        
        /*  SINGLE READ works, cant read across multiple read only registers 
        CC2500_Read(arr, CC2500_PARTNUM, 1);
        CC2500_Read(&arr[1], CC2500_VERSION, 1);
        printf("PARTNUM = %x  VERSION = %x \n",arr[0],arr[1]); */
        
        //CC2500_Read(arr, CC2500_TEST0, 4);
        //printf("TEST0 = %x ADDR0x2F = %x PARTNUM = %x  VERSION = %x \n",arr[0],arr[1],arr[2],arr[3]);
        
        //CC2500_Read(arr, CC2500_AGCTEST, 4);
        //printf("AGCTEST = %x TEST2 = %x TEST1 = %x TEST0 = %x \n",arr[0],arr[1],arr[2],arr[3]);
        //osDelay(1000);
        //CC2500_Read(arr, CC2500_PARTNUM, 2);
        //printf("PARTNUM = %x  VERSION = %x \n",arr[0],arr[1]);
        
        //osDelay(2000);
    }
    
    /* Receive loop */
    /*
        When RX is activated the chip WILL remain in RX mode until a packet is successfully 
        received 
    */
    uint8_t status;
    uint8_t bytesToRead;
    status = CC2500_CommandProbe(CC2500_READBIT, CC2500_SRX);
    printf("Moving to RX (%x) \n",status);
    uint8_t buffer[30];
    //CC2500_RXBYTES
    while(1) {
        status = CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
        printf("Status (%x) \n",status);
        
        if ((status & 0x70) == CC2500_STATE_IDLE) { // if in idle state
            CC2500_Read(&bytesToRead, CC2500_RXBYTES,1);
            bytesToRead = bytesToRead & 0x7F;
            CC2500_Read(&buffer[0], CC2500_FIFOADDR, bytesToRead); 
            bytesToRead -= 2;
            int i;
            for (i = 0; i < bytesToRead; i++)
                printf("%x ",buffer[i]); 
            
            //printf("Read %i bytes \n",bytesToRead);
            status = CC2500_CommandProbe(CC2500_READBIT, CC2500_SRX);
            printf("\n Read %i bytes and moving back to RX (%x) \n",bytesToRead,status);
        }
        osDelay(500);

    }
    
    
    
    
    
    
    
    
    
    osDelay(osWaitForever);
    
    
	// Start thread
	printf("Starting thread\n");
    tid_transmit = osThreadCreate(osThread(transmit_thread), NULL);
    //tid_receive = osThreadCreate(osThread(receive_thread), NULL);


	// The below doesn't really need to be in a loop
	while(1){
		osDelay(osWaitForever);
	}
}

void transmit_thread(void const *argument) {
  
  uint8_t bffr = 0x00; 
  printf("BffrSTX before: %x \n",bffr);
  //CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_STX);
  CC2500_Read(&bffr, CC2500_STX, 1); // Transmit mode
  //CC2500_Read(&bffr, CC2500_SRX, 1); // Rec mode
  printf("BffrSTX after: %x %i \n\n",bffr,bffr);
    
  uint8_t send[5];
  send[0] = 1;
  send[1] = 2;
  send[2] = 4;
  send[3] = 6;
  send[4] = 8;
    
  uint8_t fifo_bytes_available  = 0;
  uint8_t state; 
  uint8_t rdy;
  
  osDelay(1000);
  while(1){
		osDelay(5);
				
        while(fifo_bytes_available != 0x0F || state != CC2500_STATE_TX) {
            CC2500_Write(&bffr, CC2500_SNOP, 1);
            fifo_bytes_available = bffr & 0x0F;
            state = (bffr & 0x70);
            rdy = (bffr & 0x80);
            printf("Rdy: %x   State: %x   TX Fifo bytes: %x \n",rdy,state,fifo_bytes_available);
            osDelay(500);
        }
      
        printf("Writing to TX fifo \n");
        CC2500_Write(send,CC2500_FIFOADDR,5);
        fifo_bytes_available = 0;
      
	}
}

void receive_thread(void const *argument) {
    
  uint8_t bffr = 0x00; 
  printf("BffrSTX before: %x \n",bffr);
  //CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_STX);
  //CC2500_Read(&bffr, CC2500_STX, 1); // Transmit mode
  CC2500_Read(&bffr, CC2500_SRX, 1); // Rec mode
  printf("BffrSTX after: %x %i \n\n",bffr,bffr);
  
  uint8_t get[5];
  get[0] = 0;
  get[1] = 0;
  get[2] = 0;
  get[3] = 0;
  get[4] = 0;
    
  uint8_t fifo_bytes_available  = 0x0F;
  uint8_t state; 
  uint8_t rdy;
    
  while (1) {
      
      // Waiting 
      while(fifo_bytes_available == 0x0F) {
            CC2500_Read(&bffr, CC2500_SNOP, 1);
            fifo_bytes_available = bffr & 0x0F;
            state = bffr >> 4;
            rdy = bffr >> 7;
            printf("Rdy: %x   State: %x   TX Fifo bytes: %x \n",rdy,state,fifo_bytes_available);
            osDelay(10);
      }
      
      printf("Reading to RX fifo \n");
      CC2500_Read(get,CC2500_FIFOADDR,5); 
      printf("%x  %x  %x  %x  %x \n",get[0],get[1],get[2],get[3],get[4]);      
      fifo_bytes_available = 0x0F;
      
  }
  
}

//void printStatusByte(uint8_t ) {
    
//}
