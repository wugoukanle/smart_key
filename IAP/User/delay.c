/**********************************************************************
*												smart key
*										 Module����ʱģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-018 15:39     1.0             build this file 
*
* ģ�鹦�ܣ�
*				���õδ�ʱ����ʵ�־�ȷ��ʱ
*
**********************************************************************/






#include "delay.h"

#include "stm32f10x_rcc.h"




/*
*@ʵ�֣�ʹ��HCLK��ΪSysTick��ʱ��Դ������Ϊ����жϲ��ܣ���δʵ��
*				�δ�ʱ�ӵ�ʱ����1/72us
*/
void SystemTick_Init(void)        //1us�ж�һ��
{
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);  //���ñ�׼�⺯������ȡϵͳʱ��
	SysTick_Config(rcc_clocks.HCLK_Frequency/1000000);//��ʼ����ʹ��SysTick HCLK_Frequency=72MHZ
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
		if((num*72)<=val)		//һ��72����1us��num��72����num us
			break;
		told=tnow;
	}
}




void delay_ms(uint32_t num)
{
	uint32_t tnow=0,told=0,val=0,reload=0;
	reload=SysTick->LOAD;		//reload=71
	told=SysTick->VAL;			//SysTick->VAL��ֵ��С��72��
	
	while(1)
	{
		tnow=SysTick->VAL;		
		if(tnow<told)
			val+=told-tnow;
		else val+=reload-tnow+told;
		
		//val�Ǽ���������ÿ�ε�ʱ����1/72M
		
		if((num*72*1000)<=val)
			break;
		told=tnow;
	}	
}


/*
*@ע�⣺����ʱ��׼
*/
void delay_s(uint64_t num)
{
	uint64_t tnow=0,told=0,val=0,reload=0;
	reload=SysTick->LOAD;		//reload=71
	told=SysTick->VAL;			//SysTick->VAL��ֵ��С��72��
	
	while(1)
	{
		tnow=SysTick->VAL;		
		if(tnow<told)
			val+=told-tnow;
		else val+=reload-tnow+told;
		
		//val�Ǽ���������ÿ�ε�ʱ����1/72M
		
		if((num*72*1000*1000)<=val)
			break;
		told=tnow;
	}	
}
