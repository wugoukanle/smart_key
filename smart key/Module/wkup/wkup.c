/**********************************************************************
*												smart key
*										 Module：低功耗模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-07-20 15:18     1.0             build this file 
*
* 模块功能：
*				低功耗有三种模式：睡眠，停机，待机
*				本模块使用停机模式
*
*
* 怎么关机和怎么唤醒
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
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR和BKP外设时钟    
			PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能

			//PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式 	
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); 			//在停机模式时，程序停在这个函数中
			//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); 			//在停机模式时，程序停在这个函数中
			
		
			//WFI表示wait for interrupt,WFE表示wait for event
			//如果发生中断唤醒，会先执行中断程序 ，再执行PWR_EnterSTOPMode的下1行代码
			
			//重新初始化
			SystemInit();
			
		  //SoftReset();
			
// 		 	if(Check_WKUP())
// 		 		break;
	
	
// 			if(longpress==0)
// 					break;
//  	}
	
			//重新初始化
			//SystemInit();	
	
}


/*
*@功能：系统进入停机模式
*/
void Sys_Enter_Standby(void)
{			 
	
	//相比Sys_Standby()，在进入停机模式之前，可以做些减小功耗的措施
	//这里做一些可以在停机模式减小功耗的措施
	
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	//RCC_APB2PeriphResetCmd(0X01FC,DISABLE);		//复位所有IO口
	
	
	
	
	
	Sys_Standby();
	
}



/*
*@功能：检测WKUP脚的信号
				返回值1:连续按下3s以上
							 0:错误的触发	
*@说明：以30ms为采样周期，每个30ms检测一次
*/      
uint8_t Check_WKUP(void) 
{
	uint8_t press_num=0;
	uint8_t loose_num=0;//记录松开的次数
	uint8_t wk;
	while(1)
	{
		wk=WKUP_KD;
		if(!wk)//已经按下了确认
		{
			press_num++;
			loose_num=0;
		}
		else 
		{
			loose_num++; //超过300ms内没有WKUP信号
			if(loose_num>3)  //没有长按的话最多运行120ms
				return 0;//错误的按键,按下次数不够；这里至关重要，如果3秒内检测时没有长按，则直接退出，不用到3秒
		}
		
		delay_ms(30);
		if(press_num>=100)//按下超过3秒钟
			return 1; //按下3s以上了
	}
}  




/*
*@功能：唤醒初始化
*/
void WKUP_Init(void)
{	
	uint8_t cw;
	cw=Check_WKUP();
	
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	//程序刚下进去时由于没有长按取消键，所以检测结果为0，直接进入停机模式
	//if(Check_WKUP()) 
	if(!cw) 		
		Sys_Standby();    //不是开机,进入停机模式  
	
}



void SoftReset(void) 
{ 
	__set_FAULTMASK(1);     // 关闭所有中断
  NVIC_SystemReset();			// 复位
}
