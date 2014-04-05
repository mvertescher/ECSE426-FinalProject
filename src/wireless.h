
#ifndef __wireless
#define __wireless
#endif

#include <stdio.h>

#include "cc2500.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"


#define WIRELESS_PKT_DATALEN      10       /* Two 32bit floats = 8 bytes */

/* Packet struct */
typedef struct
{
  float f1;
  float f2;
  uint8_t b1;
  uint8_t b2;
}packet_t;



uint8_t init_wireless(void);
void transmit_pitchroll(float pitch, float roll);
uint16_t receive_pitchroll(float* pitch, float* roll);

void wait_for_idle(void);
void print_status(void);
