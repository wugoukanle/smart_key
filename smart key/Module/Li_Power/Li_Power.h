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

#ifndef __LI_POWER_H
#define __LI_POWER_H

#include "stm32f10x.h"




//ѡȡ��ѹΪ7.2V��ʱ����Ϊ�ǵ͵�ƽ

//#define  BATTERY_VALUE        0XA15    //��ؼ�����

//#define  BATTERY_VALUE        0xA00

//#define  BATTERY_VALUE          0x57a      //����﮵�أ���Ϊ�ĵ�ѹֵ


#define  BATTERY_VALUE          0x788      //����﮵�أ���Ϊ�ĵ�ѹֵ
#define  SAMPLE_NUM             0x01       //ADC��ص�������������С��20





void ADC_GPIO_Configuration(void);
void ADC_Configuration(void);
uint8_t Power_Cheak(void);
void OLED_Show_LowBattry(void);


#endif
