/**********************************************************************
*												smart key
*										 Module������ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-29 19:03     1.0             build this file 
*
* ģ�鹦�ܣ�
*				����
*				
**********************************************************************/ 



#ifndef __SWKEY_H
#define __SWKEY_H

#include "stm32f10x.h"


#define  KEY1	     0X01
#define  KEY2		   0X02
#define  KEY3		   0X03					
#define  KEY4		   0X04				 
#define  KEYNO		 0XFF				 //
#define  KEYINIT	 0XFE				 //




#define  KEYUP         KEY1		 //����ȷ��
#define  KEYCLEAR      KEY2		 //��������
#define  KEYDOWN       KEY3		 //��������
#define  KEYSURE       KEY4	   //����ȡ��





#define GPIO_PIN_KEY1_BUTTON               GPIO_Pin_5
#define GPIO_PIN_KEY2_BUTTON               GPIO_Pin_13
#define GPIO_PIN_KEY3_BUTTON               GPIO_Pin_8
#define GPIO_PIN_KEY4_BUTTON               GPIO_Pin_15


#define EXTI_LINE_KEY1_BUTTON              EXTI_Line5
#define GPIO_PORT_SOURCE_KEY1_BUTTON       GPIO_PortSourceGPIOB
#define GPIO_PIN_SOURCE_KEY1_BUTTON        GPIO_PinSource5

#define EXTI_LINE_KEY2_BUTTON              EXTI_Line13
#define GPIO_PORT_SOURCE_KEY2_BUTTON       GPIO_PortSourceGPIOC
#define GPIO_PIN_SOURCE_KEY2_BUTTON        GPIO_PinSource13

#define EXTI_LINE_KEY3_BUTTON              EXTI_Line8
#define GPIO_PORT_SOURCE_KEY3_BUTTON       GPIO_PortSourceGPIOB
#define GPIO_PIN_SOURCE_KEY3_BUTTON        GPIO_PinSource8

#define EXTI_LINE_KEY4_BUTTON              EXTI_Line15
#define GPIO_PORT_SOURCE_KEY4_BUTTON       GPIO_PortSourceGPIOA
#define GPIO_PIN_SOURCE_KEY4_BUTTON        GPIO_PinSource15


extern uint8_t Key_Num;


void Init_KeyNumuber(void);
void Init_Sw_Key_Io(void);
void Exit_Line_Init(void);
uint8_t GetKey(void);
void InsertKey(uint8_t nChar);
void ResetKey(void);
void Scan_Key(void);


#endif
