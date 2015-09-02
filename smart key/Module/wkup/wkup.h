/**********************************************************************
*												smart key
*										 Module：低功耗模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-07-20 15:18     1.0             build this file 
*
* 模块功能：
*				低功耗有三种模式：睡眠，停机，待机
*				本模块使用停机模式
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











