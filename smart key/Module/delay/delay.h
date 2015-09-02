/**********************************************************************
*												smart key
*										 Module：延时模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-018 15:39     1.0             build this file 
**********************************************************************/



#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

/*
*@功能：初始化滴答定时器，作为延时的时基
*/
void SystemTick_Init(void) ;

void delay_us(uint32_t num);
void delay_ms(uint32_t num);

/*
*@注意：秒延时不准
*/
void delay_s(uint64_t num);
#endif


