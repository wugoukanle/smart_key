/**********************************************************************
*												smart key
*										 Module：电池电量检测模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-24 10:30     1.0             build this file 
*
* 模块功能：
*				电池电量检测，通过ADC检测电压，线性化电池电压与电量的关系
*				
* 接线说明：
*				PA0做ADC的输入口
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
*@功能：锂电池检测是ADC1
*/
void ADC_Configuration(void)
{
  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//独立ADC模式
  ADC_InitStructure.ADC_ScanConvMode =ENABLE ;	//DISABLE			  //禁止扫描模式，扫描模式用于多通道采集
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//开启连续转换模式，即不停地进行 ADC 转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//不使用外部触发转换
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//采集数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;												//要转换的通道数目 1
  ADC_Init(ADC1, &ADC_InitStructure);	/* ADC1 regular channel1 configuration */ 
	
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
	
  /* Configure high and low analog watchdog thresholds */
  ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0B00, 0x0300);
  /* Configure channel8 as the single analog watchdog guarded channel */
  ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_0);
  /* Enable analog watchdog on one regular channel */
  ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
 /* Enable AWD interupt */
 // ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);		//不使用模拟看门狗功能

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
//   if(batt_inc >=20)					//平均检测20次
//   {
//      IWDG_Feed();
// 	   batt_inc =0;
// 	   for(batt_inc=0;batt_inc<20;batt_inc++)
// 	   {
// 	      if(battery[batt_inc] < BATTERY_VALUE)			//低于电池的低电压值
// 	      {
// 	        i++;
// 	      }
//      }
// 		 
// 	   if(i >=15)						//如果20次中有15次电池电压低于 电池的低电压值
// 	   {
// 	    IWDG_Feed();
// 			state =1;
// 			return state;//低电压
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
万用表测一下电池放完电后的电压V1和充满电后的电压V2，电池经分压电阻进ADC，由测得的电压算出电池电压V3，由这个电压去与V1、V2线性映射。
电量=(V3-V1)/(V2-V1)*100%;

测得 4.2v时ADC值为2530d
		 3.2v时ADC值为1930d

（2530-1928）/4~=150

bp>2455时电量满

2305<bp<2455时电量

2155<bp<2305时电量

2005<bp<2155时电量低

bp<1930时低压关机



返回值只可能是0,1,2,3,4,5
*/
uint8_t Power_Cheak(void)
{
  uint8_t state=5;
  uint8_t i =0,n=0;
	uint32_t battery_mean=0;
	
  battery[batt_inc]=ADC_GetConversionValue(ADC1);			//Returns the last ADCx conversion result data for regular channel.
  batt_inc++;
	
	
  if(batt_inc >=SAMPLE_NUM-1)					//平均检测20次
  {
     IWDG_Feed();
	   batt_inc =0;
		
		 for(n=0;n<SAMPLE_NUM;n++)
		 {
				battery_mean=battery[n]+battery_mean;
		 }
		 battery_mean=battery_mean/SAMPLE_NUM;		//求20次采样的平均值
		
		 
		 
	   if(battery_mean>=2455)						//如果20次中有15次电池电压低于 电池的低电压值
	   {
	    IWDG_Feed();
			state =0;
			return state;//满电量
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
			 return state;//低电量
     }	  
		 else if(battery_mean<1930) //电压过低
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
		OLED_ShowUnite(32,2,"电压过低");
		OLED_ShowUnite(24,4,"将自动关机");
	

}



