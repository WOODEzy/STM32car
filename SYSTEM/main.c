#include "stm32f10x.h"
#include "delay.h"
#include "motor.h"
#include "keysacn.h"
#include "IRSEARCH.h"
#include "oled.h"
#include "ultrasonic.h"
#include "stdio.h"
#include "sys.h"
#include "timer.h"
#include "adc.h"
#include "led.h"
#include "usart.h"	 
#include "math.h"
/*
void ZYSTM32_run(int speed,int time);       //前进函数
void ZYSTM32_brake(int time);               //刹车函数
void ZYSTM32_Left(int speed,int time);      //左转函数
void ZYSTM32_Spin_Left(int speed,int time); //左旋转函数
void ZYSTM32_Right(int speed,int time);     //右转函数
void ZYSTM32_Spin_Right(int speed,int time);//右旋转函数
void ZYSTM32_back(int speed,int time);      //后退函数

*/
float temp =0;
char DisNum[16]={0};
extern int timer;
extern void  TIM3_Int_Init(u16 arr,u16 psc);
float Res,PPM,ad0,R0;
 int main(void)
 {	
	 int i =0;
	 int angle;
	 delay_init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
		OLED_Init();			// 
		OLED_Clear(); 
		Ultran_Init();
	 TIM3_PWM_Init1();
		
	 Adc_Config();
	 Beep_Init();
	 USART1_Init_Config(115200);
//	for(i=0;i<20;i++)
// 	{
//		delay_ms(1000);
//	}
		Res = GetMQ_2Res();		
		R0 = MQ2_PPM_Calibration(Res);
	 OLED_ShowString(0,1,(unsigned char*)"DIST:",12); 
	 OLED_ShowString(0,2,(unsigned char*)"MQ2 :",12); 
		while(1)
		{  
				Res = GetMQ_2Res();		
				PPM = 613.9*pow(Res/R0,-2.074f);
		   	ad0 = AD1_value();
			
			  if(PPM>100)
				{
					BEEP =0;
				}else
				{
					BEEP =1;
				}
			
				if(ad0>3.0)
				{
				  duji1();
				}
				
				if(temp<=0.05)
				{
					BEEP =0;
				}else if((0.05<temp)&&(temp<=0.10))
				{
					BEEP =0;
					delay_ms(50);
					BEEP =1;
					delay_ms(50);
				}else if((0.10<temp)&&(temp<=0.15))
				{
					BEEP =0;
					delay_ms(100);
					BEEP =1;
					delay_ms(100);
				}
				
				
			  Ultra_Ranging(&temp);
			  sprintf(DisNum,"%.2f",temp);	
			  OLED_ShowString(40,1,(unsigned char*)DisNum,12);
				
				sprintf(DisNum,"Distance = %.2f\r\n",temp);	
			  UART1_SendString(DisNum);
				
				OLED_ShowString(0,2,(unsigned char*)"MQ2 :         ",12);
			  sprintf(DisNum,"%.2f",PPM);	
			  OLED_ShowString(40,2,(unsigned char*)DisNum,12);
			  
				delay_ms(200);
	

		}
 }

