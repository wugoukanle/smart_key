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

#ifndef __LI_POWER_H
#define __LI_POWER_H

#include "stm32f10x.h"




//选取电压为7.2V的时候，认为是低电平

//#define  BATTERY_VALUE        0XA15    //电池检测界限

//#define  BATTERY_VALUE        0xA00

//#define  BATTERY_VALUE          0x57a      //单节锂电池，认为的低压值


#define  BATTERY_VALUE          0x788      //单节锂电池，认为的低压值
#define  SAMPLE_NUM             0x01       //ADC电池电量次数，必须小于20





void ADC_GPIO_Configuration(void);
void ADC_Configuration(void);
uint8_t Power_Cheak(void);
void OLED_Show_LowBattry(void);


#endif
