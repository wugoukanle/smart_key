/**********************************************************************
*												smart key
*										 Module����ʱ��ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-23 11:08     1.0             build this file 
*
* ģ�鹦�ܣ�
*				ͨ����ʱ��TIM2ʵ�ֶ�ʱ��ʱ����
*				
*
*
* ����˵����������������,����ʱ��Ͷ�ʱ��ʱ�䣻����ʱ��=interval*��ʱ��ʱ��
*						�����˶�ʱ���������飬һ����ʱ������ṹ�����ÿ�����񶼻���һ����ͬ�Ķ�ʱ��
*						ÿ������Ķ�ʱ��ʱ�䶼�Ƕ�ʱ����ʱʱ���������
*						������ʱ�䵽��ʱ������ִ�ж�Ӧ��ʱ����Ĵ�����
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
//   IWDG_ReloadCounter();	  //�忴�Ź���ֵ
// }


/*
*@���ܣ���ʱ���������ϼ����ķ�ʽ
*@ע�⣺Ĭ��APB1�Ǿ���2��Ƶ�ģ�����TIM������ʱ����72MHZ
*@˵����Ĭ������£�SYSCLK=HCLK=PCLK2=72MHZ��PCLK1=36MHZ
*				�Ϸ��������ʱ��Ϊ  ((1+TIM_Prescaler )/72M)*(1+TIM_Period )
*				��ʱ��ʱ��Ϊ10ms
*/
void Init_Timer(void)
{
    uint16_t idx;
    TASKSTRUCT *pTS;          //����һ���ṹָ�� 
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			//TIM Time Base Init structure definition
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
		
		TIM_DeInit(TIM2);					//��λTIM2��ʱ��
	
		//CK_CNT=CK_INT/(TIM_Perscaler+1),CK_CNT�Ƕ�ʱ��ʱ��
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	 //���Ĵ˴����Խ���TIMER������Ƶ��,�ۼ�0x100��
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 10000-1;			//����������ڣ���0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
		TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
		TIM_Cmd(TIM2,DISABLE);    //�رն�ʱ������
	
	
    pTS = &m_TS[0];           //����Ķ�ʱ������
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
*@���ܣ���װ��ʱ����
*				���õ���TIM2
*				TimerFun��һ������ָ��
*
*@˵�������������������ж��ٸ����Ϳ�������SetTimer()���ٴ�
*				��һ����ʱ���Ķ�ʱʱ��Ĳ�ͬ��������Ϊ��ͬ������ʱ��
*
*/
uint8_t SetTimer(uint8_t timer_id, uint16_t interval, TimerFun pTask, uint16_t Time_Top )
{
    uint16_t idx;
    TASKSTRUCT *pTS;
    if (pTask==0 || interval==0 || timer_id == TIMER_NOUSE)			//�����������ͷ���
    {
        return ERR_TIMER_PARAMETER;			//��ʱ����������
    }
		
		

    TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ�ܣ��������������¼�                      
    pTS = &m_TS[0];		//m_TS[TIMER_TASK_COUNT]����һ��ȫ�ֱ���
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)   //�������е����񶼻�����Ϊ��ͬ������ṹ��ѭ������һ���жϿ��Է�ֹ
    {
        if (pTS->timer_id == timer_id)			 //�������ID���ڣ���ֱ��������ʱ�����£���ֹ����������Ϊ��ͬ
        {
            TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //ʹ�ܶ�ʱ�������ж�                 
            return  ERR_TIMER_TASKEXIST;		 //�����������					
        }
				else
				{
            if (pTS->pFun == 0)				 //��ʱ������ṹ����Ԫ�صĴ�����Ϊ��
            {
                if (!TimeInitialized)  //�����ʱ��û����������������
                {
                    Start_Timer(Time_Top);
                }
                pTS->flag = FLG_TASK_NORMAL;			//���ö�ʱ�������״̬Ϊ����
                pTS->timer_id = timer_id;					//���ö�ʱ�������IDΪָ��ID
                pTS->interval = interval;					//���ö�ʱ������ļ��Ϊָ�����
                pTS->count = 0;										//�ڶ�ʱ���ж���countһֱ����������pTS->count >= pTS->interval��ִ����������
                pTS->pFun = pTask;
                TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ�ܣ�������Ӱ�ӼĴ���
                return ERR_TIMER_OK;
            }
       }
       ++pTS;
    }
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��      //���ж�
	  //TIM_Cmd(TIM1,ENABLE);
    return ERR_TIMER_TOOMANYTASK;
}



/*
*@���ܣ�ֹͣ��ʱ����
*				ָֹͣ��ID�Ķ�ʱ������				
*
*@˵����ֻ�ǹر�����ṹ������ָ��ID�����񣬲����رն�ʱ����ֻ��ȫ������û�д�����ʱ�رն�ʱ��
*/
void KillTimer(uint8_t timer_id)  
{
    uint16_t idx,cnt=0;
    TASKSTRUCT *pTS;

    TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
    pTS = &m_TS[0];      //��������׵�ַ����ָ��
    for (idx=0;idx<TIMER_TASK_COUNT;idx++)
    {
        if (pTS->timer_id == timer_id)   //����ʱ������ṹ������Ԫ�صĶ�ʱ��ID
        {
            pTS->flag = FLG_TASK_NORMAL;
            pTS->timer_id = TIMER_NOUSE;
            pTS->interval = 0;
            pTS->count = 0;
            pTS->pFun = 0;
        }
        if (pTS->pFun) ++cnt;		//����䶨ʱ������Ĵ�������Ϊ�գ��������
        ++pTS;
    }
		
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��      
    if (cnt==0)					//�������ṹ��������������û�д���������رն�ʱ��
        Stop_Timer();
}







/*
*@���ܣ���ʼ��TIM2, 10ms һ��
*
*@���ܣ���ʼ����������
*/
void Start_Timer(uint16_t Interal)         
{ 
		uint8_t idx;
		TASKSTRUCT *pTS;
		pTS = &m_TS[0];			//ȡȫ�ֶ�ʱ������ṹ����
		for (idx=0;idx<TIMER_TASK_COUNT;idx++)			//����ʱ������ṹ�����ʼ��
		{
				pTS->flag = FLG_TASK_NORMAL;
				pTS->timer_id = TIMER_NOUSE;
				pTS->interval = 0;
				pTS->count = 0;
				pTS->pFun = 0;
				++pTS;
		}
		TimeInitialized = 1;
		TIM_SetCounter(TIM2, 0x0000);     						//����������ĳ�ʼֵ
		TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);	  //������ʱ�������ж�ʹ��
		TIM_Cmd(TIM2,ENABLE);    										//������ʱ������
}




void Stop_Timer(void)
{
   	TIM_ITConfig(TIM2,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
	  TIM_Cmd(TIM2,DISABLE);    										//�رն�ʱ������
    TimeInitialized = 0;       
}



/*
*@���ܣ���ʱ��g_WaitTimerOut��Ϊtrue
*/
void Do_TimeOut(void)
{
   g_WaitTimeOut =1;
}




/*
*@���ܣ���ʱ��TIM2���жϴ�����
*@˵�����ѷŵ�stm32f10x_it.c��
*/
// void TIM2_IRQHandler(void)
// {
//     uint8_t idx;	   
// 	  TASKSTRUCT *pTS;
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//     {
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//   	  
//        pTS = &m_TS[0];															//TIMER_TASK_COUNT=3��Ϊ�˳�����������ʱ��
//        for (idx =0; idx<TIMER_TASK_COUNT;idx++)
//        {
//             if (pTS->pFun)
//             {
//                 pTS->count++;
//                 if (pTS->count >= pTS->interval)		//�൱�ڽ�һ����ʱ����ʱʱ�䷭��interval
//                 {
//                     pTS->pFun();										//��Ϊֻ�е��˸����ı����Ž�g_WaitTimeOut��Ϊtrue
//                     pTS->count = 0;
//                 }
//             }
//             ++pTS;
//        }
//    
//   }
// }







/*******************************************************
����ĺ���ȫ����û���õ����õ���TIM1��ʹ��ɨ��ģʽ
********************************************************/


//��ʼ����ʱ��0��Ϊ	Key
void Init_Timer0(void)
{
  //TASKSTRUCT *pTS;          //����һ���ṹָ�� 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			//TIM Time Base Init structure definition
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Prescaler = 0xFF;	 //���Ĵ˴����Խ���TIMER������Ƶ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period =0x0500; //0xFFFE;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
	TIM_Cmd(TIM1,DISABLE);    //�رն�ʱ������
}



void Start_Timer0(void)
{
	TimeInitialized = 1;
	TIM_SetCounter(TIM1, 0x0000);     //����������ĳ�ʼֵ�������������ж�
	TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE);	  //��ֹ��ʱ�������ж�ʹ��
	TIM_Cmd(TIM1,ENABLE);
}




void Stop_Timer0(void)
{
	TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE);	  //��ֹ��ʱ�������ж�ʹ��
	TIM_Cmd(TIM1,DISABLE);    //�رն�ʱ������
}





//�˶�ʱ������ɨ���ģʽ���������жϵ�ģʽ
void WaitTF0(uint32_t interval)
{
	 TIM_SetAutoreload(TIM1,interval);
	 Start_Timer0();
	 while (TIM_GetITStatus(TIM1, TIM_IT_Update) == RESET);
	 TIM_ClearITPendingBit( TIM1, TIM_IT_Update);
	 Stop_Timer0();
}







