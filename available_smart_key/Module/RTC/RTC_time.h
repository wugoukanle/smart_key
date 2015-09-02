/**********************************************************************
*												smart key
*										 Module��ʵʱʱ��ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-24 10:30     1.0             build this file 
*
* ģ�鹦�ܣ�
*				ʵʱʱ��ģ�飬ΪԿ���ṩʱ��
*				
**********************************************************************/ 



#ifndef __RTC_TIME_H
#define __RTC_TIME_H

#include "stm32f10x.h"



typedef struct date_t
{
  uint8_t hour;
  u8 min;
  u8 sec;
  u8 month;
  u8 day;
  u8 wday;
  uint16_t year;
} date_tYPE ;


extern  date_tYPE date_s;
void RTC_init(void);
static uint8_t IsLeapYear(uint16_t nYear);
void Date_Update(void);
void set_time(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t min,uint8_t sec);
void GetWeekDay(date_tYPE * tm);
void read_time(void);
void RTC_Configuration(void);
void RCT_InitTime(void);
uint8_t hex_bcd(uint8_t hex);
uint32_t Time_Regulate(void);

#endif
