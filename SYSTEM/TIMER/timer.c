#include "timer.h"
#include "delay.h" 
extern vu8 Timer0_start;
extern u8 shijian;
extern u8 Times;
void servo_angle(int ang);
/*******************************************************************************
* 函数名  : Timer2_Init_Config
* 描述    : Timer2初始化配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 1s延时
*******************************************************************************/

/*******************************************************************************
* ? ? ?         : pwm_init
* ????		   : IO???TIM3?????	   
* ?    ?         : ?
* ?    ?         : ?
*******************************************************************************/
void pwm_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;   //?????????,?????GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//?????????,????????

	TIM_OCInitTypeDef TIM_OCInitStructure;//??TIM_OCInitStruct???????????TIMx

	/* ???? */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	/*  ??GPIO????IO? */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;// PC6
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//??????
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//TIM3??????
	TIM_TimeBaseInitStructure.TIM_Period = 199; //PWM ??=72000/(199+1)=36Khz//??????????????
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;//??????TIMx????????
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//??????:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM??????
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseInitStructure);

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//?????????	//pC6

	//PWM???	  //??TIM_OCInitStruct???????????TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM????
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;

	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	//????????TIM_OC1Init???TIM_OCInit,??????????????????
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//??????TIMx?CCR1????????
	TIM_Cmd(TIM3,ENABLE);//??????TIMx??
}

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//?????3??
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //??GPIO???AFIO????????
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//???GPIO
	
	
	
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3?????  TIM3_CH2->PB5    
 
   //????????????,??TIM3 CH2?PWM????	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//???GPIO
 
   //???TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //???????????????????????????
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx??????????? 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????
	
	//???TIM3 Channel2 PWM??	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //??T??????????TIM3 OC2
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //??TIM3?CCR2????????
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	TIM_Cmd(TIM3, ENABLE);  //??TIM3
	
}

void TIM3_IOConfig(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

		//??TIM3???1?GPIOA_Pin_6,??2?GPIOA_Pin_7;??3?GPIOB_Pin_0;
		//??TIM3???4?GPIOB_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		//??PWM????????AF_PP??
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		
}

//arr:????? psc:??????
//?????????:Tout=((arr+1)*(psc+1))/Ft   
void TIM3_PWM_Init1(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;//TIM??????
        TIM_OCInitTypeDef        TIM_OCInitStructure;//TIMPWM?
        TIM3_IOConfig();
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
        //??TIM3????;
        
        //?????
        TIM_DeInit(TIM3);
			TIM_TimeBaseInitStructure.TIM_Period = 199; //???????????????????????????
			TIM_TimeBaseInitStructure.TIM_Prescaler =7199; //??????TIMx??????????? 
			TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //??????:TDTS = Tck_tim
 			TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;	
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????

			//???TIM3 Channel2 PWM??	 
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
			TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //??T??????????TIM3 OC2
        //TIM_Prescaler:72,TIM_Period:20000???20ms
//        TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;                        //?????
//        TIM_TimeBaseInitStructure.TIM_Period = 199;                        //??????????0-65535
//        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);            //???TIM3      
//        
//        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//        TIM_OCInitStructure.TIM_Pulse = 1000;                                //???
//        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    
        
        TIM_OC1Init(TIM3,&TIM_OCInitStructure);
        TIM_OC2Init(TIM3,&TIM_OCInitStructure);
        TIM_OC3Init(TIM3,&TIM_OCInitStructure);
        TIM_OC4Init(TIM3,&TIM_OCInitStructure);

//        TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
//        TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
//        TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
//        TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
      
//        TIM_CtrlPWMOutputs(TIM3, ENABLE);
//        TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);
        //?????CC1,CC2,CC3,CC4
        TIM_Cmd(TIM3,ENABLE);
				servo_angle(0);
        //??TIM;
}
//500-1500us +     1500-2500  -    1500       
void duji1()
{
	TIM_SetCompare2(TIM3, 195);//90- 2.5    180 -5    45 -1.25
	delay_ms(500);
		TIM_SetCompare2(TIM3, 185);//90- 2.5    180 -5    45 -1.25
	delay_ms(500);
			TIM_SetCompare2(TIM3, 175);//90- 2.5    180 -5    45 -1.25
	delay_ms(500);
	TIM_SetCompare2(TIM3, 0);
//	TIM_SetCompare2(TIM3, 180);//175
//	delay_ms(1000);
//		TIM_SetCompare2(TIM3, 90);//1ms + 190   2ms -180
//	delay_ms(1000);
//			TIM_SetCompare2(TIM3, 0);//1ms + 190   2ms -180

//		TIM_SetCompare2(TIM3, 45);//175
//	delay_ms(1000);
//	TIM_SetCompare2(TIM3, 90);//175
//	delay_ms(1000);
////	TIM_SetCompare2(TIM3, 180);//1ms + 190   2ms -180
////	delay_ms(300);
//	TIM_SetCompare2(TIM3, 45);//175
//	delay_ms(1000);
//		TIM_SetCompare2(TIM3, 0);//175
//	delay_ms(1000);
//	


}


void duji2()
{
	TIM_SetCompare1(TIM3, 190);//1ms + 190   2ms -180
	delay_ms(300);
	TIM_SetCompare1(TIM3, 0);//175
	TIM_SetCompare1(TIM3, 180);//1ms + 190   2ms -180
	delay_ms(300);
	TIM_SetCompare1(TIM3, 0);//175
}

void servo_angle(int ang)
{
  uint16_t pulse;
  if(ang==0)
  {
	pulse=152;
  }
  else if(ang<0)
  {
	pulse=(-ang)*10+150;
  }
  else if(ang>0)
  {
	pulse=150-ang*10;
  } 
  TIM_SetCompare1(TIM3, pulse);
}

