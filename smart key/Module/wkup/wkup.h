/**********************************************************************
*												smart key
*										 Module���͹���ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-07-20 15:18     1.0             build this file 
*
* ģ�鹦�ܣ�
*				�͹���������ģʽ��˯�ߣ�ͣ��������
*				��ģ��ʹ��ͣ��ģʽ
*
**********************************************************************/ 




#ifndef __WKUP_H
#define __WKUP_H	 



#include <stm32f10x.h>


void Sys_Standby(void);
void Sys_Enter_Standby(void);
uint8_t Check_WKUP(void);
void EXTI0_IRQHandler(void);
void WKUP_Init(void);
void SoftReset(void);

#endif











