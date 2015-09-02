/**********************************************************************
*												smart key
*										 Module：串口通信模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-21 18:07     1.0             build this file 
*
* 模块功能：
*				串口通信，通过转接板与PC通信，本身带有MAX232电平转化
*				使用串口1

*	连接说明：
* -----------------------------------------------------------
*       GND   电源地
*       VCC   接5V
*       TX1   接PA9
*       RX1   接PA10              
*
*				USB_BIT接PA1
* ----------------------------------------------------------
*				
**********************************************************************/





#include "usart.h"
#include "iwdg.h"

UARTBUFF1  m_UB1;
uint8_t    m_Uart_State1;


/*
串口的配置分为3步

1.串口初始化（外设时钟开启，GPIO配置）

2.串口参数配置

3.串口中断开启（统一放在nvic.c中）

4.串口中断处理函数（统一放在stm32f10x_it.c中）

*/




/*
*@功能：串口1初始化函数
*/
void Uart1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure; 
	
	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	
	//配置UART1 TX（PA.9）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置UART1 RX（PA.10）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	


	//串口1的接受中断放在nvic.c中
	//配置USART1接收中断
// 	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);	
	
}


/*
*@功能：uart1 的配置
*				波特率：115200
*				数据长度：8位
*				停止位：1位
*				无奇偶校验
*				无硬件流控制
*				全双工
*@说明：可以根据需求更改
*/
void USART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
  USART_InitStructure.USART_BaudRate =115200;//115200;//4800;//9600; //115200; //9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//下面是同步模式下才用的
  //USART_InitStructure.USART_Clock = USART_Clock_Disable;
  //USART_InitStructure.USART_CPOL = USART_CPOL_Low;
  //USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
  //USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

  //USART_StructInit(USART_InitStructure);
  USART_Init(USART1, &USART_InitStructure);
	
  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /*接收中断使能*/
}







/*
*@功能：重定向c库函数printf到USART1
*/ 
int fputc(int ch,FILE *f)
{

	//发送一个字节数据到USART1
	USART_SendData(USART1,(uint8_t) ch);
	
	//等待发送完毕
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	
	return (ch);

}

/*
*@功能：重定向c库函数scanf到USART1
*/ 
int fgetc(FILE *f)
{
		// 等待串口1输入数据 
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}



/*
*@功能：USB_BIT用来判断USB是否插上
*/
void Usb_Power_Init_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    					//PC12即USB_BIT
}
 
 

 
 
/*
*@功能：串口1的接收中断函数
*/
// void USART1_IRQHandler(void)
// {
//   uint8_t nChar;
//   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//NE就表示有数据；表示RDR有数据
//   {
//     nChar =USART_ReceiveData(USART1);
//     m_UB1.RBuff[m_UB1.cRHead] = nChar;
// 		 ++m_UB1.cRHead;
// 		 if (m_UB1.cRHead >= UARTRXLEN)
// 			 m_UB1.cRHead = 0;
//     USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//   }
// }


/*
*功能：复位串口1缓冲区头尾指针，相当于清缓冲区
*/
void UART1Reset(uint8_t uart)
{		
		//先关接受中断
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt
		//UARTBUFF  m_UB1;
		m_UB1.cFlag = 0;
		m_UB1.cRHead = 0;
		m_UB1.cRTail = 0;
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}




/*
*@功能：通过串口1发送一个字符
*/
void UART1SendChar(uint8_t uart, uint8_t data)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
				USART_SendData(USART1,data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  
}





/*
*@功能：通过串口1发送n个字符
*/
void UART1nSendChar(uint8_t uart,uint8_t *str,uint8_t len)  //串口发送字符串
{
		uint8_t idx;
		//USART_ClearFlag(USART1,USART_FLAG_TC);//这里很重要，不然如果USART_FLAG_TC一开始就等于SET，就会覆盖第一个数据
		USART_GetFlagStatus(USART1, USART_FLAG_TC);
		for (idx=0;idx<len;idx++)
		{
			USART_SendData(USART1,str[idx]);		
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 		//等待发送完成
		}
}


/*
*@功能：通过串口1发送一个32位的数，高位在前
*/
void UART1SendwordHex(uint8_t uart, uint32_t data)
{
		uint8_t temp[4]={0,0,0,0};
	
	  temp[0]=(data&0xFF000000)>>24;
	  temp[1]=(data&0xFF0000)>>16;
	  temp[2]=(data&0xFF00)>>8;
	  temp[3]=data&0xFF;		
	
		UART1nSendChar(1,temp,4); 
}



/*
*@功能：等待发送，直到遇到结束符\0
*/
void UART1Send(uint8_t uart,const uint8_t *str)
{
		uint8_t idx=0;
		while (str[idx])
		{
			UART1SendChar(1,str[idx]);
			idx++;
		}
}




/*
*@功能：按新行发送，等待发送，直到完成,即前后都有回车换行
*/
void UART1SendNewLine(uint8_t uart,const uint8_t *str)
{
  uint8_t idx=0;

	UART1SendChar(1,'\r');
	UART1SendChar(1,'\n');
	while (str[idx])
	{
	  UART1SendChar(1,str[idx]);
		idx++;
	}
	UART1SendChar(1,'\r');
	UART1SendChar(1,'\n');
 
}



/*
*@功能：从串口1的缓冲区读一个字符
*/
uint8_t UART1ReadChar(uint8_t uart)
{

	uint8_t val='*';
	m_Uart_State1 = ERR_UART_OK;	    //检测串口缓冲区是否读到数据
	{

		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);                    // Disable USART0 RX interrupt
		if (m_UB1.cRHead != m_UB1.cRTail)
		{
			val = m_UB1.RBuff[m_UB1.cRTail++];
			if (m_UB1.cRTail>=UARTRXLEN1) m_UB1.cRTail = 0;
		}
		else
		  m_Uart_State1 = ERR_UART_EMPTY;
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    // Enable USART0 RX interrupt

	}
	return val;
}


/*
*@功能：从串口缓冲区读取长度len的数据到dst所指地址
*/
uint16_t UART1nReadChar(uint8_t uart, uint8_t *dst, uint16_t len)
{
	uint16_t idx;
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);                  // Disable USART0 RX interrupt
  {
		for (idx=0;idx<len;idx++)
		{
			*(dst+idx) = m_UB1.RBuff[m_UB1.cRTail];
			m_UB1.cRTail = (m_UB1.cRTail+1)% UARTRXLEN1;
		}
	}
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    // Enable USART0 RX interrupt
	return len;
}






/*
*@功能：读取一行,'\r'结束,返回读取的字符串长度
*/
uint8_t UART1ReadLine(uint8_t uart, uint8_t *pdst, uint8_t max)
{
		uint8_t count = 0,total;
		uint32_t idx,off;

		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		if (m_UB1.cRHead != m_UB1.cRTail)
		{
			//计算有效数据总长度
			if (m_UB1.cRHead >= m_UB1.cRTail)
			{
				total  = m_UB1.cRHead - m_UB1.cRTail;
			} 
			else
		  {
				total  = UARTRXLEN1 + m_UB1.cRHead - m_UB1.cRTail ;
			}
			//找结束符
			for (idx=0;idx<max-1 && idx<total;idx++)
			{
				off = m_UB1.cRTail + idx;
				if (off >= UARTRXLEN1)  
					off -= UARTRXLEN1;
				pdst[idx] = m_UB1.RBuff[off];
				if (m_UB1.RBuff[off]=='\r')
				{
						count = idx+1;
						pdst[count] = 0;
						m_UB1.cRTail = off + 1;
						if ( m_UB1.cRTail >= UARTRXLEN1) 
							m_UB1.cRTail =0;
						//增加内容，可以保证"\r\n"结束不影响下一行处理
			/*
						if (m_UB1.RBuff [m_UB1.cRTail]=='\n')
						{
								m_UB1.cRTail += 1;
								if ( m_UB1.cRTail >= UARTRXLEN) m_UB1.cRTail =0;
						}
			*/
						break;
				}
			}
		}
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		return count;
}





/*
*@功能：读取按协议格式发送过来的数据，并进行解析
*/
uint8_t UART1ReadFrm(uint8_t uart, uint8_t *pdst, uint32_t max)
{
  uint8_t count = 0,total;
  uint8_t idx,off;
  IWDG_Feed();
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		//关接收中断，在接受中断USART1_IRQHandler()不断的将接受的数据放在m_UB1
  if (m_UB1.cRHead != m_UB1.cRTail)
   {
      //计算有效数据总长度;得到数据时head会增加
      if (m_UB1.cRHead >= m_UB1.cRTail)		//如果头大于尾
      {
        total  = m_UB1.cRHead - m_UB1.cRTail;
      } 
			else
			{
        total  = UARTRXLEN1 + m_UB1.cRHead - m_UB1.cRTail ;		//如果尾大于头
      }
      //找结束符
      if (total>4)//帧长度至少为4
      {
          if (m_UB1.RBuff[m_UB1.cRTail]!=(uint8_t)0xaa)		//
          {
              m_UB1.cRTail = (m_UB1.cRTail+1)% UARTRXLEN1;		//UARTRXLEN为254,cRTail加1
          }
					else
					{   
              off = (m_UB1.cRTail + 1) % UARTRXLEN1;		//cTail的下一个位置的地址
              if (m_UB1.RBuff[off]!=(uint8_t)0x75)
              {
                m_UB1.cRTail = (m_UB1.cRTail+2)% UARTRXLEN1;		//再将cTail加1
              }
							else		//表示接收到的第一个数aa，第二个数75
							{
                  off = (m_UB1.cRTail + 3) % UARTRXLEN1;	//off表示第3个数，cRTail没有变
									count=m_UB1.RBuff[off]+5;		//5表示开头2个字符，1个命令字，1个长度，0x00
//                   count = m_UB1.RBuff[off]<<8;//数据长度高字节		表示取第三个数的高8位
//                   off = (m_UB1.cRTail + 4) % UARTRXLEN;//off表示第4个数
//                   count = count + m_UB1.RBuff[off]+7;//数据长度低字节，两者相加后count表示整个数据长度，7表示2个开头字符，1个命令字，2个长度，0x00,检验位
                  if (count <= total && count < max)	//total是缓冲区有效数据的长度，count是整个命令帧的长度,所以永远有count<total
                  {
                      //数据长度满足一帧的要求
                      //total = 0;//纪录校验和,total换了一种含义
                      for (idx=0; idx < max-1 && idx<count;idx++)
                      {
                        off = (m_UB1.cRTail + idx) % UARTRXLEN1;		//cRTail也没有变
                        pdst[idx] = m_UB1.RBuff[off];		////将接受缓冲区的按命令帧存入pdst数组
                        //total ^= pdst[idx];//计算校验和
                      }
//                      if (total==0)//校验正确，此处为奇校验
                          m_UB1.cRTail = (off + 1) % UARTRXLEN1;		//校验正确就将cRTail放在整个数据帧的下一个位置
//                       else
//                       {
//                           //校验错误，抛弃2个字节，其实为本帧的头
//                           m_UB1.cRTail = (m_UB1.cRTail + 2) % UARTRXLEN;
//                           count = 0;
//                       }
                   }
									 else count =0;
              }
          }
  	   }
	  }
	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	  IWDG_Feed();
    return count;
}



/*
*@功能：返回缓冲区内字节数
*/
uint16_t UART1GetCount(uint8_t uart)
{
		uint16_t count = 0 ;
    {
				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);           // Disable USART0 RX interrupt
				if (m_UB1.cRHead >= m_UB1.cRTail)
				{
					count  = m_UB1.cRHead - m_UB1.cRTail;
				}
				else
				{
					count  = UARTRXLEN1 + m_UB1.cRHead - m_UB1.cRTail;
				}
			
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);              // Enable USART0 RX interrupt
	  }
	  return count;
}





