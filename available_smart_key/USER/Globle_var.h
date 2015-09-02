/**********************************************************************
*												smart key
*										 Module：全局变量模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-22 20:01     1.0             build this file 
*
* 模块功能：
*				全局变量声明,及全局宏定义
*						
**********************************************************************/


#ifndef __GLOBLE_VAR_H
#define __GLOBLE_VAR_H

#include "stm32f10x.h"


#include "stdint.h"

#define  ST_IDLE          0XFF
#define  ST_RTC           0XFE
#define  ST_USB	          0XFD
#define  ST_KEY	          0XFC
#define  ST_DEBUG         0xFB
#define  ST_LCDMENU       0XFA
#define  TIMER_TIMEOUT	  0x01


#define  BLUETOOTH_ON				(GPIOB->BSRR = GPIO_Pin_0)
#define  BLUETOOTH_OFF		  (GPIOB->BRR  = GPIO_Pin_0)

#define  MAX232_ON	        (GPIOB->BSRR = GPIO_Pin_6)		
#define  MAX232_OFF		      (GPIOB->BRR  = GPIO_Pin_6)

#define  OLED_ON	          (GPIOB->BSRR = GPIO_Pin_7)		
#define  OLED_OFF		        (GPIOB->BRR  = GPIO_Pin_7)

#define  BUZZER_ON					(GPIOA->BSRR = GPIO_Pin_8)
#define  BUZZER_OFF					(GPIOA->BRR  = GPIO_Pin_8)

#define  COIL_ACTUATE				(GPIOB->BSRR = GPIO_Pin_1)
#define  COIL_NO_ACTUATE		(GPIOB->BRR  = GPIO_Pin_1)


// #define  COIL_ACTUATE				(GPIOB->BSRR = GPIO_Pin_6)
// #define  COIL_NO_ACTUATE		(GPIOB->BRR  = GPIO_Pin_6)





#define  MAX_BUFF      520     //255
#define  MAX_TI        246

#define MAX_MENU       10




extern uint8_t time_buff[6];

extern uint8_t end_flag;

extern uint8_t USB_Cheak_init_flag;

extern uint8_t unlock_result;		//0表示初始状态，1表示授权，2表示未授权

extern uint8_t shutdown_flag;;//关机标志

extern uint8_t key_code;		//要钥匙编号



extern uint32_t author_addr,author_row,row_num,temp_addr;
extern uint8_t author_data[24];

extern uint32_t caima_addr,caima_row,row_num2,temp_addr2;
extern uint8_t caima_data[8];

extern uint32_t record_addr,record_row,row_num3,temp_addr3;
extern uint8_t record_data[5];

extern uint8_t g_State;

extern uint8_t guanjiflag;

extern uint8_t f_CurSel;


extern uint8_t Rfid_Buff[6]; 

extern uint8_t g_Buff[MAX_BUFF];

extern uint8_t mode;

extern uint8_t bluetooth_uart_switch;

extern uint8_t mac_query_result;


extern uint8_t item[25];

extern uint32_t row;

extern uint32_t point;






#endif
