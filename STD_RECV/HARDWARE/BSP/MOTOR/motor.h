#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "sys.h"


void TIM4_PWM_Init(u16 arr,u16 psc);
void LIMIT_16(float x,float low,float high);
void PWM_Control(void);

#endif

