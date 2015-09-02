/**********************************************************************
*												smart key
*										 Module：蓝牙模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-21 18:15     1.0             build this file 
*
* 模块功能：
*				蓝牙模块通过串口3与STM32通信
*				

*	连接说明：
* -----------------------------------------------------------
*       GND   电源地
*       VCC   接3.3V
*       TX3   接PB10
*       RX2   接PB11
*				EN    接PA11			EN为低是广播状态，高休眠
*				BRST  接PA12			BRST为低，表示主机有数据去发送
* ----------------------------------------------------------
*				
**********************************************************************/




#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"
#include <stdio.h>


#define  UARTRXLEN3	    254


//串口读程序需要用到
#define ERR_UART_OK     0x00   //正常
#define ERR_UART_EMPTY  0x01   //缓冲区空

#define    SET_BT_EN	    	(GPIOA->BSRR  = GPIO_Pin_11)		//bluetooth的EN置高
#define    CLR_BT_EN				(GPIOA->BRR = GPIO_Pin_11)			//bluetooth的EN置低

#define    SET_BT_BRTS	    	(GPIOA->BSRR  = GPIO_Pin_12)  //bluetooth的BRTS置高
#define    CLR_BT_BRTS				(GPIOA->BRR = GPIO_Pin_12)    //bluetooth的BRTS置高



/*
定义串口3的接收数据缓冲区
*/
typedef struct tg3UARTBUFF
{
  uint8_t   cFlag;
  uint16_t  cRHead; //要处理的字符的序号
  uint16_t  cRTail; //当前缓冲区有效长度
  uint8_t  RBuff[UARTRXLEN3];//缓冲区
} UARTBUFF3;



extern UARTBUFF3  m_UB3;
extern uint8_t    m_Uart_State3;




void Uart3_Init(void);
void USART3_Configuration(void);
void bluetooth_Configuration(void);

// int fputc(int ch,FILE *f);
// int fgetc(FILE *f);

void UART3Reset(uint8_t uart);

void UART3SendChar(uint8_t uart, uint8_t data);

void UART3nSendChar(uint8_t uart,uint8_t *str,uint8_t len);

uint8_t UART3ReadChar(uint8_t uart);

uint16_t UART3nReadChar(uint8_t uart, uint8_t *dst, uint16_t len);

uint8_t UART3ReadFrm(uint8_t uart, uint8_t *pdst, uint32_t max);

uint16_t UART3GetCount(uint8_t uart);

uint8_t bluetooth_mac(void);
#endif
