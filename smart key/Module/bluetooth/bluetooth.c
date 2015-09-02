/**********************************************************************
*												smart key
*										 Module：蓝牙模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-21 18:15     1.0             build this file 
*
* 模块功能：
*				蓝牙模块通过串口3与STM32通信
*				

*	连接说明：
* -----------------------------------------------------------
*       GND   电源地
*       VCC   接3.3V
*       TX3   接PB10
*       RX2   接PB11
*				EN    接PA11			EN为低是广播状态，高休眠
*				BRST  接PA12			BRST为低，表示主机有数据去发送
* ----------------------------------------------------------
*				
**********************************************************************/

#include "bluetooth.h"
#include "iwdg.h"
#include "Globle_Var.h"
#include "spi_flash.h"
#include "delay.h"
#include "usb.h"

//串口读程序需要用到
#define ERR_UART_OK     0x00   //正常
#define ERR_UART_EMPTY  0x01   //缓冲区空

UARTBUFF3  m_UB3;
uint8_t    m_Uart_State3;


/*
串口的配置分为3步

1.串口初始化（外设时钟开启，GPIO配置）

2.串口参数配置

3.串口中断开启（统一放在nvic.c中）

4.串口中断处理函数（统一放在stm32f10x_it.c中）

*/

/*
*@功能：串口3初始化函数
*/
void Uart3_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	//USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure; 
	
	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		
	
	//配置UART3 TX（PB10）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//配置UART3 RX（PB11）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
// 	//USART3 模式配置
// 	USART_InitStructure.USART_BaudRate = 115200;
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
// 	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
// 	USART_InitStructure.USART_Parity = USART_Parity_No;
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1;
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
// 	USART_Init(USART3,&USART_InitStructure);
// 	
// 	//使能串口3接收中断 
// 	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
// 	
// 	USART_Cmd(USART3,ENABLE);//使能USART1

	
	  //配置USART1接收中断
// 	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
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
void USART3_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

/* USART3 configuration ------------------------------------------------------*/
  /* USART3 configured as follow:
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
  USART_InitStructure.USART_BaudRate =9600;//115200;//4800;//9600; //115200; //9600;
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
  USART_Init(USART3, &USART_InitStructure);
	
  /* Enable USART3 */
  USART_Cmd(USART3, ENABLE);
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); /*接收中断使能*/
}


/*
*@功能：主要是蓝牙的EN和BRTS的GPIO配置，
*				用于控制蓝牙的4个宏SET_BT_EN，CLR_BT_EN，
*				SET_BT_BRTS，CLR_BT_BRTS
*/
void bluetooth_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

}


/*
*
*	重定向c库函数printf到USART3
*
*/ 
// int fputc(int ch,FILE *f)
// {

// 	//发送一个字节数据到USART1
// 	USART_SendData(USART3,(uint8_t) ch);
// 	
// 	//等待发送完毕
// 	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
// 	
// 	return (ch);

// }

/*
*
*	重定向c库函数scanf到USART1
*
*/ 
// int fgetc(FILE *f)
// {
// 		// 等待串口3输入数据 
// 		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

// 		return (int)USART_ReceiveData(USART3);
// }




/*
*功能：复位串口3缓冲区头尾指针，相当于清缓冲区
*/
void UART3Reset(uint8_t uart)
{		
		//先关接受中断
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt
		//UARTBUFF  m_UB1;
		m_UB3.cFlag = 0;
		m_UB3.cRHead = 0;
		m_UB3.cRTail = 0;
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}





/*
*@功能：通过串口3发送一个字符
*/
void UART3SendChar(uint8_t uart, uint8_t data)
{
		CLR_BT_BRTS;
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
				USART_SendData(USART3,data);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
		SET_BT_BRTS;
}



/*
*@功能：通过串口3发送n个字符
*
*@注意：STM32的处理速率与
*/
void UART3nSendChar(uint8_t uart,uint8_t *str,uint8_t len)  //串口发送字符串
{
		
		uint8_t idx;
	

	
 		CLR_BT_BRTS;
 		delay_ms(500);
// 	
// 		USART_ClearFlag(USART3,USART_FLAG_TC);	//硬件复位后，串口发送的首个数据之前没有read SR的操作，是直接write DR，也就是说，TC没有被清除掉

	
		for (idx=0;idx<len;idx++)
		{
			//USART_GetFlagStatus(USART3, USART_FLAG_TC);
			USART_SendData(USART3,str[idx]);	
// 			delay_ms(20);
// 			delay_us(500);
			while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 		//等待发送完成

// 			OLED_Clear();
// 			OLED_ShowUnite(16,2,"一字节发送成功");			
			
		}
		delay_ms(1000);		//串口发包间隔 TS (ms) 当 L<80时，TS >= T(BLE连接间隔,默认为200ms)			
 		SET_BT_BRTS;
 		delay_ms(1000);
		//UART3SendChar(3,0);	

}
// /*
// *@功能：通过串口3发送n个字符
// */
// void UART3nSendChar(uint8_t uart,uint8_t *str,uint8_t len)  //串口发送字符串
// {
// 		
// 		uint8_t idx;
//  		CLR_BT_BRTS;
// 		for (idx=0;idx<len;idx++)
// 		{
// 			USART_SendData(USART3,str[idx]);		
// 			while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 		//等待发送完成
// 		}
//  		SET_BT_BRTS;
// }








/*
*@功能：从串口3的缓冲区读一个字符
*/
uint8_t UART3ReadChar(uint8_t uart)
{

	uint8_t val='*';
	m_Uart_State3 = ERR_UART_OK;	    //检测串口缓冲区是否读到数据
	{

		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);                    // Disable USART0 RX interrupt
		if (m_UB3.cRHead != m_UB3.cRTail)
		{
			val = m_UB3.RBuff[m_UB3.cRTail++];
			if (m_UB3.cRTail>=UARTRXLEN3) m_UB3.cRTail = 0;
		}
		else
		  m_Uart_State3 = ERR_UART_EMPTY;
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                    // Enable USART0 RX interrupt

	}
	return val;
}


/*
*@功能：从串口缓冲区读取长度len的数据到dst所指地址
*/
uint16_t UART3nReadChar(uint8_t uart, uint8_t *dst, uint16_t len)
{
	uint16_t idx;
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);                  // Disable USART0 RX interrupt
  {
		for (idx=0;idx<len;idx++)
		{
			*(dst+idx) = m_UB3.RBuff[m_UB3.cRTail];
			m_UB3.cRTail = (m_UB3.cRTail+1)% UARTRXLEN3;
		}
	}
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                    // Enable USART0 RX interrupt
	return len;
}



/*
*@功能：读取按协议格式发送过来的数据，并进行解析
*/
uint8_t UART3ReadFrm(uint8_t uart, uint8_t *pdst, uint32_t max)
{
  uint8_t count = 0,total;
  uint8_t idx,off;
  IWDG_Feed();
  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);		//关接收中断，在接受中断USART1_IRQHandler()不断的将接受的数据放在m_UB1
  if (m_UB3.cRHead != m_UB3.cRTail)
   {
      //计算有效数据总长度;得到数据时head会增加
      if (m_UB3.cRHead >= m_UB3.cRTail)		//如果头大于尾
      {
        total  = m_UB3.cRHead - m_UB3.cRTail;
      } 
			else
			{
        total  = UARTRXLEN3 + m_UB3.cRHead - m_UB3.cRTail ;		//如果尾大于头
      }
      //找结束符
      if (total>4)//帧长度至少为4
      {
          if (m_UB3.RBuff[m_UB3.cRTail]!=(uint8_t)0xaa)		//
          {
              m_UB3.cRTail = (m_UB3.cRTail+1)% UARTRXLEN3;		//UARTRXLEN为254,cRTail加1
          }
					else
					{   
              off = (m_UB3.cRTail + 1) % UARTRXLEN3;		//cTail的下一个位置的地址
              if (m_UB3.RBuff[off]!=(uint8_t)0x75)
              {
                m_UB3.cRTail = (m_UB3.cRTail+2)% UARTRXLEN3;		//再将cTail加1
              }
							else		//表示接收到的第一个数aa，第二个数75
							{
                  off = (m_UB3.cRTail + 3) % UARTRXLEN3;	//off表示第3个数，cRTail没有变
									count=m_UB3.RBuff[off]+5;		//5表示开头2个字符，1个命令字，1个长度，0x00
//                   count = m_UB1.RBuff[off]<<8;//数据长度高字节		表示取第三个数的高8位
//                   off = (m_UB1.cRTail + 4) % UARTRXLEN;//off表示第4个数
//                   count = count + m_UB1.RBuff[off]+7;//数据长度低字节，两者相加后count表示整个数据长度，7表示2个开头字符，1个命令字，2个长度，0x00,检验位
                  if (count <= total && count < max)	//total是缓冲区有效数据的长度，count是整个命令帧的长度,所以永远有count<total
                  {
                      //数据长度满足一帧的要求
                      //total = 0;//纪录校验和,total换了一种含义
                      for (idx=0; idx < max-1 && idx<count;idx++)
                      {
                        off = (m_UB3.cRTail + idx) % UARTRXLEN3;		//cRTail也没有变
                        pdst[idx] = m_UB3.RBuff[off];		////将接受缓冲区的按命令帧存入pdst数组
                        //total ^= pdst[idx];//计算校验和
                      }
//                      if (total==0)//校验正确，此处为奇校验
                          m_UB3.cRTail = (off + 1) % UARTRXLEN3;		//校验正确就将cRTail放在整个数据帧的下一个位置
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
	  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	  IWDG_Feed();
    return count;
}



/*
*@功能：返回缓冲区内字节数
*/
uint16_t UART3GetCount(uint8_t uart)
{
		uint16_t count = 0 ;
    {
				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);           // Disable USART3 RX interrupt
				if (m_UB3.cRHead >= m_UB3.cRTail)
				{
					count  = m_UB3.cRHead - m_UB3.cRTail;
				}
				else
				{
					count  = UARTRXLEN3 + m_UB3.cRHead - m_UB3.cRTail;
				}
			
				USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);              // Enable USART3 RX interrupt
	  }
	  return count;
}



/*
*@功能：蓝牙MAC地址查询
*
*				将MAC地址写入外部flash中
*/
uint8_t bluetooth_mac(void)
{
	uint8_t j=0,t=0,count=0;
	uint8_t try_num=1;
	uint8_t char_buf[100];
	uint8_t MAC[6];
	uint8_t MAC_NULL[6]={0,0,0,0,0,0};

	UART3Reset(3);

	for(t=0;t<try_num;t++)				//至多尝试3次
	{
			UART3nSendChar(3,"TTM:RST-SYSTEMRESET",20);		//蓝牙串口AT指令
			delay_ms(50);
		
			UART3nSendChar(3,"TTM:MAC-?",10);		//蓝牙串口AT指令
			delay_ms(50);		//延时等待蓝牙回复
		
			count=UART3GetCount(3);
			UART3nReadChar(3,char_buf,count);			//读缓冲区数据到char_buf
		
		
			//for debug
// 			UART1nSendChar(1,&count,1);		//发送读到的所有数据
// 			delay_ms(100);			
		
		
			//for debug
// 			UART1nSendChar(1,char_buf,count);		//发送读到的所有数据
// 			delay_ms(100);					
		
			for(j=0;j<count-1;j++)		//从for循环退出的话，j=buf_len-1
				if(char_buf[j]=='0'&&char_buf[j+1]=='x')
						break;								//从break退出的话，j<buf_len-1
				

			if(j<count-1-12)							//如果提前成功
			{
				//for debug
				//UART1nSendChar(1,char_buf+j,12);		//已字符形式发送MAC地址
				//delay_ms(100);
				
				j=j+2;
				CharToHex(char_buf+j,MAC,12);		
				
				SPI_FLASH_Write(MAC,BLUETOOTH_MAC,6);
							
				return 1;//不用再尝试了
				
			}

			
	}
	
	//失败
	if(t>=try_num)
	{
		
		SPI_FLASH_Write(MAC_NULL,BLUETOOTH_MAC,6);
		return 0;
	}
  
}
