/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"
#include "rfid.h"
#include "bluetooth.h"
#include "timer.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 



/*
*@���ܣ�����1�жϴ���
*/
void USART1_IRQHandler(void)
{
  uint8_t nChar;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//NE�ͱ�ʾ�����ݣ���ʾRDR������
  {
    nChar =USART_ReceiveData(USART1);
    m_UB1.RBuff[m_UB1.cRHead] = nChar;
		++m_UB1.cRHead;
		if (m_UB1.cRHead >= UARTRXLEN1)
			m_UB1.cRHead = 0;
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}



/*
*@���ܣ�����2�жϴ������ңƣɣĵĴ����ж�
*/
void USART2_IRQHandler(void)
{
  uint8_t nChar;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    nChar =USART_ReceiveData(USART2);
    m_UB2.RBuff[m_UB2.cRHead] = nChar;		//�����ܵ��ַ����ڻ�������
		++m_UB2.cRHead;
		if (m_UB2.cRHead >= UARTRXLEN1)
			m_UB2.cRHead = 0;		//�û�������Ϊһ����
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}



/*
*@���ܣ�����3�жϴ�����BLUETOOTH�Ĵ����ж�
*/
void USART3_IRQHandler(void)
{
  uint8_t nChar;
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    nChar =USART_ReceiveData(USART3);
    m_UB3.RBuff[m_UB3.cRHead] = nChar;		//�����ܵ��ַ����ڻ�������
		++m_UB3.cRHead;
		if (m_UB3.cRHead >= UARTRXLEN1)
			m_UB3.cRHead = 0;		//�û�������Ϊһ����
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  }
}







/*
*@���ܣ���ʱ��TIM2���жϴ�����
*
*@˵����ÿ���ж�ʱ������һ�鶨ʱ�����������е�ÿ������
*				�Ϸ��������ʱ��Ϊ  ((1+TIM_Prescaler )/72M)*(1+TIM_Period )
*
*/
void TIM2_IRQHandler(void)
{
    uint8_t idx;	   
	  TASKSTRUCT *pTS;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)		//Checks whether the TIM interrupt has occurred or not.
    {
       TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//Clears the TIMx's interrupt pending bits.
  	  
       pTS = &m_TS[0];		//TIMER_TASK_COUNT=3��Ϊ�˳�����������ʱ��
       for (idx =0; idx<TIMER_TASK_COUNT;idx++)
       {
            if(pTS->pFun)  //����ʱ������ṹ����Ԫ�صĴ���������
            {
                pTS->count++;
                if (pTS->count >= pTS->interval)		//�൱�ڽ�һ����ʱ����ʱʱ�䷭��interval
																										//��ʵ�ʶ�ʱʱ��=interval*��ʱ����ʱʱ��
                {
                    pTS->pFun();		//��Ϊֻ�е��˸����ı����Ž�g_WaitTimeOut��Ϊtrue
																		//������ʱ������ṹ����Ķ�ʱʱ�䵽�ˣ���ִ������ṹ�Ĵ�����
                    pTS->count = 0;
                }
            }
            ++pTS;
       }
   
  }
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
