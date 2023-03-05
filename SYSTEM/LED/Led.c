#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/5/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved											  
////////////////////////////////////////////////////////////////////////////////// 	   

BYTE BUF[8]; 
void GPIO_Config(void)  
{ 
  GPIO_InitTypeDef GPIO_InitStructure; 
 
  /* GPIOA and GPIOB clock enable */ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  
 
  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */ 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
} 

void Beep_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //??PA????
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED0-->PA.8 ????
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_SetBits(GPIOB,GPIO_Pin_12);						 //PA.8 ???

}

 
void TIM3_Config(void)  
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure;  
    /* PWM??????? */  
    u16 CCR1= 36000;          
    u16 CCR2= 36000;  

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /* Time base configuration */                                            
    TIM_TimeBaseStructure.TIM_Period =CCR1;  
    TIM_TimeBaseStructure.TIM_Prescaler = 2;                                    //?????:???=2,??72/3=24MHz  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //????????:???  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //????????  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //???PWM??1  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR1;                                       //?????,???????????,??????  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //?????????CCR1?????  
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                                    //????1      
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  
    /* PWM1 Mode configuration: Channel2 */  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
    TIM_OCInitStructure.TIM_Pulse = CCR2;                                       //????2??????,??????????PWM  
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                    //?????????CCR2????? 
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                                    //????2  
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
    TIM_ARRPreloadConfig(TIM3, ENABLE);                                         //??TIM3?????ARR  
    /* TIM3 enable counter */  
    TIM_Cmd(TIM3, ENABLE);                                                      //??TIM3   
} 

static void IIC_Init ( void ) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );
    IIC_SCL = 1;
    IIC_SDA = 1;
}

static void IIC_Start ( void ) {
    SDA_OUT();
    IIC_SDA = 1;
    IIC_SCL = 1;
    delay_us ( 4 );
    IIC_SDA = 0;
    delay_us ( 4 );
    IIC_SCL = 0;
}

static void IIC_Stop ( void ) {
    SDA_OUT();
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us ( 4 );
    IIC_SCL = 1;
    IIC_SDA = 1;
    delay_us ( 4 );
}

static u8 IIC_Wait_Ack ( void ) {
    u8 ucErrTime = 0;
    SDA_IN();
    IIC_SDA = 1;
    delay_us ( 1 );
    IIC_SCL = 1;
    delay_us ( 1 );

    while ( READ_SDA ) {
        ucErrTime++;

        if ( ucErrTime > 250 ) {
            IIC_Stop();
            return 1;
        }
    }

    IIC_SCL = 0;
    return 0;
}

static void IIC_Ack ( void ) {
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 0;
    delay_us ( 2 );
    IIC_SCL = 1;
    delay_us ( 2 );
    IIC_SCL = 0;
}

static void IIC_NAck ( void ) {
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 1;
    delay_us ( 2 );
    IIC_SCL = 1;
    delay_us ( 2 );
    IIC_SCL = 0;
}

static void IIC_Send_Byte ( u8 txd ) {
    u8 t;
    SDA_OUT();
    IIC_SCL = 0;

    for ( t = 0; t < 8; t++ ) {
        IIC_SDA = ( txd & 0x80 ) >> 7;
        txd <<= 1;
        delay_us ( 2 );
        IIC_SCL = 1;
        delay_us ( 2 );
        IIC_SCL = 0;
        delay_us ( 2 );
    }
}

static u8 IIC_Read_Byte ( unsigned char ack ) {
    unsigned char i, receive = 0;
    SDA_IN();

    for ( i = 0; i < 8; i++ ) {
        IIC_SCL = 0;
        delay_us ( 2 );
        IIC_SCL = 1;
        receive <<= 1;

        if ( READ_SDA ) {
            receive++;
        }

        delay_us ( 1 );
    }

    return receive;
}

void BH1750_Start ( void ) { /* ????????? */
    IIC_Start();
}

void BH1750_Stop ( void ) { /* ????????? */
    IIC_Stop();
}

void BH1750_SendACK ( BYTE ack ) { /* ?????????ack?0?ACK,?1?NAK */
    if ( ack == 0 ) {
        IIC_Ack();
    } else {
        IIC_NAck();
    }
}

BYTE BH1750_RecvACK ( void ) { /* ?????? */
    return IIC_Wait_Ack();
}

void BH1750_SendByte ( BYTE dat ) { /* ?IIC?????????? */
    IIC_Send_Byte ( dat );
    BH1750_RecvACK();
}

BYTE BH1750_RecvByte ( void ) { /* ?IIC?????????? */
    return IIC_Read_Byte ( 0 );
}

void Single_Write_BH1750 ( uchar REG_Address ) {
    BH1750_Start(); /* ???? */
    BH1750_SendByte ( SlaveAddress ); /* ?????? + ??? */
    BH1750_SendByte ( REG_Address ); /* ??????? */
    BH1750_Stop(); /* ?????? */
}

void Init_BH1750 ( void ) { /* BH1750????? */
    IIC_Init();
    delay_ms ( 50 );
    Single_Write_BH1750 ( 0x01 );
}

void Multiple_read_BH1750 ( void ) { /* ????BH1750???? */
    uchar i;
    BH1750_Start(); /* ???? */
    BH1750_SendByte ( SlaveAddress + 1 ); /* ?????? + ??? */

    for ( i = 0; i < 3; i++ ) { /* ????2?????,???BUF? */
        BUF[i] = BH1750_RecvByte(); /* BUF[0]??0x32?????? */

        if ( i == 3 ) {
            BH1750_SendACK ( 1 ); /* ??????????NOACK */
        } else {
            BH1750_SendACK ( 0 ); /* ??ACK */
        }
    }

    BH1750_Stop(); /* ???? */
    delay_ms ( 5 );
}






 
