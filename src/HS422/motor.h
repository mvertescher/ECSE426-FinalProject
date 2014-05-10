#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


#define FEEDBACK_THRESHOLD		10
#define FEEDBACK_FACTOR				.05


void motorControl(float desired_pitch, float desired_roll, float meas_pitch, float meas_roll);
void motorEnable(void);