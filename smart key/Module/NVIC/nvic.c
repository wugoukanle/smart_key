/**********************************************************************
*												smart key
*										 Module���ж�ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-018 21:04     1.0             build this file 
*
* ģ�鹦�ܣ�
*				�жϷ��飬�ж����ȼ�����
*
**********************************************************************/




#include "nvic.h"
#include "stm32f10x.h"


/*
*@����:�������ȼ��飬�ⲿ�ж�
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;   //����һ���ṹ��
	
	/*
	��ϵͳ�õ����жϣ�
			����1�жϣ���USB
			����2�жϣ���RFID
			����3�жϣ���bluetooth
	
			�����ж�,���ⲿ�ж����ж�
					EXTI9_5_IRQn   		S1=PB5 �� S3=PB8
					EXTI15_10_IRQn		S2=PC13�� S4=PA15
	
			��ʱ��TIM2�ж�
	*/
	
	
  /*�ж����ȼ�����*/  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 				//��ʾֻ����Ӧ���ȼ���û����ռ���ȼ�
	
	
	
	
  /*UART1�жϳ�ʼ������,��USB*/	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 			//�ж���������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
	
  /*UART2�жϳ�ʼ�����֣���RFID*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/*UART3�жϳ�ʼ�����֣���bluetooth*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
	
	
	/*TIM2�жϳ�ʼ������*/
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
