/**********************************************************************
*												smart key
*										 Module：延时模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-018 15:39     1.0             build this file 
*
* 模块功能：
*				利用滴答定时器，实现精确定时
*
**********************************************************************/






#include "delay.h"

#include "stm32f10x_rcc.h"




/*
*@实现：使用HCLK作为SysTick的时钟源，计数为零的中断不管，即未实现
*				滴答时钟的时基是1/72us
*/
void SystemTick_Init(void)        //1us中断一次
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);  //调用标准库函数，获取系统时钟
	SysTick_Config(rcc_clocks.HCLK_Frequency/1000000);//初始化并使能SysTick HCLK_Frequency=72MHZ
}

void delay_us(uint32_t num)
{
	uint32_t tnow=0,told=0,val=0,reload=0;
	reload=SysTick->LOAD;		//71
	told=SysTick->VAL;
	
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow<told)
			val+=told-tnow;
		else val+=reload-tnow+told;
		if((num*72)<=val)		//一个72就是1us，num个72就是num us
			break;
		told=tnow;
	}
}




void delay_ms(uint32_t num)
{
	uint32_t tnow=0,told=0,val=0,reload=0;
	reload=SysTick->LOAD;		//reload=71
	told=SysTick->VAL;			//SysTick->VAL的值是小于72的
	
	while(1)
	{
		tnow=SysTick->VAL;		
		if(tnow<told)
			val+=told-tnow;
		else val+=reload-tnow+told;
		
		//val是计数次数，每次的时间是1/72M
		
		if((num*72*1000)<=val)
			break;
		told=tnow;
	}	
}


/*
*@注意：秒延时不准
*/
void delay_s(uint64_t num)
{
	uint64_t tnow=0,told=0,val=0,reload=0;
	reload=SysTick->LOAD;		//reload=71
	told=SysTick->VAL;			//SysTick->VAL的值是小于72的
	
	while(1)
	{
		tnow=SysTick->VAL;		
		if(tnow<told)
			val+=told-tnow;
		else val+=reload-tnow+told;
		
		//val是计数次数，每次的时间是1/72M
		
		if((num*72*1000*1000)<=val)
			break;
		told=tnow;
	}	
}
