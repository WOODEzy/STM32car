#ifndef __ADC_H
#define __ADC_H	
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//ADC 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/7 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved					   
//////////////////////////////////////////////////////////////////////////////////	 

/*
#define ADC_CH0  0 //通道0
#define ADC_CH1  1 //通道1
#define ADC_CH2  2 //通道2
#define ADC_CH3  3 //通道3	   
*/
void Adc_GPIO_Config(void); 
void Adc_Config(void);
float GetMQ_2Res(void);
 float MQ2_PPM_Calibration(float RS_1);
 float AD1_value();
#endif 
