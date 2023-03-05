#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"


void Timer2_Init_Config(void);
void pwm_init(void);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init1(void);
void duji1(void);
void duji2(void);
void servo_angle(int ang);
#endif
