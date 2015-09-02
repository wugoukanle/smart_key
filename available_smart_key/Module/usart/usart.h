/**********************************************************************
*												smart key
*										 Module������ͨ��ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-21 18:07     1.0             build this file 
*
* ģ�鹦�ܣ�
*				����ͨ�ţ�ͨ��ת�Ӱ���PCͨ�ţ��������MAX232��ƽת��
*				ʹ�ô���1

*	����˵����
* -----------------------------------------------------------
*       GND   ��Դ��
*       VCC   ��5V
*       TX1   ��PA9
*       RX1   ��PA10              
* ----------------------------------------------------------
*				
**********************************************************************/




#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>



#define UARTRXLEN1       254



//���ڶ�������Ҫ�õ�
#define ERR_UART_OK     0x00   //����
#define ERR_UART_EMPTY  0x01   //��������

#define  USB_CHEAK          (GPIOA->IDR  & GPIO_Pin_1)  //���USB�ӿ��Ƿ����ӵ��������,��PA1


/*
���崮��1�Ľ������ݻ�����
*/
typedef struct tg1UARTBUFF
{
  uint8_t   cFlag;
  uint16_t  cRHead; //Ҫ������ַ������
  uint16_t  cRTail; //��ǰ��������Ч����
  uint8_t  RBuff[UARTRXLEN1];//������
} UARTBUFF1;


extern UARTBUFF1  m_UB1;

//���ڶ�������Ҫ�õ�
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
