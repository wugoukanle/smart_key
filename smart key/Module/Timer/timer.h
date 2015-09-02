/**********************************************************************
*												smart key
*										 Module：定时器模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-23 11:08     1.0             build this file 
*
* 模块功能：
*				通过定时器TIM2实现定时延时功能
*				
**********************************************************************/ 




#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"



//Timer 部分的错误代码
#define ERR_TIMER_OK            0
#define ERR_TIMER_TOOMANYTASK   1
#define ERR_TIMER_NOSUCHTASK    2
#define ERR_TIMER_TASKEXIST     4
#define ERR_TIMER_PARAMETER     3

#define FLG_TASK_NORMAL         0x00
#define FLG_TASK_READY          0x01
#define FLG_TASK_DELAYED        0x02

#define TIMER_TASK_COUNT   3
#define TIMER_NOUSE        255  //禁止使用编号

#define  ONE_MIN    3907
#define  TOW_MIN    7814
#define  THREE_MIN  11721
#define  FOUR_MIN   15628
#define  HALF_MIN   1500




//typedef  void (*TimerFun)(uchar para1, uint para2);
typedef  void (*TimerFun)(void);   //定义一个函数指针

typedef  struct tgTaskStuct 
{
    uint8_t   flag;
    uint8_t   timer_id;
    uint16_t  interval;
    uint16_t  count;
    TimerFun  pFun;
} TASKSTRUCT;


extern uint8_t g_WaitTimeOut;
extern uint8_t TimeInitialized;
extern TASKSTRUCT m_TS[TIMER_TASK_COUNT];


// void Clear_WDT(void);


void Init_Timer(void);
uint8_t SetTimer(uint8_t timer_id, uint16_t interval, TimerFun pTask,uint16_t Time_Top );//安装定时任务
void KillTimer(uint8_t timer_id);//停止定时任务

void Start_Timer(uint16_t Interal);
void Stop_Timer(void);
void Do_TimeOut(void);


/*******************************************************
下面的函数全部都没有用到，用的是TIM1，使用扫描模式
********************************************************/
void Init_Timer0(void);
void Start_Timer0(void);
void Stop_Timer0(void);
void WaitTF0(uint32_t interval);


#endif
