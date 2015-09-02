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
	
	/*
	此系统用到的中断：
			串口1中断，即USB
			串口2中断，即RFID
			串口3中断，即bluetooth
	
			按键中断,即外部中断线中断
					EXTI9_5_IRQn   		S1=PB5 ； S3=PB8
					EXTI15_10_IRQn		S2=PC13； S4=PA15
	
			定时器TIM2中断
	*/
	
	
  /*中断优先级分组*/  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 				//表示只有响应优先级，没有抢占优先级
	
	
	
	
  /*UART1中断初始化部分,即USB*/	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 			//中断向量类型
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
	
  /*UART2中断初始化部分，即RFID*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/*UART3中断初始化部分，即bluetooth*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
	
	
	/*TIM2中断初始化部分*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	
	
  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  /* Enable the EXTI15_10 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
}
