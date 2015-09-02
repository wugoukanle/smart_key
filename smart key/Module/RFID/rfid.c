/**********************************************************************
*												smart key
*										 Module：RFID模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-21 18:07     1.0             build this file 
*
* 模块功能：
*				通过串口2采取中断的方式读取RFID的数据
*				

*	连接说明：
* -----------------------------------------------------------
*       GND   电源地
*       VCC   3.3v电源
*       TX2   接PA2
*       RX2   接PA3             
* ----------------------------------------------------------
*				
* 补充：	RFID模块有两种读取方式 1.采用中断端口读取维根码的数据
																	2.采用串口中断模式读取数据
**********************************************************************/ 



#include "rfid.h"
#include "usb.h"
#include "oled.h"
#include "Globle_Var.h"
#include "usart.h"
#include "iwdg.h"
#include "delay.h"


UARTBUFF1  m_UB2;


//韦根部分
uint8_t byte_cnt=0;
uint8_t bit_cnt=0;
uint32_t wg_data[4]={0xff,0xff,0xff,0xff};
uint32_t wg_buf[4]={0xff,0xff,0xff,0xff};
uint8_t wg_mask=0x80;
uint8_t a;

void resume_wg26_data(void);
void store_tag_bit(uint8_t tag_data_bit );




uint8_t Lock_number[4]={0,0,0,0};		//暂存锁码，在check_lock_code中赋值，在save_rfid_result使用
uint8_t User_Num[2]={0,0};					//暂存用户编号，在check_lock_code中赋值，在save_rfid_result使用


/*
*@功能：两个端口  输入口，必须接在中断脚上
*				GPIO引脚外部中断模式。这里暂时不采用
*/
void Rfid_IO_Init_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //Key_IO--通信管脚，PA0，接EXITA，即WG0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //Key_IO--通信管，PA1,接EXITB，即WG1
}
 

/*
串口的配置分为3步

1.串口初始化（外设时钟开启，GPIO配置）

2.串口参数配置

3.串口中断开启（统一放在nvic.c中）

4.串口中断处理函数（统一放在stm32f10x_it.c中）

*/
 


/*
*@功能：串口2初始化函数
*/
void Uart2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	//USART_InitTypeDef USART_InitStructure;
	
	//NVIC_InitTypeDef NVIC_InitStructure; 
	
	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);		
	
	
	//配置UART2 TX（PA.2）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置UART2 RX（PA.3）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);


	//中断初始化部分放到nvic.c中
//  配置USART1接收中断
// 	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);	
	
}


 
 
/*
*@功能：uart2 的配置
*				波特率：9600
*				数据长度：8位
*				停止位：1位
*				无奇偶校验
*				无硬件流控制
*				全双工
*@说明：配置成这样是RFID模块确定的
*/
void USART2_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate =  9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	//同步通信时用的
	//USART_InitStructure.USART_Clock = USART_Clock_Disable;
	//USART_InitStructure.USART_CPOL = USART_CPOL_Low;
	//USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
	//USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

  //USART_StructInit(USART_InitStructure);
  USART_Init(USART2, &USART_InitStructure);
	
  /* Enable USART2 */
  USART_Cmd(USART2, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); /*接收中断使能*/
}


/*
*@功能：通过串口2发送一个字符（一个字节）
*/
void UART2SendChar(uint8_t uart,uint8_t data)
{
	
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
			USART_SendData(USART2,data);
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  
	
}


/*
*@功能：通过串口2发送n个字符（n个字节）
*/
void UART2nSendChar(uint8_t uart,uint8_t *str,uint8_t len)
{
	uint8_t idx;
	
	for (idx=0;idx<len;idx++)
	{
			USART_SendData(USART2,str[idx]);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
	}
	
}




/*
*@功能：串口2的接收中断函数，放到stm32f10x_it.c
**/
// void USART2_IRQHandler(void)
// {
//   u8 nChar;
//   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//   {
//     nChar =USART_ReceiveData(USART2);
//     m_UB2.RBuff[m_UB2.cRHead] = nChar;		//将接受的字符放在缓冲区中
// 		++m_UB2.cRHead;
// 		if (m_UB2.cRHead >= UARTRXLEN1)
// 			m_UB2.cRHead = 0;		//让缓冲区成为一个环
//     USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//   }
// }




/*
*@功能：读取RFID串口发送过来的数据包,判断是否正确
*
*@实现:最少为5个字节,进行判断
* 当接收缓冲区，接收到一定数目的字节时，进行判断
*	数据的接受过程在USART2_IRQHandler（）中处理
* 串口中断将接受到的数据全都放到缓冲区，数据长度不管；
*	然后Read_RfidData（）函数通过检验查找ID数据的开头，
*	如果不对就再向后移动一位，继续校验
*	如果正确就将从此位子开始的5个字节放入pdst中
*
*/
uint8_t Read_RfidData(uint8_t uart, uint8_t *pdst, uint8_t max)
{
	//uart没有用到，直接用的是USART2
  uint8_t count = 0,total;
  uint8_t idx,off;
  
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//关中断
  if(m_UB2.cRHead != m_UB2.cRTail)
  {
      //计算有效数据总长度
      if (m_UB2.cRHead >= m_UB2.cRTail)
      {
        total  = m_UB2.cRHead - m_UB2.cRTail;
      } 
			else
			{
        total  = UARTRXLEN2 + m_UB2.cRHead - m_UB2.cRTail ;
      }
      //找结束符
      if (total>=5)//帧长度至少为5，这里是正确的，保证缓冲区能容下ID
      {
        
						//数据长度满足一帧的要求
						total = 0;//纪录校验和
						for (idx=0; idx<5;idx++)
						{
							off = (m_UB2.cRTail + idx) % UARTRXLEN2;		//偏移地址
							pdst[idx] = m_UB2.RBuff[off];		//将缓冲区数据放到目标数组中
							total ^= pdst[idx];//计算校验和，应该是偶校验
						}
						if (total==0)//校验正确
					  {
							m_UB2.cRTail = (off + 1) % UARTRXLEN2;
						  count =5;
					  }
						else		//通过校验来找到ID数据的头，重点
						{
								//校验错误，抛弃1个字节，其实为本帧的头
								m_UB2.cRTail = (m_UB2.cRTail + 1) % UARTRXLEN2;
								count = 0;
						}
       }
	 }
// 	 m_UB2.cRHead=0;
// 	 m_UB2.cRTail=0;
	 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开中断
	 
	if(count ==5) 
	{
		if(pdst[0]==0&&pdst[1]==0&&pdst[2]==0&&pdst[3]==0)
			count=0;		//此时的count=5无效
		else		//至少有一个不为零
		{
			UART1nSendChar(1,pdst,5);
			UART2Reset(0);
		}
	}	 

  return count;		//count要么为5要么为0，返回0表示校验失败

}


/*
*@功能：清缓冲区
*/
void UART2Reset(uint8_t uart)
{		
		//先关接受中断
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt

		m_UB2.cFlag = 0;
		m_UB2.cRHead = 0;
		m_UB2.cRTail = 0;
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}








/************************
韦根26协议
HID：HiddenIDcode 隐含码
PID：PublicIDcode 公开码

26位=2~13位的校验位1位+HID的低8位+PID码16位+14~25位的校验位1位

韦根应该用外部中断，查询是不行的
****************************/


// void resume_wg26_data(void)
// {
// 	
// 		uint32_t i;
// 		wg_data[0] = wg_buf[0] << 1;
// 		wg_data[0] &=0xfe;
// 		i = wg_buf[1] >>7 & 0x01;
// 		wg_data[0] |= i;		//将2~9位存于wg_data[0],即HID的低八位
// 		
// 		wg_data[1] = wg_buf[1] << 1;
// 		wg_data[1] &=0xfe;
// 		i = wg_buf[2] >>7 & 0x01;
// 		wg_data[1] |= i;		//将10~17位存于wg_data[1]，即PID的高八位
// 		
// 		wg_data[2] = wg_buf[2] << 1;
// 		wg_data[2] &=0xfe;
// 		i = wg_buf[3] >>7& 0x01;
// 		wg_data[2] |= i;		//将wg_data存于wg_data[2]，即PID的低八位
// 	
// }




// void store_tag_bit(uint8_t tag_data_bit )		//这个函数应该是为中断准备的
// {
// 		//  ( mask = 0x80 ; mask > 0 ; mask >>= 1 )   //1000 0000=0x80
// 		if( tag_data_bit == 1 )
// 			{wg_buf[byte_cnt] |= wg_mask;}		//置1
// 		else
// 			{wg_buf[byte_cnt] &=~wg_mask;}    //清0              //0111 1111=~0x80   =0        

// 		wg_mask>>= 1;		//wg_mask右移1位
// 		bit_cnt ++;
// 		
// 		if(wg_mask == 0)		//	如果wg_mask右移1位之后为0
// 		{
// 				wg_mask = 0x80;
// 				byte_cnt++; 
// 		}
// 						//
// 		if(bit_cnt==26)		//接受完26位之后，重新清理wg_buf为wg_data
// 		{
// 			resume_wg26_data();		//将26位韦根数的HID存入wg_data[0],PID存入wg_data[1~2]
// 			byte_cnt=0;
// 			wg_mask=0x80;
// 			bit_cnt=0;
// 			//for(a=0;a<4;a++)
// 			//{uart_transmit(wg_data[a]);}        
// 		}
// }  








void Key_Trismit(void)
{
		uint8_t i,nChar;
		//AA  75 +11+06+00 +ID +4字节RFID+1字节校验位
    uint8_t PDA_RFID[11]={0xAA,0x75,0x11,0x06,0x00,0x01,0x00,0x00,0x00,0x00,0x00};


		//在ST_Idle（）中已经检测到了RFID，才会调到这里
		for(i =0;i<4;i++)		//Lock_number[4]和Rfid_Buff[6]都是全局变量
		 Lock_number[i] =Rfid_Buff[i]; //在ST_Idle()中已经将数据放入Rfid_Buff[]	

		
		for(i =0;i<4;i++)		//Lock_number[4]和Rfid_Buff[6]都是全局变量
		 PDA_RFID[i+6] =Rfid_Buff[i]; //在ST_Idle()中已经将数据放入Rfid_Buff[]	
		PDA_RFID[10]=PDA_RFID[0]^PDA_RFID[1]^PDA_RFID[2]^PDA_RFID[3]^PDA_RFID[4]^PDA_RFID[5]^PDA_RFID[6]^PDA_RFID[7]^PDA_RFID[8]^PDA_RFID[9];



		//传递RFID锁的编号,靠UART2串口中断，将RFID数据放到放到缓冲区，
		//再由read_RfidData将数据读入Rfid_Buff[],这个函数在ST_Idle（）中调用过
		IWDG_Feed();
	    
		OLED_Clear();
		OLED_ShowUnite(16,2,"ID:");
		OLED_ShowNumHex(48,2,Lock_number[0],2,16);
		OLED_ShowNumHex(64,2,Lock_number[1],2,16);
		OLED_ShowNumHex(80,2,Lock_number[2],2,16);
		OLED_ShowNumHex(96,2,Lock_number[3],2,16);		
		
	  //OLED_ShowUnite(16,4,"核对授权中");	
		nChar= check_lock_code();		//匹配就为1；不匹配就为0	
		//UART1nSendChar(1,&nChar,1);			 
		//UART1nSendChar(1,"22222",5);				  


		//nChar =1;		//只有读取的锁码在授权文件中，才返回1	
		if(nChar ==0)
		{
			OLED_ClearArea(0,4,128,6);
			OLED_ShowUnite(36,4,"未授权");						
			
			buzzer_unlock_fail();
			
			delay_ms(1500);
			
			unlock_result=2;							//返回授权中需要的状态结果			
			
		  save_rfid_result();             //将锁码及开锁状态写入存储芯片中
			
			g_State =ST_IDLE;
			unlock_result=2;		          //返回授权中需要的状态结果
			return;
		}		
		else if(nChar==1)
		{ 				
			IWDG_Feed();

			UART3nSendChar(3,PDA_RFID,11);
			
			buzzer_unlock_success();
			COIL_ACTUATE;
			
			OLED_ClearArea(0,4,128,6);			
			OLED_ShowUnite(36,4,"已授权");					
			
			delay_ms(1000);
			
			unlock_result=1;
			
			save_rfid_result();                        //将锁码及开锁状态写入存储芯片中
			
			COIL_NO_ACTUATE;//关闭SPX2941电源，即关闭钥匙动作过程
			
			//UART1nSendChar(1,"33333",5);						
			g_State =ST_IDLE;
			IWDG_Feed();
			
			UART2Reset(2);
			
			return;
		}
		
}








/*
*@功能：检查读取到的锁码是否在授权文件当中
*
*
*/
uint8_t check_lock_code(void)
{
		uint8_t i,j,total_row;
		uint8_t known_lock_number[4];
		uint32_t lock_number_addr,addr_end;
		uint8_t IN_AUTHOR_OFFSET=4;		//RFID在一条授权中的偏移


		addr_end= SPI_FLASH_ReadWord(AUTHOR_POINT);   		//在界面的采码功能里会改变采码指针的值
		
		total_row=(addr_end-AUTHOR_START)/AUTHOR_LEN;
		
// 		if(total_row==0)
// 		{
// 			UART1nSendChar(1,"没有授权文件",11);		//如果total_row=0，	
// 			return 0;
// 		}

		for(i=0;i<total_row;i++)		//
		{		
				lock_number_addr=AUTHOR_START+i*AUTHOR_LEN+IN_AUTHOR_OFFSET;
					

				SPI_FLASH_Read(known_lock_number,lock_number_addr,4);
				
				IWDG_Feed();
				
				if(Lock_number[0]==known_lock_number[0]&&Lock_number[1]==known_lock_number[1]&&Lock_number[2]==known_lock_number[2]&&Lock_number[3]==known_lock_number[3])
				{					
					SPI_FLASH_Read(User_Num,lock_number_addr-4,2);		//如果读到的RFID在授权中则记录下其用户编号			
					return 1;		//读到了就返回1
				}
		}
		User_Num[0]=0;
		User_Num[1]=0;
		return 0;

}



/*
*@功能：2字节用户编号+4字节RFID+6字节时间+1字节开锁结果，存入记录区
*
*				每条长度为13字节
*/
void save_rfid_result(void)  //操作钥匙
{
	uint32_t record_addr;
	uint32_t Lock_number_addr;
	uint32_t total_row;
	uint8_t  temp_Lock_number[4];
	
	 
	
	IWDG_Feed();
  record_addr= SPI_FLASH_ReadWord(RECORD_POINT);   //已存储的组数
	
	
	
	SPI_FLASH_Write(User_Num,record_addr,2);				 		//写查找到的授权的用户编号到记录中，如果没有找到则编号为零
	
	SPI_FLASH_Write(Lock_number,record_addr+2,4);		 		//写读到的RFID到记录中
	
	read_time();  																 			//读取实时时间,放到time_buff中
	SPI_FLASH_Write(time_buff,record_addr+6,6);			 		//写实时时间到time记录中

	SPI_FLASH_Write(&unlock_result,record_addr+12,1);		//写开锁结果到记录中

	//for debug	
	SPI_FLASH_Read(item,record_addr,13);
	UART1nSendChar(1,item,13);
	delay_ms(1000);		
	
	
	record_addr=record_addr+RECORD_LEN;   //每条授权RECORD_LEN=13字节
	//更改授权用户信息指针的位置
	SPI_FLASH_WriteWord(RECORD_POINT,record_addr);		


	//for debug
	point=SPI_FLASH_ReadWord(RECORD_POINT);		//RECORD_POINT=RECORD_START
	UART1SendwordHex(1, point);
	delay_ms(1000);	



	//要区分宏地址里存放的内容到底是表示地址还是数据
	total_row= SPI_FLASH_ReadWord(RECORD_ROW);   		
	total_row=total_row+1;		//授权信息条数加一
	SPI_FLASH_WriteWord(RECORD_ROW,total_row);					


	//for debug
	row=SPI_FLASH_ReadWord(RECORD_ROW);
	UART1SendwordHex(1, row);
	delay_ms(1000);


}



/*
*@功能：滴滴
*/
void buzzer_unlock_success(void)
{

	
	
	
	  uint16_t time=100*3;
	
		BUZZER_ON;
		delay_ms(time);
		BUZZER_OFF;
		//delay_ms(time);		
	
}



/*
*@功能：滴~
*/
void buzzer_unlock_fail(void)
{
		uint16_t time=100;
	
		BUZZER_ON;
		delay_ms(time);
		BUZZER_OFF;
		delay_ms(time);	
	
	
		BUZZER_ON;
		delay_ms(time);
		BUZZER_OFF;
		//delay_ms(time);	
	
	
}
