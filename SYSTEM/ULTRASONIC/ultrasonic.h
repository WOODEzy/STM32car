#ifndef _ultrasonic_H
#define _ultrasonic_H
#include "sys.h"



//------------------------------------------------------------��ֲ�޸���-----------------------------------------------------------------------

#define ULTRA_PORT	GPIOA
#define ULTRA_CLK       RCC_APB2Periph_GPIOA
#define ULTRA_TRIG			GPIO_Pin_5
#define ULTRA_ECHO			GPIO_Pin_6

#define TRIG_Send  PAout(5)
#define ECHO_Reci  PAin(6)


//---------------------------------------------------------------------------------------------------------------------------------------------
void Ultran_Init(void);
void Ultra_Ranging(float *p);




#endif

