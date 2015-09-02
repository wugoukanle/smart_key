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


#include "iwdg.h"


/*
*@功能：初始化并启动看门狗
*@说明：T=（分频系数/4）*0.1*(重装载值)=3276ms
*				超时就复位，由硬件决定，没有中断直接复位
*/
void iwdg_init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		//关闭IWDG_PR和IWDG_RLR的写保护
	IWDG_SetReload(0xfff);													//设置重装载值为0xfff
	IWDG_SetPrescaler(IWDG_Prescaler_32);						//设置预分频系数为32
	IWDG_ReloadCounter();														//按照IWDG重装载寄存器的值重装载IWDG计数值即喂狗
	IWDG_Enable();																	//使能看门狗即启动看门狗

}


void IWDG_Feed(void)
{
	IWDG_ReloadCounter();														//按照IWDG重装载寄存器的值重装载IWDG计数值即喂狗
  //IWDG_KR=0xAAAA;																//reload,喂狗
}


