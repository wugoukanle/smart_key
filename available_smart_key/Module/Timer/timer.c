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
*
*
* 补充说明：区分两个概念,任务时间和定时器时间；任务时间=interval*定时器时间
*						采用了定时器任务数组，一个定时器任务结构数组的每个任务都基于一个相同的定时器
*						每个任务的定时器时间都是定时器定时时间的整数倍
*						当任务时间到达时，可以执行对应定时任务的处理函数
*
*
*
*
**********************************************************************/ 




#include "timer.h"
#include "stm32f10x_tim.h"


uint8_t 			g_WaitTimeOut;
uint8_t       TimeInitialized = 0;
TASKSTRUCT    m_TS[TIMER_TASK_COUNT];




// void Clear_WDT(void)
// {
//   IWDG_ReloadCounter();	  //清看门狗数值
// }


/*
*@功能：定时器采用向上计数的方式
*@注意：默认APB1是经过2分频的，所以TIM的输入时钟是72MHZ
*@说明：默认情况下，SYSCLK=HCLK=PCLK2=72MHZ，PCLK1=36MHZ
*				断服务程序间隔时间为  ((1+TIM_Prescaler )/72M)*(1+TIM_Period )
*				定时器时间为10ms
*/
void Init_Timer(void)
{
    uint16_t idx;
    TASKSTRUCT *pTS;          //定义一个结构指针 
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			//TIM Time Base Init structure definition
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
		
		TIM_DeInit(TIM2);					//复位TIM2定时器
	
		//CK_CNT=CK_INT/(TIM_Perscaler+1),CK_CNT是定时器时钟
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	 //更改此处可以降低TIMER的输入频率,累计0x100后
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 10000-1;			//定义计数周期，即0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
		TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
		TIM_Cmd(TIM2,DISABLE);    //关闭定时计数器
	
	
    pTS = &m_TS[0];           //定义的定时器数组
    for(idx=0;idx<TIMER_TASK_COUNT;idx++)
    {
        pTS->flag = FLG_TASK_NORMAL;
        pTS->timer_id = TIMER_NOUSE;
        pTS->interval = 0;
        pTS->count = 0;
        pTS->pFun = 0;
        ++pTS;
    }
    TimeInitialized = 0;
}








/*
*@功能：安装定时任务
*				配置的是TIM2
*				TimerFun是一个函数指针
*
*@说明：任务数组中任务有多少个，就可以设置SetTimer()多少次
*				将一个定时器的定时时间的不同整数倍作为不同的任务时间
*
*/
uint8_t SetTimer(uint8_t timer_id, uint16_t interval, TimerFun pTask, uint16_t Time_Top )
{
    uint16_t idx;
    TASKSTRUCT *pTS;
    if (pTask==0 || interval==0 || timer_id == TIMER_NOUSE)			//输入参数错误就返回
    {
        return ERR_TIMER_PARAMETER;			//定时器参数错误
    }
		
		

    TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能，将不产生更新事件                      
    pTS = &m_TS[0];		//m_TS[TIMER_TASK_COUNT]，是一个全局变量
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)   //看似所有的任务都会重设为相同的任务结构，循环中有一个判断可以防止
    {
        if (pTS->timer_id == timer_id)			 //如果任务ID存在，则直接启动定时器更新；防止所有任务设为相同
        {
            TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //使能定时器更新中断                 
            return  ERR_TIMER_TASKEXIST;		 //任务参数存在					
        }
				else
				{
            if (pTS->pFun == 0)				 //定时器任务结构数组元素的处理函数为空
            {
                if (!TimeInitialized)  //如果定时器没有启动，则启动它
                {
                    Start_Timer(Time_Top);
                }
                pTS->flag = FLG_TASK_NORMAL;			//设置定时器任务的状态为正常
                pTS->timer_id = timer_id;					//设置定时器任务的ID为指定ID
                pTS->interval = interval;					//设置定时器任务的间隔为指定间隔
                pTS->count = 0;										//在定时器中断中count一直在自增，当pTS->count >= pTS->interval，执行任务处理函数
                pTS->pFun = pTask;
                TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能，即更新影子寄存器
                return ERR_TIMER_OK;
            }
       }
       ++pTS;
    }
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能      //打开中断
	  //TIM_Cmd(TIM1,ENABLE);
    return ERR_TIMER_TOOMANYTASK;
}



/*
*@功能：停止定时功能
*				停止指定ID的定时器任务				
*
*@说明：只是关闭任务结构数组中指定ID的任务，并不关闭定时器，只有全部任务都没有处理函数时关闭定时器
*/
void KillTimer(uint8_t timer_id)  
{
    uint16_t idx,cnt=0;
    TASKSTRUCT *pTS;

    TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
    pTS = &m_TS[0];      //将数组的首地址发给指针
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)
    {
        if (pTS->timer_id == timer_id)   //将定时器任务结构数组中元素的定时器ID
        {
            pTS->flag = FLG_TASK_NORMAL;
            pTS->timer_id = TIMER_NOUSE;
            pTS->interval = 0;
            pTS->count = 0;
            pTS->pFun = 0;
        }
        if (pTS->pFun) ++cnt;		//如果其定时器任务的处理函数不为空，则计数加
        ++pTS;
    }
		
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能      
    if (cnt==0)					//如果任务结构数组中所有任务都没有处理函数，则关闭定时器
        Stop_Timer();
}







/*
*@功能：初始化TIM2, 10ms 一次
*
*@功能：初始化所有任务
*/
void Start_Timer(uint16_t Interal)         
{ 
		uint8_t idx;
		TASKSTRUCT *pTS;
		pTS = &m_TS[0];			//取全局定时器任务结构数组
		for (idx=0;idx<TIMER_TASK_COUNT;idx++)			//将定时器任务结构数组初始化
		{
				pTS->flag = FLG_TASK_NORMAL;
				pTS->timer_id = TIMER_NOUSE;
				pTS->interval = 0;
				pTS->count = 0;
				pTS->pFun = 0;
				++pTS;
		}
		TimeInitialized = 1;
		TIM_SetCounter(TIM2, 0x0000);     						//清除计数器的初始值
		TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //开启定时器更新中断使能
		TIM_Cmd(TIM2,ENABLE);    										//开启定时计数器
}




void Stop_Timer(void)
{
   	TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
	  TIM_Cmd(TIM2,DISABLE);    										//关闭定时计数器
    TimeInitialized = 0;       
}



/*
*@功能：超时将g_WaitTimerOut置为true
*/
void Do_TimeOut(void)
{
   g_WaitTimeOut =1;
}




/*
*@功能：定时器TIM2的中断处理函数
*@说明：已放到stm32f10x_it.c中
*/
// void TIM2_IRQHandler(void)
// {
//     uint8_t idx;	   
// 	  TASKSTRUCT *pTS;
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//     {
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//   	  
//        pTS = &m_TS[0];															//TIMER_TASK_COUNT=3是为了充分利用这个定时器
//        for (idx =0; idx<TIMER_TASK_COUNT;idx++)
//        {
//             if (pTS->pFun)
//             {
//                 pTS->count++;
//                 if (pTS->count >= pTS->interval)		//相当于将一个定时器定时时间翻倍interval
//                 {
//                     pTS->pFun();										//因为只有到了给定的倍数才将g_WaitTimeOut置为true
//                     pTS->count = 0;
//                 }
//             }
//             ++pTS;
//        }
//    
//   }
// }







/*******************************************************
下面的函数全部都没有用到，用的是TIM1，使用扫描模式
********************************************************/


//初始化定时器0作为	Key
void Init_Timer0(void)
{
  //TASKSTRUCT *pTS;          //定义一个结构指针 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			//TIM Time Base Init structure definition
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Prescaler = 0xFF;	 //更改此处可以降低TIMER的输入频率
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period =0x0500; //0xFFFE;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
	TIM_Cmd(TIM1,DISABLE);    //关闭定时计数器
}



void Start_Timer0(void)
{
	TimeInitialized = 1;
	TIM_SetCounter(TIM1, 0x0000);     //清除计数器的初始值，开计数，开中断
	TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE);	  //禁止定时器更新中断使能
	TIM_Cmd(TIM1,ENABLE);
}




void Stop_Timer0(void)
{
	TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE);	  //禁止定时器更新中断使能
	TIM_Cmd(TIM1,DISABLE);    //关闭定时计数器
}





//此定时器采用扫描的模式，不采用中断的模式
void WaitTF0(uint32_t interval)
{
	 TIM_SetAutoreload(TIM1,interval);
	 Start_Timer0();
	 while (TIM_GetITStatus(TIM1, TIM_IT_Update) == RESET);
	 TIM_ClearITPendingBit( TIM1, TIM_IT_Update);
	 Stop_Timer0();
}







