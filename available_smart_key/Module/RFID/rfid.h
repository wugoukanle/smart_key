/**********************************************************************
*												smart key
*										 Module：RFID模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-21 18:07     1.0             build this file 
*
* 模块功能：
*				通过串口2采取中断的方式读取RFID的数据
*				
*
*	连接说明：
* -----------------------------------------------------------
*       GND   电源地
*       VCC   3.3v电源
*       TX2   接PA2
*       RX2   接PA3             
* ----------------------------------------------------------
*				
* 补充：	RFID模块有两种读取方式 1.采用中断端口读取维根码的数据
																	2.采用串口中断模式读取数据
**********************************************************************/ 



#ifndef __RFID_H
#define __RFID_H




#include "usart.h"


#define  UARTRXLEN2	    10


typedef struct tg2UARTBUFF
{
  uint8_t   cFlag;
  uint16_t  cRHead; //要处理的字符的序号
  uint16_t  cRTail; //当前缓冲区有效长度
  uint8_t   RBuff[UARTRXLEN2];//缓冲区
} UARTBUFF2;


extern UARTBUFF1  m_UB2;
extern uint8_t Lock_number[4];



void Rfid_IO_Init_In(void);
void Uart2_Init(void);
void USART2_Configuration(void);
void UART2SendChar(uint8_t uart,uint8_t data);
void UART2nSendChar(uint8_t uart,uint8_t *str,uint8_t len);
uint8_t Read_RfidData(uint8_t uart, uint8_t *pdst, uint8_t max);
void UART2Reset(uint8_t uart);


void Key_Trismit(void);
uint8_t check_lock_code(void);
void save_rfid_result(void);


void buzzer_unlock_success(void);
void buzzer_unlock_fail(void);

#endif
