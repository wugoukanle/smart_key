/**********************************************************************
*												smart key
*										 Module：中断模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-018 21:04     1.0             build this file 
*
* 模块功能：
*				中断分组，中断优先级设置
*
**********************************************************************/




#include "nvic.h"
#include "stm32f10x.h"


/*
*@功能:配置优先级组，外部中断
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;   //定义一个结构体
	
  /*中断优先级分组*/	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				//表示只有响应优先级，没有抢占优先级
	
  /*UART1中断初始化部分*/	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 			//中断向量类型
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
	
}
