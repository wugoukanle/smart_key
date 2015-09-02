/**********************************************************************
*												smart key
*										 Module��ͨ��Э��ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-22 16:12     1.0             build this file 
*
* ģ�鹦�ܣ�
*				�涨������λ����ͨ�Ÿ�ʽ
*				
*
*				
**********************************************************************/



#ifndef __USB_H
#define __USB_H


#include "spi_flash.h"





uint8_t  USB_Cheak_init(void);
void USB_transmit(void);

void Test_Usb_Data(void);
void Test_Bluetooth_Data(void);

void register_key(void);
void mac_query(void);
void CharToHex(uint8_t * buffer, uint8_t * dest, uint32_t len);
void left_author_space(void);
void time_verify(void);
void authorization(void);
void back_record(void);
// void caima(void);
// void auto_back_caima(void);
void delete_device(void);
// void download_xunjian(void);
// void auto_back_xunjian(void);
void clear_all_author(void);
void pc_to_slave(void);
void pc_end_com(void);
void reset_flash(void);
void set_unlock_code(void);


//uint8_t key_state(void);


#endif

