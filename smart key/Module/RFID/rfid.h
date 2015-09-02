/**********************************************************************
*												smart key
*										 Module��RFIDģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-21 18:07     1.0             build this file 
*
* ģ�鹦�ܣ�
*				ͨ������2��ȡ�жϵķ�ʽ��ȡRFID������
*				
*
*	����˵����
* -----------------------------------------------------------
*       GND   ��Դ��
*       VCC   3.3v��Դ
*       TX2   ��PA2
*       RX2   ��PA3             
* ----------------------------------------------------------
*				
* ���䣺	RFIDģ�������ֶ�ȡ��ʽ 1.�����ж϶˿ڶ�ȡά���������
																	2.���ô����ж�ģʽ��ȡ����
**********************************************************************/ 



#ifndef __RFID_H
#define __RFID_H




#include "usart.h"


#define  UARTRXLEN2	    10


typedef struct tg2UARTBUFF
{
  uint8_t   cFlag;
  uint16_t  cRHead; //Ҫ������ַ������
  uint16_t  cRTail; //��ǰ��������Ч����
  uint8_t   RBuff[UARTRXLEN2];//������
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
