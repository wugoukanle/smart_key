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
	
  /*�ж����ȼ�����*/	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				//��ʾֻ����Ӧ���ȼ���û����ռ���ȼ�
	
  /*UART1�жϳ�ʼ������*/	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 			//�ж���������
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
	
}
