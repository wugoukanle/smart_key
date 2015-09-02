/**********************************************************************
*												smart key
*										 Module����ʱģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-018 15:39     1.0             build this file 
**********************************************************************/



#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

/*
*@���ܣ���ʼ���δ�ʱ������Ϊ��ʱ��ʱ��
*/
void SystemTick_Init(void) ;

void delay_us(uint32_t num);
void delay_ms(uint32_t num);

/*
*@ע�⣺����ʱ��׼
*/
void delay_s(uint64_t num);
#endif


