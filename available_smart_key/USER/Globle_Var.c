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
*				全局变量定义
*						
**********************************************************************/

#include "Globle_Var.h"


//存储实时时间数组，分别是年月日时分秒
uint8_t time_buff[6];




uint8_t end_flag=0;


uint8_t USB_Cheak_init_flag=1;//为1表示USB初始插上，要进行初始化连接，为0表示插上之后已经初始化过了

// uint8_t YH_BH_H=0x00;
// uint8_t YH_BH_L=0x00;

uint8_t unlock_result=0;		//0表示初始状态，1表示授权，2表示未授权

uint8_t guanjiflag=0;//关机标志

uint8_t key_code=0;		//要钥匙编号






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

//mode=0;表示正常显示；mode=1，表示反显
uint8_t mode=0;



uint8_t bluetooth_uart_switch=1;		//默认开启蓝牙


uint8_t mac_query_result=0;


//为了调试协议而定义的变量
uint8_t item[25]={0};

uint32_t row=0;

uint32_t point=0;



