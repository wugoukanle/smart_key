/**********************************************************************
*												smart key
*										 Module���͹���ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-07-20 15:18     1.0             build this file 
*
* ģ�鹦�ܣ�
*				�͹���������ģʽ��˯�ߣ�ͣ��������
*				��ģ��ʹ��ͣ��ģʽ
*
*
* ��ô�ػ�����ô����
*
*
**********************************************************************/ 



#include "wkup.h"
#include "sw_key.h"
#include "delay.h"
#include "oled.h"
#include "Globle_var.h"
#include "bluetooth.h"
#include "spi_flash.h"

void Sys_Standby(void)
{
	stop_flag=1;
	  
// 	while(1)
// 	{
			NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,ENABLE);	   
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR��BKP����ʱ��    
			PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���

			//PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ 	
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); 			//��ͣ��ģʽʱ������ͣ�����������
			//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); 			//��ͣ��ģʽʱ������ͣ�����������
			
		
			//WFI��ʾwait for interrupt,WFE��ʾwait for event
			//��������жϻ��ѣ�����ִ���жϳ��� ����ִ��PWR_EnterSTOPMode����1�д���
			
			//���³�ʼ��
			SystemInit();
			
		  //SoftReset();
			
// 		 	if(Check_WKUP())
// 		 		break;
	
	
// 			if(longpress==0)
// 					break;
//  	}
	
			//���³�ʼ��
			//SystemInit();	
	
}


/*
*@���ܣ�ϵͳ����ͣ��ģʽ
*/
void Sys_Enter_Standby(void)
{			 
	
	//���Sys_Standby()���ڽ���ͣ��ģʽ֮ǰ��������Щ��С���ĵĴ�ʩ
	//������һЩ������ͣ��ģʽ��С���ĵĴ�ʩ
	
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	//RCC_APB2PeriphResetCmd(0X01FC,DISABLE);		//��λ����IO��
	
	
	
	
	
	Sys_Standby();
	
}



/*
*@���ܣ����WKUP�ŵ��ź�
				����ֵ1:��������3s����
							 0:����Ĵ���	
*@˵������30msΪ�������ڣ�ÿ��30ms���һ��
*/      
uint8_t Check_WKUP(void) 
{
	uint8_t press_num=0;
	uint8_t loose_num=0;//��¼�ɿ��Ĵ���
	uint8_t wk;
	while(1)
	{
		wk=WKUP_KD;
		if(!wk)//�Ѿ�������ȷ��
		{
			press_num++;
			loose_num=0;
		}
		else 
		{
			loose_num++; //����300ms��û��WKUP�ź�
			if(loose_num>3)  //û�г����Ļ��������120ms
				return 0;//����İ���,���´�������������������Ҫ�����3���ڼ��ʱû�г�������ֱ���˳������õ�3��
		}
		
		delay_ms(30);
		if(press_num>=100)//���³���3����
			return 1; //����3s������
	}
}  




/*
*@���ܣ����ѳ�ʼ��
*/
void WKUP_Init(void)
{	
	uint8_t cw;
	cw=Check_WKUP();
	
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	//������½�ȥʱ����û�г���ȡ���������Լ����Ϊ0��ֱ�ӽ���ͣ��ģʽ
	//if(Check_WKUP()) 
	if(!cw) 		
		Sys_Standby();    //���ǿ���,����ͣ��ģʽ  
	
}



void SoftReset(void) 
{ 
	__set_FAULTMASK(1);     // �ر������ж�
  NVIC_SystemReset();			// ��λ
}
