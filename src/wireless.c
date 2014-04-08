#include "wireless.h"

static void send_packet(uint8_t *pnt);
static void read_packet(packet_t *return_pktpnt);

uint8_t init_wireless(void) {	
    
    // Setup up CC2500 SPI
    CC2500_InitTypeDef CC2500_InitStruct;
    CC2500_Init(&CC2500_InitStruct);
	  
    // Send reset command
    CC2500_CommandProbe(CC2500_READBIT, CC2500_SRES);
    
    // Wait for CHIP_RDYn
    while (CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP) & 0x80); 
  
    // Configure the CC2500 registers 
    CC2500_SmartRF_Config();
    
    osDelay(1000);
  
    return CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
}


void transmit_pitchroll(float pitch, float roll) {

    packet_t pkt;
    pkt.f1 = pitch;
    pkt.f2 = roll;
    pkt.b1 = PACKET_CTRL1_PR;
    pkt.b2 = 2;
    
    //printf("        pitch = %f ", pkt.f1);
    uint8_t *pnt = (uint8_t *) &pkt;
    //printf("        pitch = %f ", pkt.f1);
    //printf("Data: %x %x %x %x %x %x %x %x %x %x \n",pnt[0],pnt[1],pnt[2],pnt[3],pnt[4],pnt[5],pnt[6],pnt[7],pnt[8],pnt[9]);
    
    // Put data into the TX FIFO buffer  
    CC2500_Write(pnt, CC2500_FIFOADDR, WIRELESS_PKT_DATALEN); // Write to the TX buffer 
    
    //while ((CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP) & 0x70) !=); 
    //osDelay(1000);
    
    //print_status();
    
    // Move into transmit state
    uint8_t status = CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_STX);
    printf("Moved to TX (%x) \n",status);
    
    // The CC2500 will move back into the idle state when the transmission has been sent 
    //wait_for_idle();
    osDelay(20);
    
}

uint16_t receive_pitchroll(float* pitch, float* roll) {
  // assume already in RX
    // Wait for a transmisson to be read. When this happens, the CC2500 will move to the idle state
    wait_for_idle();
    
    // Determine number of bytes to read
    uint8_t bytesToRead;
    CC2500_Read(&bytesToRead, CC2500_RXBYTES,1);
    bytesToRead = bytesToRead & 0x7F;
      
    uint8_t buffer[bytesToRead];
    CC2500_Read(&buffer[0], CC2500_FIFOADDR, bytesToRead); 
    bytesToRead -= 2;
    
    //printf("Data: %x %x %x %x %x %x %x %x %x %x \n ",buffer[0],buffer[1],buffer[2],buffer[3],
    //  buffer[4],buffer[5],buffer[6],buffer[7],buffer[8],buffer[9]);
    
    packet_t *pkt_pnt = (packet_t *) &buffer[0]; 
    *pitch = pkt_pnt->f1;
    *roll = pkt_pnt->f2;
    uint8_t ctrl1 = pkt_pnt->b1;
    uint8_t ctrl2 = pkt_pnt->b2;
    
    // Exract data
    //*pitch = buffer[0]; 
    //*roll = (float) buffer[4]; 
    uint16_t ctrl  = buffer[8];
    //printf("   Pitch: %f Roll: %f \n",pkt_pnt->f1,pkt_pnt->f2); 
  
    // Move back to recieve state 
    CC2500_CommandProbe(CC2500_READBIT, CC2500_SRX);
    return ctrl; 
}


void transmit_keypad_pitch(float pitch) {
    packet_t pkt;
    pkt.f1 = pitch;
    pkt.f2 = 0;
    pkt.b1 = PACKET_CTRL1_PITCH;
    pkt.b2 = 2;
    
    uint8_t *pnt = (uint8_t *) &pkt;
    send_packet(pnt);
}


void transmit_keypad_roll(float roll) {
    packet_t pkt;
    pkt.f1 = roll;
    pkt.f2 = 0;
    pkt.b1 = PACKET_CTRL1_ROLL;
    pkt.b2 = 2;
    
    uint8_t *pnt = (uint8_t *) &pkt;
    send_packet(pnt);
}
 
void transmit_keypad_time(float time) {
    packet_t pkt;
    pkt.f1 = time;
    pkt.f2 = 0;
    pkt.b1 = PACKET_CTRL1_TIME;
    pkt.b2 = 2;
    
    uint8_t *pnt = (uint8_t *) &pkt;
    send_packet(pnt);
}

void transmit_keypad_begin() {
    packet_t pkt;
    pkt.f1 = 0;
    pkt.f2 = 0;
    pkt.b1 = PACKET_CTRL1_BEGIN;
    pkt.b2 = 2;
    
    uint8_t *pnt = (uint8_t *) &pkt;
    send_packet(pnt);
}

void transmit_keypad_end() {
    packet_t pkt;
    pkt.f1 = 0;
    pkt.f2 = 0;
    pkt.b1 = PACKET_CTRL1_END;
    pkt.b2 = 2;
    
    uint8_t *pnt = (uint8_t *) &pkt;
    send_packet(pnt);
}

void transmit_record_sequence(int size, float *pitchBuffer, float *rollBuffer) {
		packet_t pkt;
    pkt.f1 = 0;
    pkt.f2 = 0;
    pkt.b1 = PACKET_CTRL1_RECORD_BEGIN;
    pkt.b2 = 0;
		uint8_t *pnt = (uint8_t *) &pkt;
		
		// Send start packet
		send_packet(pnt);

		pkt.b1 = PACKET_CTRL1_RECORD_PKT;
		uint8_t index;
		for (index = 0; index < size; index++) {
			pkt.f1 = pitchBuffer[index];
			pkt.f2 = pitchBuffer[index];
			pkt.b2 = index;
			 
			send_packet(pnt);
			
			osDelay(30); // Could be 20
		}
		
		pkt.f1 = 0;
    pkt.f2 = 0;
    pkt.b1 = PACKET_CTRL1_RECORD_END;
    pkt.b2 = 0;
		
		// Send end packet
		send_packet(pnt);
		
		// Wait for sequence to complete
		osDelay(5000);
}

void receive_record_sequence(float *pitchBuffer, float *rollBuffer) {
		uint8_t ctrl1 = PACKET_CTRL1_RECORD_PKT;
		uint8_t index;
		packet_t pkt;
	
		while (ctrl1 != PACKET_CTRL1_RECORD_END) {
			wait_for_idle();
			read_packet(&pkt);
			pitchBuffer[pkt.b2] = pkt.f1;
			pitchBuffer[pkt.b2] = pkt.f2;
			ctrl1 = pkt.b1;
		}
}

void receive_keypad(uint8_t *ctrl, float *value) {
    wait_for_idle();
  
    // Determine number of bytes to read
    uint8_t bytesToRead;
    CC2500_Read(&bytesToRead, CC2500_RXBYTES,1);
    bytesToRead = bytesToRead & 0x7F;
      
    uint8_t buffer[bytesToRead];
    CC2500_Read(&buffer[0], CC2500_FIFOADDR, bytesToRead); 
    bytesToRead -= 2;
    
    
    packet_t *pkt_pnt = (packet_t *) &buffer[0]; 
    *value = pkt_pnt->f1;
    *ctrl = pkt_pnt->b1;
    //uint8_t ctrl2 = pkt_pnt->b2;
    
    // Exract data
    //*pitch = buffer[0]; 
    //*roll = (float) buffer[4]; 
    //uint16_t ctrl  = buffer[8];
    //printf("   Pitch: %f Roll: %f \n",pkt_pnt->f1,pkt_pnt->f2); 
  
    // Move back to recieve state 
    CC2500_CommandProbe(CC2500_READBIT, CC2500_SRX);

}


void send_packet(uint8_t *pnt) {
    // Put data into the TX FIFO buffer  
    CC2500_Write(pnt, CC2500_FIFOADDR, WIRELESS_PKT_DATALEN); // Write to the TX buffer 

    // Move into transmit state
    uint8_t status = CC2500_CommandProbe(CC2500_WRITEBIT, CC2500_STX);
    
    // The CC2500 will move back into the idle state when the transmission has been sent 
    osDelay(20);
}

void read_packet(packet_t *return_pktpnt) {
    // Determine number of bytes to read
		uint8_t bytesToRead;
		CC2500_Read(&bytesToRead, CC2500_RXBYTES,1);
    bytesToRead = bytesToRead & 0x7F;
      
    uint8_t buffer[bytesToRead];
    CC2500_Read(&buffer[0], CC2500_FIFOADDR, bytesToRead); 
    bytesToRead -= 2;

    packet_t *pkt_pnt = (packet_t *) &buffer[0]; 
    return_pktpnt->f1 = pkt_pnt->f1;
    return_pktpnt->f2 = pkt_pnt->f2;
    return_pktpnt->b1 = pkt_pnt->b1;
    return_pktpnt->b2 = pkt_pnt->b2;
    
    // Move back to recieve state 
    CC2500_CommandProbe(CC2500_READBIT, CC2500_SRX);
}   
    

void wait_for_idle(void) {
    uint8_t status = CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
    while ((status & 0x70) != CC2500_STATE_IDLE) {
        //printf("Waiting for idle state. (%x) \n",status);
        osDelay(20);
        status = CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
    }  
}  

void print_status(void) {
    uint8_t status = CC2500_CommandProbe(CC2500_READBIT, CC2500_SNOP);
    printf("Status (%x) \n",status);
}

