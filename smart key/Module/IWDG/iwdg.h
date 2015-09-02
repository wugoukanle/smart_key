/**********************************************************************
*												smart key
*										 Module：独立看门狗定时器模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-28 11:07     1.0             build this file 
*
* 模块功能：
*				使用独立看门狗防止程序跑飞
*				
**********************************************************************/ 

#ifndef __IWDG_H
#define __IWDG_H		


#include "stm32f10x.h"


void iwdg_init(void);

void IWDG_Feed(void);

#endif  
