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
#include "sw_key.h"
#include "wkup.h"
#include "Globle_Var.h"
#include "oled.h"



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
	
	
	//保存之前的屏幕
	//屏幕接口虽然是SPI，但是其没有MISO，读不了显示RAM 
	
		
	nChar =Read_RfidData(2,Rfid_Buff,0xff);			//非常重要的一点，是理解Trismit（）的关键
	if(nChar >=3)
	{
			Key_Trismit();	 
		
			if(g_State==ST_IDLE)
			{
				OLED_Clear();	
				ShowMenuNoInverse(HsMenu);	
				Main_Oled_Power();
				Main_Oled_Time();
				//g_State==ST_LCDMENU;
			}
			else
				InsertKey(KEYCLEAR);
			//显示之前的图片		
		
		
		
		
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
            if (pTS->pFun)
            {
                pTS->count++;
                if (pTS->count >= pTS->interval)		//相当于将一个定时器定时时间翻倍interval
                {
                    pTS->pFun();		//因为只有到了给定的倍数才将g_WaitTimeOut置为true
                    pTS->count = 0;
                }
            }
            ++pTS;
       }
   
  }
}




/***********************************************************/
//下面为按键中断处理
/***********************************************************/

/*
KEY2和KEY4

S2和S4的中断处理
S2接PC13，S4接PA15

如果发生中断唤醒，会先执行中断程序 ，再执行PWR_EnterSTOPMode的下1行代码
*/
void EXTI15_10_IRQHandler(void)
{
  uint8_t cw=0;
	
	
  if(EXTI_GetITStatus(EXTI_LINE_KEY2_BUTTON) != RESET)		//KEY2接PC13
  {
	  Key_Num =KEY2;
		
 		delay_ms(50);
		
		
		
// 		if(((GPIOC->IDR) & GPIO_Pin_13 )==0)  
// 			Key_Num=KEY2;
// 		else
// 			Key_Num=KEYNO;		
		
		if(stop_flag)			//唤醒时用于软件复位
			SoftReset();		
		
		
		
		cw=Check_WKUP();
		if(cw)//是否关机
		{		  
			OLED_Clear();	
			//Sys_Enter_Standby();  
			SoftReset();
			UART1nSendChar(1,"long press",10);
			longpress=1;
		}			
		

		
    /* Clear the Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_LINE_KEY2_BUTTON);	

		
		UART1nSendChar(1,"interrupt KEY2",15);	
		UART1SendByteASCII(1,Key_Num);	
		
// 		UART1SendChar(1,'\r');
// 		UART1SendChar(1,'\n');		
		

  }
  else if(EXTI_GetITStatus(EXTI_LINE_KEY4_BUTTON) != RESET)
  {
		Key_Num =KEY4;
		
 		delay_ms(50);
// 		if(((GPIOA->IDR) & GPIO_Pin_15 )==0)  
// 			Key_Num=KEY4;
// 		else
// 			Key_Num=KEYNO;	
		
		
// 		if(Check_WKUP())//是否关机
// 		{		  
// 			Sys_Enter_Standby();  
// 		}		
		
		EXTI_ClearITPendingBit(EXTI_LINE_KEY4_BUTTON);

		
		UART1nSendChar(1,"interrupt KEY4",15);
		UART1SendByteASCII(1,Key_Num);
		
// 		UART1SendChar(1,'\r');
// 		UART1SendChar(1,'\n');		
		
  }
	else
	{
// 		UART1nSendChar(1,"EXTI15_10 error",15);
// 		delay_ms(1000);
// 		UART1SendwordHex(1,EXTI->PR);
// 		delay_ms(1000);
		UART1nSendChar(1,"interrupt mask",15);
		delay_ms(1000);		
		
	}	

	


}




/*
KEY1和KEY3

S1和S3的中断处理
S1接PB5，S3接PB8
*/
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_LINE_KEY1_BUTTON) != RESET)		
  {
		Key_Num =KEY1; 
		
		
 		delay_ms(50);
// 		if(((GPIOB->IDR) & GPIO_Pin_5 )==0)  
// 			Key_Num=KEY1;
// 		else
// 			Key_Num=KEYNO;		
		
		
		/* Clear the Key Button EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_LINE_KEY1_BUTTON);
		//delay_ms(50);
		
		UART1nSendChar(1,"interrupt KEY1",15);	
		UART1SendByteASCII(1,Key_Num);	
		
// 		UART1SendChar(1,'\r');
// 		UART1SendChar(1,'\n');		
		
		
  }
  else if(EXTI_GetITStatus(EXTI_LINE_KEY3_BUTTON) != RESET)		//KEY3接PB8
  {
		Key_Num =KEY3;
		
 		delay_ms(50);
// 		if(((GPIOB->IDR) & GPIO_Pin_8 )==0)  
// 			Key_Num=KEY3;
// 		else
// 			Key_Num=KEYNO;

			
		EXTI_ClearITPendingBit(EXTI_LINE_KEY3_BUTTON);

		
		UART1nSendChar(1,"interrupt KEY3",15);
		UART1SendByteASCII(1,Key_Num);
		
// 		UART1SendChar(1,'\r');
// 		UART1SendChar(1,'\n');		
		
		
  }
	else
	{
// 		UART1nSendChar(1,"EXTI9_5 error",14);
// 		delay_ms(1000);
// 		UART1SendwordHex(1,EXTI->PR);
// 		delay_ms(1000);
		UART1nSendChar(1,"interrupt mask",14);
		delay_ms(1000);		
		
		
		
	}

}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
