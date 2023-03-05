#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
#include "sys.h"
//Mini STM32开发板
//LED驱动代码			 
//正点原子@ALIENTEK
//2012/2/27

//LED端口定义
//#define LED0 PAout(8)// PA8
void GPIO_Config(void) ;
void TIM3_Config(void) ; 
	void Beep_Init(void);
#define BEEP PBout(12)// PA8

#define uchar unsigned char
#define SDA_IN()  {GPIOA->CRL &= 0XFF0FFFFF; GPIOA->CRL |= 8 << (5*4);}
#define SDA_OUT() {GPIOA->CRL &= 0XFF0FFFFF; GPIOA->CRL |= 3 << (5*4);}

#define IIC_SCL  PAout(4)
#define IIC_SDA  PAout(5)
#define READ_SDA PAin(5)


typedef unsigned char BYTE;
typedef unsigned short WORD;

#define SlaveAddress 0x46

void BH1750_Start ( void );
void BH1750_Stop ( void );
void BH1750_SendACK ( BYTE ack );
BYTE BH1750_RecvACK ( void );
void BH1750_SendByte ( BYTE dat );
BYTE BH1750_RecvByte ( void );
void Single_Write_BH1750 ( uchar REG_Address );
void Init_BH1750 ( void );
void Multiple_read_BH1750 ( void );	
#endif
