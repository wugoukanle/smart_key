/**********************************************************************
*												smart key
*										 Module������ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-21 18:15     1.0             build this file 
*
* ģ�鹦�ܣ�
*				����ģ��ͨ������3��STM32ͨ��
*				

*	����˵����
* -----------------------------------------------------------
*       GND   ��Դ��
*       VCC   ��3.3V
*       TX3   ��PB10
*       RX2   ��PB11
*				EN    ��PA11			ENΪ���ǹ㲥״̬��������
*				BRST  ��PA12			BRSTΪ�ͣ���ʾ����������ȥ����
* ----------------------------------------------------------
*				
**********************************************************************/




#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"
#include <stdio.h>


#define  UARTRXLEN3	    254


//���ڶ�������Ҫ�õ�
#define ERR_UART_OK     0x00   //����
#define ERR_UART_EMPTY  0x01   //��������

#define    SET_BT_EN	    	(GPIOA->BSRR  = GPIO_Pin_11)		//bluetooth��EN�ø�
#define    CLR_BT_EN				(GPIOA->BRR = GPIO_Pin_11)			//bluetooth��EN�õ�

#define    SET_BT_BRTS	    	(GPIOA->BSRR  = GPIO_Pin_12)  //bluetooth��BRTS�ø�
#define    CLR_BT_BRTS				(GPIOA->BRR = GPIO_Pin_12)    //bluetooth��BRTS�ø�



/*
���崮��3�Ľ������ݻ�����
*/
typedef struct tg3UARTBUFF
{
  uint8_t   cFlag;
  uint16_t  cRHead; //Ҫ������ַ������
  uint16_t  cRTail; //��ǰ��������Ч����
  uint8_t  RBuff[UARTRXLEN3];//������
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
