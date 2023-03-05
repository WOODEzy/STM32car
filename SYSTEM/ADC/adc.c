 #include "adc.h"
 #include "stm32f10x_adc.h"
 #include "math.h"
 
 
void Adc_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  
 
                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}


void  Adc_Config(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	Adc_GPIO_Config();	
  /*72M/6=12,ADC????????14M*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	/*??? ADC1 ????????????*/
	ADC_DeInit(ADC1); 
  /*ADC????:ADC1?ADC2???????*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	/*????????????*/
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	/*?????????????*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  /*ADC??????????????*/	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	/*ADC?????*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	/*?????????ADC?????*/
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	/*??ADC_InitStruct???????????ADCx????*/   
	ADC_Init(ADC1, &ADC_InitStructure);	
 
  /*?????ADC1*/
	ADC_Cmd(ADC1, ENABLE);	
	/*?????ADC1??????*/
	ADC_ResetCalibration(ADC1);	
	/*??ADC1??????????,???????*/
	while(ADC_GetResetCalibrationStatus(ADC1));
	/*????ADC1???*/
	ADC_StartCalibration(ADC1);		
  /*????ADC1?????,???????*/
	while(ADC_GetCalibrationStatus(ADC1));		
  /*?????ADC1?????????*/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
}	


#define  RL   5                               //10000.00   //??????

float AD1_value()
{
	float	temp = 0;
	char i = 0;
	for(i = 0;i<30;i++)
	{
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );	  			    
  
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE);			
	
	 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	
	 temp += ADC_GetConversionValue(ADC1);			
	}
	temp = temp/30;
	temp = temp*3.3/4096;
	
  return temp;
}

/********************************************
 * 1.651428	          200               *
 * 1.437143	          300               *
 * 1.257143	          400               *
 * 1.137143	          500               *
 * 1		          600               *
 * 0.928704	          700               *
 * 0.871296	          800               *
 * 0.816667	          900               *
 * 0.785714	          1000              *
 * 0.574393	          2000              *
 * 0.466047	          3000              *
 * 0.415581	          4000              *
 * 0.370478	          5000              *
 * 0.337031	          6000              *
 * 0.305119	          7000              *
 * 0.288169	          8000              *
 * 0.272727	          9000              *
 * 0.254795	          10000             *
 *                                      *
 * ppm = 613.9f * pow(RS/RL, -2.074f)   *
 ***************************************/





/*
1.STM32F103ZET6  ADC 12? 4095
2.????3300mV
3.ADC?? = (3300/4095)*ADC?
4.ADC?? = (0.8058608058608059)*ADC?
5.MQ_2??/RL= (5000-ADC??)/ADC??
6.MQ_2?? = 5000-ADC??)/ADC?? × RL


float Vrl = 3.3f * ADC_Value / 4095.f;
    float RS = (3.3f - Vrl) / Vrl * RL; 


*/
float GetMQ_2Res(void)
{
  int i;
	float res;
	float temp;
	float voltage;

	//??30?????
	temp = 0;
	for(i = 0;i<30;i++)
	{
	
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );	  			    
  
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE);			
	
	 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	
	 temp += ADC_GetConversionValue(ADC1);			
	}
	temp = temp/30;
	
	 //voltage = (0.8058608058608059)*temp;
	
	// temp = (5000.00-voltage)/voltage*RL;
	
	voltage = 3.3f * temp / 4095.f;
    temp = (3.3f - voltage) / voltage * RL; 
	
	
	 res = temp;
	 
	 return(res);	
}

#define CAL_PPM 20   //?????????? PPM?



float MQ2_PPM_Calibration(float RS_1)
{
		float R0_1; 
	
    R0_1 = RS_1 / pow(CAL_PPM / 613.9f, 1 / -2.074f);
	  return(R0_1);
}






















