/**********************************************************************
*												smart key
*										 Module：串口通信模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-21 18:07     1.0             build this file 
*
* 模块功能：
*				串口通信，通过转接板与PC通信，本身带有MAX232电平转化
*				使用串口1

*	连接说明：
* -----------------------------------------------------------
*       GND   电源地
*       VCC   接5V
*       TX1   接PA9
*       RX1   接PA10              
* ----------------------------------------------------------
*				
**********************************************************************/




#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>



#define UARTRXLEN1       254



//串口读程序需要用到
#define ERR_UART_OK     0x00   //正常
#define ERR_UART_EMPTY  0x01   //缓冲区空

#define  USB_CHEAK          (GPIOA->IDR  & GPIO_Pin_1)  //检测USB接口是否连接到计算机上,即PA1


/*
定义串口1的接收数据缓冲区
*/
typedef struct tg1UARTBUFF
{
  uint8_t   cFlag;
  uint16_t  cRHead; //要处理的字符的序号
  uint16_t  cRTail; //当前缓冲区有效长度
  uint8_t  RBuff[UARTRXLEN1];//缓冲区
} UARTBUFF1;


extern UARTBUFF1  m_UB1;

//串口读程序需要用到
extern uint8_t    m_Uart_State;



void Uart1_Init(void);
void USART1_Configuration(void);
int fputc(int ch,FILE *f);
int fgetc(FILE *f);


void UART1Reset(uint8_t uart);
void UART1SendChar(uint8_t uart, uint8_t data);

void UART1SendwordHex(uint8_t uart, uint32_t data);

void UART1nSendChar(uint8_t uart,uint8_t *str,uint8_t len);
void UART1Send(uint8_t uart,const uint8_t *str);
void UART1SendNewLine(uint8_t uart,const uint8_t *str);
uint8_t UART1ReadChar(uint8_t uart);
uint16_t UART1nReadChar(uint8_t uart, uint8_t *dst, uint16_t len);
uint8_t UART1ReadLine(uint8_t uart, uint8_t *pdst, uint8_t max);
uint8_t UART1ReadFrm(uint8_t uart, uint8_t *pdst, uint32_t max);
uint16_t UART1GetCount(uint8_t uart);



#endif
