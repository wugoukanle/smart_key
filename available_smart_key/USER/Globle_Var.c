/**********************************************************************
*												smart key
*										 Module��ȫ�ֱ���ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-22 20:01     1.0             build this file 
*
* ģ�鹦�ܣ�
*				ȫ�ֱ�������
*						
**********************************************************************/

#include "Globle_Var.h"


//�洢ʵʱʱ�����飬�ֱ���������ʱ����
uint8_t time_buff[6];




uint8_t end_flag=0;


uint8_t USB_Cheak_init_flag=1;//Ϊ1��ʾUSB��ʼ���ϣ�Ҫ���г�ʼ�����ӣ�Ϊ0��ʾ����֮���Ѿ���ʼ������

// uint8_t YH_BH_H=0x00;
// uint8_t YH_BH_L=0x00;

uint8_t unlock_result=0;		//0��ʾ��ʼ״̬��1��ʾ��Ȩ��2��ʾδ��Ȩ

uint8_t guanjiflag=0;//�ػ���־

uint8_t key_code=0;		//ҪԿ�ױ��






//uint8_t m_Uart_State;



uint8_t Rfid_Buff[6]; 
uint8_t g_Buff[MAX_BUFF];



uint32_t author_addr,author_row,row_num,temp_addr;
uint8_t author_data[24];

uint32_t caima_addr,caima_row,row_num2,temp_addr2;
uint8_t caima_data[8];

uint32_t record_addr,record_row,row_num3,temp_addr3;
uint8_t record_data[5];



uint8_t g_State;



uint8_t shutdown_flag=0;

uint8_t f_CurSel=1;

//mode=0;��ʾ������ʾ��mode=1����ʾ����
uint8_t mode=0;



uint8_t bluetooth_uart_switch=1;		//Ĭ�Ͽ�������


uint8_t mac_query_result=0;


//Ϊ�˵���Э�������ı���
uint8_t item[25]={0};

uint32_t row=0;

uint32_t point=0;



