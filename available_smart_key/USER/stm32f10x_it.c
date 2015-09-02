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
*@功能：串口1中断处理
*/
void USART1_IRQHandler(void)
{
  uint8_t nChar;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//NE就表示有数据；表示RDR有数据
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
*@功能：串口2中断处理，即ＲＦＩＤ的串口中断
*/
void USART2_IRQHandler(void)
{
  uint8_t nChar;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    nChar =USART_ReceiveData(USART2);
    m_UB2.RBuff[m_UB2.cRHead] = nChar;		//将接受的字符放在缓冲区中
		++m_UB2.cRHead;
		if (m_UB2.cRHead >= UARTRXLEN1)
			m_UB2.cRHead = 0;		//让缓冲区成为一个环
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}



/*
*@功能：串口3中断处理，即BLUETOOTH的串口中断
*/
void USART3_IRQHandler(void)
{
  uint8_t nChar;
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    nChar =USART_ReceiveData(USART3);
    m_UB3.RBuff[m_UB3.cRHead] = nChar;		//将接受的字符放在缓冲区中
		++m_UB3.cRHead;
		if (m_UB3.cRHead >= UARTRXLEN1)
			m_UB3.cRHead = 0;		//让缓冲区成为一个环
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  }
}







/*
*@功能：定时器TIM2的中断处理函数
*
*@说明：每次中断时都处理一遍定时器任务数组中的每个任务
*				断服务程序间隔时间为  ((1+TIM_Prescaler )/72M)*(1+TIM_Period )
*
*/
void TIM2_IRQHandler(void)
{
    uint8_t idx;	   
	  TASKSTRUCT *pTS;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)		//Checks whether the TIM interrupt has occurred or not.
    {
       TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//Clears the TIMx's interrupt pending bits.
  	  
       pTS = &m_TS[0];		//TIMER_TASK_COUNT=3是为了充分利用这个定时器
       for (idx =0; idx<TIMER_TASK_COUNT;idx++)
       {
            if(pTS->pFun)  //若定时器任务结构数组元素的处理函数存在
            {
                pTS->count++;
                if (pTS->count >= pTS->interval)		//相当于将一个定时器定时时间翻倍interval
																										//即实际定时时间=interval*定时器定时时间
                {
                    pTS->pFun();		//因为只有到了给定的倍数才将g_WaitTimeOut置为true
																		//即当定时器任务结构数组的定时时间到了，则执行任务结构的处理函数
                    pTS->count = 0;
                }
            }
            ++pTS;
       }
   
  }
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
