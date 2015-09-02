/**********************************************************************
*												smart key
*										 Module���������Ź���ʱ��ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-28 11:07     1.0             build this file 
*
* ģ�鹦�ܣ�
*				ʹ�ö������Ź���ֹ�����ܷ�
*				
**********************************************************************/ 


#include "iwdg.h"


/*
*@���ܣ���ʼ�����������Ź�
*@˵����T=����Ƶϵ��/4��*0.1*(��װ��ֵ)=3276ms
*				��ʱ�͸�λ����Ӳ��������û���ж�ֱ�Ӹ�λ
*/
void iwdg_init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		//�ر�IWDG_PR��IWDG_RLR��д����
	IWDG_SetReload(0xfff);													//������װ��ֵΪ0xfff
	IWDG_SetPrescaler(IWDG_Prescaler_32);						//����Ԥ��Ƶϵ��Ϊ32
	IWDG_ReloadCounter();														//����IWDG��װ�ؼĴ�����ֵ��װ��IWDG����ֵ��ι��
	IWDG_Enable();																	//ʹ�ܿ��Ź����������Ź�

}


void IWDG_Feed(void)
{
	IWDG_ReloadCounter();														//����IWDG��װ�ؼĴ�����ֵ��װ��IWDG����ֵ��ι��
  //IWDG_KR=0xAAAA;																//reload,ι��
}


