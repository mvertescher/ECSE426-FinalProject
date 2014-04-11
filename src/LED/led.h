#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define HIGH_SIGNAL   -55
#define LOW_SIGNAL    -80
#define LED_GRADIENT  100/(HIGH_SIGNAL-LOW_SIGNAL)
#define LED_OFFSET    -LED_GRADIENT*LOW_SIGNAL

void enableBlueLED(void);
void set_Blue_LED_Intensity(uint8_t value);
void showSignalStrength(float signal);
void enableRedLED(void);
void set_Red_LED_Intensity(uint8_t value);
void enableOrangeLED(void);
void set_Orange_LED_Intensity(uint8_t value);
void enableGreenLED(void);
void set_Green_LED_Intensity(uint8_t value);
void enableLEDS(void);
