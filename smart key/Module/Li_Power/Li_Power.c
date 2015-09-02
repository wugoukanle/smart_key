/**********************************************************************
*												smart key
*										 Module����ص������ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-24 10:30     1.0             build this file 
*
* ģ�鹦�ܣ�
*				��ص�����⣬ͨ��ADC����ѹ�����Ի���ص�ѹ������Ĺ�ϵ
*				
* ����˵����
*				PA0��ADC�������
*
**********************************************************************/ 




#include "Li_Power.h"
#include "stm32f10x_adc.h"
#include "iwdg.h"
#include "oled.h"
#include "Globle_var.h" 




/*****************************************************************************
* Function Name  : ADC_GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void ADC_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA,ENABLE);
	
  /* Configure PB.00 (ADC Channel8) as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/*
*@���ܣ�﮵�ؼ����ADC1
*/
void ADC_Configuration(void)
{
  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//����ADCģʽ
  ADC_InitStructure.ADC_ScanConvMode =ENABLE ;	//DISABLE			  //��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//��������ת��ģʽ������ͣ�ؽ��� ADC ת��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//��ʹ���ⲿ����ת��
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//�ɼ������Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 1;												//Ҫת����ͨ����Ŀ 1
  ADC_Init(ADC1, &ADC_InitStructure);	/* ADC1 regular channel1 configuration */ 
	
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
	
  /* Configure high and low analog watchdog thresholds */
  ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0B00, 0x0300);
  /* Configure channel8 as the single analog watchdog guarded channel */
  ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_0);
  /* Enable analog watchdog on one regular channel */
  ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
 /* Enable AWD interupt */
 // ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);		//��ʹ��ģ�⿴�Ź�����

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}




// uint8_t Power_Cheak(void)
// {
//   uint8_t state=0;
//   uint8_t i =0;
// 	
// 	
//   battery[batt_inc] =ADC_GetConversionValue(ADC1);//Returns the last ADCx conversion result data for regular channel.
//   batt_inc++;
// 	
// 	
//   if(batt_inc >=20)					//ƽ�����20��
//   {
//      IWDG_Feed();
// 	   batt_inc =0;
// 	   for(batt_inc=0;batt_inc<20;batt_inc++)
// 	   {
// 	      if(battery[batt_inc] < BATTERY_VALUE)			//���ڵ�صĵ͵�ѹֵ
// 	      {
// 	        i++;
// 	      }
//      }
// 		 
// 	   if(i >=15)						//���20������15�ε�ص�ѹ���� ��صĵ͵�ѹֵ
// 	   {
// 	    IWDG_Feed();
// 			state =1;
// 			return state;//�͵�ѹ
// 	   }
// 		 else if(i<15 && i>10)
// 		 {
// 			 IWDG_Feed();
// 			 state =2;
// 			 return state;
//      }
// 		 else if(i<10 && i>5)
// 		 {
// 			 IWDG_Feed();
// 			 state =3;return state;
//      }
// 	  
//    }
// 	 
//    IWDG_Feed();
// 	 return state;  
// }




/*
���ñ��һ�µ�ط�����ĵ�ѹV1�ͳ������ĵ�ѹV2����ؾ���ѹ�����ADC���ɲ�õĵ�ѹ�����ص�ѹV3���������ѹȥ��V1��V2����ӳ�䡣
����=(V3-V1)/(V2-V1)*100%;

��� 4.2vʱADCֵΪ2530d
		 3.2vʱADCֵΪ1930d

��2530-1928��/4~=150

bp>2455ʱ������

2305<bp<2455ʱ����

2155<bp<2305ʱ����

2005<bp<2155ʱ������

bp<1930ʱ��ѹ�ػ�



����ֵֻ������0,1,2,3,4,5
*/
uint8_t Power_Cheak(void)
{
  uint8_t state=5;
  uint8_t i =0,n=0;
	uint32_t battery_mean=0;
	
  battery[batt_inc]=ADC_GetConversionValue(ADC1);			//Returns the last ADCx conversion result data for regular channel.
  batt_inc++;
	
	
  if(batt_inc >=SAMPLE_NUM-1)					//ƽ�����20��
  {
     IWDG_Feed();
	   batt_inc =0;
		
		 for(n=0;n<SAMPLE_NUM;n++)
		 {
				battery_mean=battery[n]+battery_mean;
		 }
		 battery_mean=battery_mean/SAMPLE_NUM;		//��20�β�����ƽ��ֵ
		
		 
		 
	   if(battery_mean>=2455)						//���20������15�ε�ص�ѹ���� ��صĵ͵�ѹֵ
	   {
	    IWDG_Feed();
			state =0;
			return state;//������
	   }
		 else if(battery_mean<2455 && battery_mean>2305)
		 {
			 IWDG_Feed();
			 state =1;
			 return state;
     }
		 else if(battery_mean<2305 && battery_mean>2155)
		 {
			 IWDG_Feed();
			 state =2;
			 return state;
     }
		 else if(battery_mean<2155 && battery_mean>1930)
		 {
			 IWDG_Feed();
			 state =3;
			 return state;//�͵���
     }	  
		 else if(battery_mean<1930) //��ѹ����
		 {
			 IWDG_Feed();
			 state =4;
			 return state;
		 }
		 
		 
   }
	 
   IWDG_Feed();
	 return state;  
}



void OLED_Show_LowBattry(void)
{
	
		OLED_Clear();
		OLED_ShowUnite(32,2,"��ѹ����");
		OLED_ShowUnite(24,4,"���Զ��ػ�");
	

}



