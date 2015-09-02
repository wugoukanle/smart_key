/**********************************************************************
*												smart key
*										 Module：通信协议模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-22 16:12     1.0             build this file 
*
* 模块功能：
*				规定了与上位机的通信格式
*				
* 说明：ROW <= (POINT-START)/LEN;因为中间有些可能被删掉了
*				
**********************************************************************/



#include "usb.h" 
#include "stm32f10x.h"
#include "spi_flash.h"
#include "timer.h"
#include "usart.h"
#include "Globle_Var.h"
#include "oled.h"
#include "delay.h"
#include "RTC_time.h"
#include "iwdg.h"
#include "bluetooth.h"


#define  M   173
#define  MAX_BUFF      520     //255
#define  WRITE_ZHI     40


extern unsigned char m_Uart_State;
extern unsigned char time_buff[6];
// unsigned char key_in_code[4];
// unsigned int  battery[20];
extern unsigned char Lock_number[4];


extern uint8_t Rfid_Buff[6]; 
extern uint8_t g_Buff[MAX_BUFF];
extern uint8_t g_State;



uint8_t a_f[24];









/***********************************************************
下面是协议部分
************************************************************/


/*
*@功能：检测USB是否成功的进行了连接
*       等待上位机发送
*       aa 75 0a  00  00 00 D5   的确认帧	  此时串口转接器和钥匙读卡器连接已经成功
*       最后一位为异或的值
*/
uint8_t  USB_Cheak_init(void)
{
	
	uint8_t nChar;
	uint8_t usb_code[6]={0x55,0x7A,0xBB,0x01,0x00,0x01};//下位机反复的向上位机发送此命令，用以建立初始化连接
	uint8_t usb_code2[6]={0x55,0x7A,0xCC,0x01,0x00,0x01};//下位机回复上位机的命令，原因是上位机需要
	uint8_t state =0;
	//IWDG_Feed();

	//for(i =0;i<20;i++)
	while(1)
	{
		
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,usb_code,6);	 			//串口发送字符串
			else
				UART3nSendChar(3,usb_code,6);				//通过蓝牙发送数据
		
		
			IWDG_Feed();
			
			g_WaitTimeOut = 0;
			SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut将g_WaitTimeOut置为true
			while( g_WaitTimeOut == 0 )  //等待返回的GPS头段的字节
			{
				IWDG_Feed();
			 
				if(bluetooth_uart_switch==0)
					nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//读取按协议格式发送过来的数据，并进行解析		
				else
					nChar =UART3ReadFrm(3,g_Buff,MAX_BUFF);	  //通过蓝牙读取数据			 
			 
			 
				if(nChar>=6&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0xaa&&g_Buff[3]==0x01&&g_Buff[4]==0x00&&g_Buff[5]==0x01)		//为什么是3？	3表示接收到上位机的返回命令的命令字
				{

					if(bluetooth_uart_switch==0)
						UART1nSendChar(1,usb_code2,6);	 			//串口发送字符串
					else
						UART3nSendChar(3,usb_code2,6);				//通过蓝牙发送数据					
					break;
				}		//没有检查其它的数据位和没有清接收缓冲区
			 
			 
				if((USB_CHEAK&(GPIO_Pin_1))==0)
					return state;
			 
			}		//程序还可能靠定时器超时退出循环
			 
			 
			 
			 //接受到上位机的数据之后，且没有超时
			 KillTimer(TIMER_TIMEOUT);
			 if( g_WaitTimeOut == 0)		//表示未超时退出，即表示收到了上位机的回复
			 {
					state =1;
					IWDG_Feed();
					return state;		//正确的出口应该在这里，只要某次尝试成功就返回state=1
			 }
			}
			//return state;		//20次尝试都失败了，此处state是0
}




void USB_transmit(void)
{
  uint8_t nChar =0;
	
	if(bluetooth_uart_switch==0)
	{
	
// 			if(USB_Cheak_init_flag==1)
// 			{
// 				OLED_Clear();
// 				OLED_ShowUnite(16,2,"初始化通信中..");			
// 				nChar =USB_Cheak_init();		//连接初始化
// 			}
		// 	else if(USB_Cheak_init_flag==0)
		// 	{
		// 		g_State=ST_IDLE;
		// 		return;
		// 	}
		// 	else
		// 		nChar=1;
			
			
// 			if(nChar ==1||(USB_Cheak_init_flag==0))		//表示初始化返回了正确值或者是非初始化标示
// 			{
				 OLED_Clear();
				 OLED_ShowUnite(16,2,"PC机通信中..");
				
				 Test_Usb_Data();
				
				 IWDG_Feed();
				 
				
				 OLED_Clear();
				 OLED_ShowUnite(32,2,"通信结束");
				 delay_ms(2000);
				 OLED_Clear();		

				 g_State =ST_IDLE;
				 return;
				
// 			}
// 			else if(nChar ==0)
// 			{
// 				IWDG_Feed();
// 				//MOS_OFF;  //关闭MAX3232电源
// 				g_State =ST_IDLE;
// 				return;
// 			}

	}
	else
	{
		
		
		
		Test_Bluetooth_Data();
		
		OLED_Clear();
		OLED_ShowUnite(16,2,"蓝牙通信结束");
// 		delay_ms(2000);
		OLED_Clear();		

		g_State =ST_IDLE;
		return;
	}
	
	
	
}






/*
*@功能：测试钥匙和PC机(或PAD)的通信
*/
void Test_Usb_Data(void)
{
	
	 uint8_t nChar,usb_bit;
   uint8_t order ;   //命令字
	 end_flag=0;			 //改为全局变量
	 usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
   while(usb_bit==1) 
   {
				IWDG_Feed();
				UART1Reset(0);		//复位串口接受缓冲区				 
				while(1)  				//等待返回的USb头段的字节
				{
					IWDG_Feed();
					
					
						nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//读取按协议格式发送过来的数据，并进行解析		
				
					
				
					if(nChar >=3)										//这里不用再判断头两个字符了
						break;
					if(end_flag==1)
						return;												//直接返回，此时不管USB_BIT是否已经拔出
					usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
					if(usb_bit==0)		//如果USB_BIT已经拔掉，就直接返回
						return;
				}
				
				
				order =g_Buff[2];  //取出来命令字
				
				switch(order)
				{
					case 0x01:register_key();break;
					case 0x02:mac_query();break;
					case 0x03:left_author_space();break;
					case 0x04:time_verify();break;
					
					case 0x05:authorization();break;
					case 0x06:back_record();break;
					
					//case 0x07:caima();break;
					//case 0x08:auto_back_caima();break;		//自动回传授权和自动回传采码都是钥匙主动的上发
					
					case 0x09:delete_device();break;
					
					//case 0x0a:download_xunjian();break;
					//case 0x0b:auto_back_xunjian();break;
					
					case 0x0c:clear_all_author();break;
					
					case 0x0d:pc_to_slave();break;
					
					case 0x0e:pc_end_com();break;
					
					case 0x0f:reset_flash();break;
					
					case 0x10:set_unlock_code();break;	
					
					default:break;
				}
				//UART1nSendChar(1,"11111",5);
	}
}


void Test_Bluetooth_Data(void)
{
	
	 uint8_t nChar;
   uint8_t order ;   //命令字
	
 	 uint8_t pc_to_slave_ack[8]={0x55,0x7A,0x0D,0x03,0x00,0x01,0x01,0x21};
// 	 UART3nSendChar(3,pc_to_slave_ack,8);
// 	 delay_ms(100);

// 	 UART3nSendChar(3,pc_to_slave_ack,8);
// 	 delay_ms(100);

// 	 UART3nSendChar(3,pc_to_slave_ack,8);
// 	 delay_ms(100);

	 end_flag=0;			 //改为全局变量
   while(1) 
   {
				//IWDG_Feed();
				//UART3Reset(0);		//复位串口接受缓冲区				 
				while(1)  				//等待返回的USb头段的字节
				{
					//IWDG_Feed();
// 				  UART3nSendChar(3,pc_to_slave_ack,8);
 				  delay_ms(100);					
					
					
				  m_UB3.RBuff;
					nChar =UART3ReadFrm(3,g_Buff,MAX_BUFF);	  //通过蓝牙读取数据						
					
				
					if(nChar>=3)										//这里不用再判断头两个字符了
						break;
					if(end_flag==1)						
						return;												//直接返回


				}
				
				
				order =g_Buff[2];  //取出来命令字
				
				switch(order)
				{
					case 0x01:register_key();break;
					case 0x02:mac_query();break;
					case 0x03:left_author_space();break;
					case 0x04:time_verify();break;
					
					case 0x05:authorization();break;
					case 0x06:back_record();break;
					
					//case 0x07:caima();break;
					//case 0x08:auto_back_caima();break;		//自动回传授权和自动回传采码都是钥匙主动的上发
					
					case 0x09:delete_device();break;
					
					//case 0x0a:download_xunjian();break;
					//case 0x0b:auto_back_xunjian();break;
					
					case 0x0c:clear_all_author();break;
					
					case 0x0d:pc_to_slave();break;
					
					case 0x0e:pc_end_com();break;
					
					case 0x0f:reset_flash();break;
					
					case 0x10:set_unlock_code();break;						
					default:break;
				}
				//UART1nSendChar(1,"11111",5);

	}
}




/*
*@功能：注册钥匙信息
*				0x01上位机传给下位机用户信息
*				10字节分组信息+8字节钥匙信息
*				这两个信息都在flash中有备份，关机后不丢失
*/
void register_key(void)
{
		uint8_t i,ID,receive_len;
		uint8_t xor =0;		      //校验和
		uint8_t mid_buff[8];
		uint32_t addr;
	  uint32_t row;
		
		uint8_t temp_group_inf[10]={0};
		uint8_t temp_key_inf[8]={0};
	
		ID=g_Buff[5];	//g_Buff是全局变量

	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x01;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =ID;	
	
		ID=g_Buff[5];
	
		receive_len=g_Buff[3]+5;		//表示整个命令的长度
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
		{
			
				SPI_FLASH_Write((g_Buff+6),GROUP_INFORMATION,10);
			
				//for debug
// 				SPI_FLASH_Read(temp_group_inf,GROUP_INFORMATION,10);
// 				UART1nSendChar(1,temp_group_inf,10);
// 				delay_ms(1000);
			
			
				SPI_FLASH_Write((g_Buff+16), KEY_INFORMATION, 8);
			
				//for dedug
// 				SPI_FLASH_Read(temp_key_inf,KEY_INFORMATION,8);
// 				UART1nSendChar(1,temp_key_inf,8);			
// 				delay_ms(1000);			
			
				mid_buff[6]=0x01;		//返回成功
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				IWDG_Feed();
			
				if(bluetooth_uart_switch==0)
					UART1nSendChar(1,mid_buff,8);	 //通过串口返回成功的确定码		
				else
					UART3nSendChar(3,mid_buff,8);	 //通过蓝牙返回成功的确定码							

		}
		else
		{

			mid_buff[6]= 0x00;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 //通过串口返回成功的确定码		
			else
				UART3nSendChar(3,mid_buff,8);	 //通过蓝牙返回成功的确定码	
		}	
	

}


/*
*@功能：0x02 MAC地址查询

*/
void mac_query(void)
{	
		uint8_t i=0,j=0,t=0,command_len,count=0;
		uint8_t xor =0;		//校验和
		const uint8_t buf_len=50;
	  const uint8_t try_num=3;
		uint8_t char_buf[buf_len];
		uint8_t MAC[6];
		uint8_t mid_buff[13];	
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x02;
		mid_buff[3] =0x08;		//长度
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];			//端口号
	
		command_len=g_Buff[3]+5;
		
		for(i =0;i<command_len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[command_len-1])		//判断接受命令校验是否正确
		{	
			
				if(mac_query_result)
				//if(bluetooth_mac())	
				{
						SPI_FLASH_Read(MAC,BLUETOOTH_MAC,6);
						for(i=0;i<8;i++)
							mid_buff[i+6]=MAC[i];
						
						xor=0;
						for(i =0;i<12;i++)
							xor =xor^mid_buff[i];	
						mid_buff[12]=xor;
						IWDG_Feed();

						
						if(bluetooth_uart_switch==0)
							UART1nSendChar(1,mid_buff,13);	 //通过串口返回MAC地址		
						else
							UART3nSendChar(3,mid_buff,13);	 //通过蓝牙返回MAC地址						
				}
				else
				{
						mid_buff[3]= 0x03;
						mid_buff[6]= 0x00;
						xor=0;
						xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
						mid_buff[7]=xor;
						
						IWDG_Feed();
					
						if(bluetooth_uart_switch==0)
							UART1nSendChar(1,mid_buff,8);	 	
						else
							UART3nSendChar(3,mid_buff,8);		
				}
			
			
			
// 				for(t=0;t<try_num;t++)				//至多尝试3次
// 				{
// 						UART3nSendChar(3,"TTM:MAC-?",10);		//蓝牙串口AT指令
// 						delay_ms(100);		//延时等待蓝牙回复
// 					
// 						count=UART3GetCount(3);
// 						UART3nReadChar(3,char_buf,count);			//读缓冲区数据到char_buf
// 					
// 						//for debug
//  						//UART1nSendChar(1,char_buf,count);		//发送读到的所有数据
//  						//delay_ms(100);					
// 					
// 						for(j=0;j<count-1;j++)		//从for循环退出的话，j=buf_len-1
// 							if(char_buf[j]=='0'&&char_buf[j+1]=='x')
// 									break;								//从break退出的话，j<buf_len-1
// 							

// 						if(j<count-1-12)							//如果提前成功
// 						{
// 							//for debug
//  							//UART1nSendChar(1,char_buf+j,12);		//已字符形式发送MAC地址
//  							//delay_ms(100);
// 							
// 							j=j+2;
// 							CharToHex(char_buf+j,MAC,12);		
// 							
// 							for(i=0;i<8;i++)
// 								mid_buff[i+6]=MAC[i];
// 							
// 							xor=0;
// 							for(i =0;i<12;i++)
// 								xor =xor^mid_buff[i];	
// 							mid_buff[12]=xor;
// 							IWDG_Feed();

// 							
// 							if(bluetooth_uart_switch==0)
// 								UART1nSendChar(1,mid_buff,13);	 //通过串口返回MAC地址		
// 							else
// 								UART3nSendChar(3,mid_buff,13);	 //通过蓝牙返回MAC地址								
// 							
// 							
// 							break;//不用再尝试了
// 							
// 						}
// 						
// 				}
// 				
// 				//失败
// 				if(t>=try_num)
// 				{
// 						mid_buff[3]= 0x03;
// 						mid_buff[6]= 0x00;
// 						xor=0;
// 						xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 						mid_buff[7]=xor;
// 						
// 						IWDG_Feed();
// 					
// 						if(bluetooth_uart_switch==0)
// 							UART1nSendChar(1,mid_buff,8);	 	
// 						else
// 							UART3nSendChar(3,mid_buff,8);					
// 					
// 					
// 				}

		}
		else
		{
			mid_buff[3]= 0x03;
			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);					
		}	
		
}


/*
*@功能：将字符转化为十六进制
				buffer为字符串地址，dest为十六进制数据地址，buf_len为buffer中字符的长度

        "0xB4994C861F26"的"B4994C861F26"部分的连续两个字符转化为一个数

*/
void CharToHex(uint8_t * buffer, uint8_t * dest, uint32_t buf_len)
{
	 uint32_t i=0;
	 uint32_t j=0;
	 uint8_t temp;
	 while(i<buf_len)
	 {
		temp=buffer[i];
		if((temp>=0x30)&&(temp<=0x39))				//temp在0与9之间
		{
			 temp=temp-0x30;
			 dest[j]=temp<<4;
		}
		else if((temp>=0x41)&&(temp<=0x46))		//temp在A和F之间
		{
			 temp=temp-0x41+0x0A;
			 dest[j]=temp<<4;
		}
		else if((temp>=0x61)&&(temp<=0x66))		//temp在a和f之间
		{
			 temp=temp-0x61+0x0A;
			 dest[j]=temp<<4;
		}
		else
		{
			 dest[j]=0x00;
		}
		
	

		temp=buffer[i+1];
		if((temp>=0x30)&&(temp<=0x39))				//temp在0与9之间
		{
			 temp=temp-0x30;
			 dest[j]=dest[j]|temp;
		}
		else if((temp>=0x41)&&(temp<=0x46))		//temp在A和F之间
		{
			 temp=temp-0x41+0x0A;
			 dest[j]=dest[j]|temp;
		}
		else if((temp>=0x61)&&(temp<=0x66))  //temp在a和f之间
		{
			 temp=temp-0x61+0x0A;
			 dest[j]=dest[j]|temp;
		}
		else
		{
			dest[j]=dest[j]|0x00;
		}
	
		i=i+2;
		j=j+1;
 }
 
 return;
}













/*
*@功能：0x03锁存储空间查询
*
*@说明：返回的是实际剩余可用授权的条数
*/
void left_author_space(void)
{		
		uint8_t i,receive_len;
		uint8_t mid_buff[11];
		uint16_t left_space;
	  //uint32_t addr;
		uint8_t xor =0;		//校验和
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x03;

		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];			
	
		receive_len=g_Buff[3]+5;
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
		{
				//addr= SPI_FLASH_ReadWord(AUTHOR_POINT);   

				//for debug
// 				UART1SendwordHex(1, addr);
// 				delay_ms(1000);			
			
			
				//left_space=(AUTHOR_END-addr)/AUTHOR_LEN;

				left_space=(AUTHOR_MAX_NUM-SPI_FLASH_ReadWord(AUTHOR_ROW));		//返回剩余空间的字节数,这样的空间更准确（考虑到删除的授权）
				
				//for debug
// 			  row=SPI_FLASH_ReadWord(AUTHOR_ROW);
// 				UART1SendwordHex(1,row);
// 				delay_ms(1000);			
			
			
				mid_buff[3] =0x04;
			
				mid_buff[6]= (left_space&0xff00)>>8;		
				mid_buff[7]= (left_space&0x00ff);		

				xor=0;
				for(i =0;i<8;i++)
					xor =xor^mid_buff[i];
				mid_buff[8]=xor;
			
				IWDG_Feed();
				
				if(bluetooth_uart_switch==0)
					UART1nSendChar(1,mid_buff,9);	 	
				else
					UART3nSendChar(3,mid_buff,9);							
				
				
				
	  }
		else
		{

			IWDG_Feed();
			
			mid_buff[3] =0x03;
			
			mid_buff[6]= 0x00;
			
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);						
			
		}	
}

/*
*@功能：0x04对电子钥匙进行校时
*				6个字节的时间（YYMMDDHHMMSS）
*/
void time_verify(void)
{
  uint8_t time_date[6];
  uint8_t mid_buff[13];
  uint8_t i,k,receive_len;
  uint8_t xor =0;		//校验和
  IWDG_Feed();
  mid_buff[0] =0x55;		//表示下位机发给上位机
  mid_buff[1] =0x7a;
  mid_buff[2] =0x04;		//命令字

  mid_buff[4] =0x00;
  mid_buff[5] =0x01;		//ID
  IWDG_Feed();
	
	receive_len=g_Buff[3]+5;
	
	for(i =0;i<receive_len-1;i++)
		xor =xor^g_Buff[i];	
	if(xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
	{
			for(i =0;i<6;i++)
				time_date[i] =g_Buff[6+i];		//g_Buff[7~12] 6个数组单元
			
			set_time(time_date[0],time_date[1],time_date[2],time_date[3],time_date[4],time_date[5]);//根据上位机的时间设置钥匙的时间
			delay_ms(500);
			IWDG_Feed();
			read_time();    //返回实时的时间,存于time_buff中
			for( i =0;i<6;i++)
				mid_buff[6+i] =time_buff[i];
		  mid_buff[3] =0x08;		//长度
			xor=0;
			for(i =0;i<12;i++)
				xor =xor^mid_buff[i];
			mid_buff[12]=xor;
			IWDG_Feed();
		
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,13);	 	
			else
				UART3nSendChar(3,mid_buff,13);					
		
		
		
	}
	else
	{
		for(k =0;k<WRITE_ZHI;k++)
			delay_ms(500);
		IWDG_Feed();
		mid_buff[3] =0x03;		//长度
		mid_buff[6]= 0x00;
		xor=0;
		xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
		mid_buff[7]=xor;
		
		IWDG_Feed();
		
		if(bluetooth_uart_switch==0)
			UART1nSendChar(1,mid_buff,8);	 	
		else
			UART3nSendChar(3,mid_buff,8);					
		
		
		
	}
}

/*
*@功能：0x05将授权文件的信息写入EEPROM
*				每次写一条授权文件
*				回复接受成功命令
*				2个字节用户编号+2字节锁编号 +4字节锁RFID
*/
void authorization(void)
{
		uint8_t data_len,i,receive_len;
		uint8_t xor=0;
		uint8_t mid_buff[8];
		uint32_t addr,total_row;
		data_len=g_Buff[3];		//g_Buff是全局变量
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x05;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];
	
		receive_len=g_Buff[3]+5;
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
		{	
			
				addr= SPI_FLASH_ReadWord(AUTHOR_POINT);  //初始化 AUTHOR_POINT=AUTHOR_START
				IWDG_Feed();
		
				SPI_FLASH_Write((g_Buff+6),addr,(data_len-2));//怎么传进来的就怎么放
			
				//for debug
// 				SPI_FLASH_Read(item,addr,25);
// 			  UART1nSendChar(1,item,25);
// 				delay_ms(1000);			

			
			
				

							
				addr=addr+AUTHOR_LEN;   //每条授权AUTHOR_LEN=25字节
				//更改授权用户信息指针的位置
				SPI_FLASH_WriteWord(AUTHOR_POINT,addr);		
				
				//for debug
// 				point=SPI_FLASH_ReadWord(AUTHOR_POINT);		//AUTHOR_POINT=AUTHOR_START;AUTHOR_START=0x080000
// 				UART1SendwordHex(1, point);
// 				delay_ms(1000);


				
				//要区分宏地址里存放的内容到底是表示地址还是数据
				total_row= SPI_FLASH_ReadWord(AUTHOR_ROW);   		
				total_row=total_row+1;		//授权信息条数加一
				SPI_FLASH_WriteWord(AUTHOR_ROW,total_row);				
				 
				//for debug
// 				row=SPI_FLASH_ReadWord(AUTHOR_ROW);
// 				UART1SendwordHex(1, row);
// 				delay_ms(1000);
	
				
				mid_buff[6]= 0x01;
				xor=0;
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				
				IWDG_Feed();
				
				if(bluetooth_uart_switch==0)
					UART1nSendChar(1,mid_buff,8);	 	
				else
					UART3nSendChar(3,mid_buff,8);							
				
				
		}
		else
		{

			IWDG_Feed();

			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);						
			
		}
		
}





/*
*@功能：回传记录
*				0x06	发送所有操作记录信息,主要是开锁状态；
*				每帧: 55 +7A +06+0F(数据长度)+00+ID+2字节用户编号+4个字节RFID+6字节时间+1字节开锁状态+1字节校验位
*
*				2字节用户编号+4个字节RFID+6字节时间+1字节开锁状态
*/

void back_record(void)
{
		uint8_t i,m,ID,nChar,receive_len;
	  const uint8_t send_len=13+2+5;
  	uint8_t xor=0;
		uint8_t mid_buff[send_len];
		uint32_t addr,start_addr;
		uint32_t total_row;
		uint8_t record_end_cmd[8]={0x55,0x7A,0x06,0x03,0x00,0x01,0x00,0x2B};
		uint8_t slave_to_host_ACK[8]={0x55,0x7A,0x06,0x03,0x00,0x01,0x01,0x2A};
	
		ID=0x01;//假设ID等于01
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x06;

		mid_buff[4] =0x00;
		mid_buff[5] =ID;		
			
	
	
		receive_len=g_Buff[3]+5;		//命令长度为7
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[receive_len-1]&&receive_len==7)		//判断接受命令校验是否正确,并且命令长度为7，确定命令长度为8
		{			
			
				addr = SPI_FLASH_ReadWord(RECORD_POINT);  		//RECORD_POINT=RECORD_START
				
				//for debug
// 				UART1SendwordHex(1, addr);
// 				delay_ms(1000);
				////OLED_ShowNumHex(0,6,addr,8,16);
			
				total_row=(addr-RECORD_START)/RECORD_LEN;		//每条授权25字节
			
				//for debug
// 				UART1SendwordHex(1,total_row);				
// 				delay_ms(1000);		
// 				UART1SendChar(1,'\r');
// 				UART1SendChar(1,'\n');				
// 				delay_ms(1000);
			
				if(total_row==0)
				{
						IWDG_Feed();

					
						if(bluetooth_uart_switch==0)
							UART1nSendChar(1,record_end_cmd,8);	 	
						else
							UART3nSendChar(3,record_end_cmd,8);								

				}
				else
				{
						for(i=0;i<total_row;i++)		//此时i>=1
						{
							
								start_addr=RECORD_START+i*RECORD_LEN;		
							  
								//for debug
// 								UART1SendwordHex(1,start_addr);				
// 								delay_ms(1000);		
							
								SPI_FLASH_Read(mid_buff+6,start_addr,13);					
							
								
								mid_buff[3] =send_len-5;		//命令长度为7
								xor=0;
								for(m =0;m<send_len-1;m++)
									xor =xor^mid_buff[m];						
								mid_buff[send_len-1]=xor;
															
								//发送第一条记录
								if(bluetooth_uart_switch==0)
									UART1nSendChar(1,mid_buff,send_len);	 	
								else
									UART3nSendChar(3,mid_buff,send_len);								
							

								IWDG_Feed();
								UART1Reset(0);		//复位串口接受缓冲区										
								while(1)  //发送完一条授权之后必须等待上位机的返回命令
								{
//											UART1Reset(0);		//复位串口接受缓冲区		(串口复位不能放在这里)										
									
// 											g_WaitTimeOut = 0;
// 											SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut将g_WaitTimeOut置为true
// 											while( g_WaitTimeOut == 0 );
// 											KillTimer(TIMER_TIMEOUT);		
									
											//delay_ms(500);									
									
											if(bluetooth_uart_switch==0)
												nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  读取按协议格式发送过来的数据，并进行解析		
											else
												nChar =UART3ReadFrm(3,g_Buff,MAX_BUFF);	  //通过蓝牙读取数据										
									
									
									
									
											//如果没有数据则nChar=0
																												
											if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xDA)//如果返回0x01就发下一条授权
											{
												IWDG_Feed();
												//UART1nSendChar(1,"接受成功",5);	
												
												//for debug
// 				 								UART1SendwordHex(1,i);				
// 				 								delay_ms(1000);													
												
												
												if(i==total_row-1)		//如果这是最后一条命令，则没有记录可发，直接发结束命令
												{
													
														if(bluetooth_uart_switch==0)
															UART1nSendChar(1,record_end_cmd,8);	 	
														else
															UART3nSendChar(3,record_end_cmd,8);													
													
													
														return;
												}
												
												break;		//只是跳出内部等待循环，发送下一条授权
											}	
											else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x00&&g_Buff[7]==0xDB)//如果返回0x01就删除这条授权，再发下一条
											{


												i=i-1;		//跳出内部等待循环之后，会直接再跳出发送循环，直接结束整个自动回传
												break;
// 												IWDG_Feed();
// 												UART1nSendChar(1,"接受失败",5);
												
// 												;		//什么也不做继续等待
											}
											else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x02&&g_Buff[7]==0xDB)//如果返回0x01就删除这条授权，再发下一条
											{
												if(bluetooth_uart_switch==0)
													UART1nSendChar(1,slave_to_host_ACK,8);	 	
												else
													UART3nSendChar(3,slave_to_host_ACK,8);	
												return;
// 												IWDG_Feed();
// 												UART1nSendChar(1,"直接结束",5);
											
// 												;		//什么也不做继续等待
											}											

								}//内部while循环结束

						 }//for循环结束
				}//else结束
		}
		else
		{
	
			mid_buff[3] =0x03;
			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			UART1nSendChar(1,mid_buff,8);	 //返回失败码		
		}
}






/*
*@功能：0x07设备采码
*				2个字节员工编号+2字节锁编号 
*/
// void caima(void)
// {
// 		uint8_t data_len,i,total_row,receive_len;
// 		uint8_t xor;
// 		uint8_t mid_buff[8];
// 		uint8_t addr;
// 		data_len=g_Buff[3];		//g_Buff是全局变量
// 	
// 		mid_buff[0] =0x55;
// 		mid_buff[1] =0x7a;
// 		mid_buff[2] =0x07;
// 		mid_buff[3] =0x06;
// 		mid_buff[4] =0x00;
// 		mid_buff[5] =g_Buff[5];		

// 		
// 		receive_len=g_Buff[3]+5;
// 		
// 		for(i =0;i<receive_len-1;i++)
// 			xor =xor^g_Buff[i];	
// 	  if(xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
// 		{	

// 				addr= SPI_FLASH_ReadWord(CAIMA_POINT);   
// 		
// 				SPI_FLASH_Write((g_Buff+6),addr,(data_len-2));
// 				
// 				
// 				addr=addr+CAIMA_LEN;   //采码区每项5个字节
// 				//更改授权用户信息指针的位置
// 				IWDG_Feed();
// 				SPI_FLASH_WriteWord(CAIMA_POINT,addr);		

// 				
// 				
// 				//要区分宏地址里存放的内容到底是表示地址还是数据
// 				total_row= SPI_FLASH_ReadWord(CAIMA_ROW);   		
// 				total_row=total_row+1;
// 				SPI_FLASH_WriteWord(CAIMA_ROW,total_row);				//CAIMA_MAX_NUM		52428
// 				
// 				
// 				mid_buff[6]= 0x01;
// 				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 				mid_buff[7]=xor;
// 				
// 				IWDG_Feed();
// 				UART1nSendChar(1,mid_buff,8);	 //返回确定码
// 				
// 		}		
// 		else
// 		{
// 			
// 				mid_buff[6]= 0x00;
// 				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 				mid_buff[7]=xor;
// 				
// 				IWDG_Feed();
// 				UART1nSendChar(1,mid_buff,8);	 //返回失败码		
// 		}

// }





/*
*@功能：0x08自动回传采码
*				2个字节员工编号+2字节锁编号 +4个字节的锁码
*				这4个字节的锁码是钥匙采码时新增的
*/
// void auto_back_caima(void)
// {
// 		uint8_t i,j,k,m,ID,total_row,nChar,receive_len;
// 		uint8_t xor=0;
// 		uint8_t mid_buff[15];
// 		uint8_t addr,start_addr;
// 		
// 		ID=0x01;//假设ID等于01
// 		
// 		mid_buff[0] =0x55;
// 		mid_buff[1] =0x7a;
// 		mid_buff[2] =0x08;

// 		mid_buff[4] =0x00;
// 		mid_buff[5] =ID;		
// 			
// 		//total_row= ReadData(AUTHOR_ROW_L);
// 		receive_len=g_Buff[3]+5;
// 		
// 		for(m =0;m<receive_len-1;m++)
// 			xor =xor^g_Buff[m];	
// 	  if(receive_len==7&&xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确,将这条命令与其确定命令区别开来
// 		{		

// 				addr= SPI_FLASH_ReadWord(CAIMA_POINT);   
// 				
// 				total_row=(addr-CAIMA_START)/8;
// 				
// 			  if(total_row==0)
// 					 UART1nSendChar(1,"00000",5);		//如果total_row=0，

// 				for(i=0;i<total_row;i++)
// 				{
// 						start_addr=CAIMA_START+i*8;
// 						
// 						for(j=0;j<8;j++)		//8个字节：用户编号、锁编号、锁码
// 							mid_buff[6+j]= ReadData(start_addr+j);
// 					
// 						mid_buff[3] =0x0a;		
// 						xor=0;
// 						for(m =0;m<14;m++)
// 							xor =xor^mid_buff[m];	
// 						mid_buff[14] =xor;	
// 					
// 						delay_ms(500);
// 						IWDG_Feed();
// 						UART1nSendChar(1,mid_buff,15);	 //发送一条授权文件
// 						
// 						for(k =0;k<WRITE_ZHI;k++)
// 							delay_ms(500);
// 						IWDG_Feed();

// 						UART1Reset(0);		//复位串口接受缓冲区				 
// 						while(1)  
// 						{
// 							IWDG_Feed();
// 							nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  
// 							if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x08&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xD4)
// 								break;		//如果收到确认，结束等待，传下一条采码数据
// 							//UART1nSendChar(1,&total_row,1);
// 						}
// 				}
// 		}		
// 		else
// 		{

// 			IWDG_Feed();
// 	  	mid_buff[3] =0x03;
// 			mid_buff[6]= 0x00;
// 			xor=0;
// 			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 			mid_buff[7]=xor;
// 			
// 			IWDG_Feed();
// 			UART1nSendChar(1,mid_buff,8);	 //返回失败码		
// 		}
// }



/*
*@功能：0x09删除设备，即删除锁
*				2字节锁编
*@说明：被删除的授权全为1，在查找授权时是不会匹配的，因为没有RFID全为1
*/
void delete_device(void)
{
		uint8_t i,ID,k,receive_len;
		uint8_t xor=0;
		uint8_t mid_buff[8];
		uint8_t keycode[2];
		uint8_t clear_author[25]={
			0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff,
			0xff,0xff,0xff,0xff,0xff};
		uint32_t addr,temp_addr,known_max_row,total_row;
		
	
		ID=g_Buff[5];
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x09;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =ID;		
	
		receive_len=g_Buff[3]+5;
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
		{
				temp_addr= SPI_FLASH_ReadWord(AUTHOR_POINT);   
			
				known_max_row=(temp_addr-AUTHOR_START)/AUTHOR_LEN;		//只需查AUTHOR_START~AUTHOR_POINT之间的所有授权，其中删除的授权全部为0
			
			
				addr=AUTHOR_START;
				for(i=0;i<known_max_row;i++)		//其实可以查完所有的授权
				{
						SPI_FLASH_Read(keycode,addr+2,2);
					
						//for debug
// 						UART1nSendChar(1,keycode,2);
// 						delay_ms(1000);					
					
					
						if(keycode[0]==g_Buff[6]&&keycode[1]==g_Buff[7])		//根据锁编号查找用户，且该用户没有被删除
								break;
						
						addr=addr+AUTHOR_LEN;		//当前编号不匹配，检查下一条授权
				}
				
				
				
				
				if(i<known_max_row)		//如果找到对应的锁编号
				{
					
						SPI_FLASH_Write(clear_author,addr,AUTHOR_LEN);		//将找到的授权全部置1
					

						total_row= SPI_FLASH_ReadWord(AUTHOR_ROW);   			//更新实际的行数
						total_row=total_row-1;
						SPI_FLASH_WriteWord(AUTHOR_ROW,total_row);			
					
	
						mid_buff[6]= 0x01;
						xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
						mid_buff[7]=xor;
						IWDG_Feed();

						if(bluetooth_uart_switch==0)
							UART1nSendChar(1,mid_buff,8);	 	
						else
							UART3nSendChar(3,mid_buff,8);		
					
				} 
				else		//没查到对应的锁编号
				{

						mid_buff[6]= 0x00;
						xor=0;
						xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
						mid_buff[7]=xor;
						IWDG_Feed();
						if(bluetooth_uart_switch==0)
							UART1nSendChar(1,mid_buff,8);	 	
						else
							UART3nSendChar(3,mid_buff,8);							
				}
		}
		else		//命令校验不正确
		{
			for(k =0;k<WRITE_ZHI;k++)
				delay_ms(500);
			IWDG_Feed();

			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);			
		}
}




/*
0x0a上位机传给下位机巡检任务
2字节锁编号+2字节巡检任务编号+14个字节具体任务 +1字节巡检结果
*/
// void download_xunjian(void)
// {
// 		uint8_t data_len,i,total_row,receive_len;
// 		uint8_t xor=0;
// 		uint8_t mid_buff[8];
// 		uint8_t addr;
// 		data_len=g_Buff[3];		//g_Buff是全局变量

// 		mid_buff[0] =0x55;
// 		mid_buff[1] =0x7a;
// 		mid_buff[2] =0x0a;
// 		mid_buff[3] =0x03;
// 		mid_buff[4] =0x00;
// 		mid_buff[5] =g_Buff[5];		
// 	
// 		receive_len=g_Buff[3]+5;
// 		
// 		for(i =0;i<receive_len-1;i++)
// 			xor =xor^g_Buff[i];	
// 	  if(receive_len>8&&xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
// 		{
// 			addr= SPI_FLASH_ReadWord(XUNJIAN_POINT);   
//       //怎么传进来的就怎么放,注意是按数据长度来放的
// 			SPI_FLASH_Write((g_Buff+6),addr,(data_len-2));
// 			
// 			
// 			addr=addr+19;			 //每一条巡检任务19个字节			
// 			//更改巡检小任务指针的位置
// 			IWDG_Feed();
// 			SPI_FLASH_WriteWord(XUNJIAN_POINT,addr);		//#define AUTHOR_START  0x0b4f 
// 		
// 			
// 		
// 			//要区分宏地址里存放的内容到底是表示地址还是数据
// 			//由于巡检小任务条数最多只有53条，所以只用总行数的低8位
// 			IWDG_Feed();			
// 			total_row= SPI_FLASH_ReadWord(XUNJIAN_ROW) ;   		
// 			total_row=total_row+1;		//巡检小任务条数加一
// 			SPI_FLASH_WriteWord(XUNJIAN_ROW,total_row);				
// 			

// 			IWDG_Feed();
// 	
// 			mid_buff[6]= 0x01;
// 			xor=0;
// 			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 			mid_buff[7]=xor;
// 			IWDG_Feed();
// 			UART1nSendChar(1,mid_buff,8);	 //返回确定码
// 		}
// 		else
// 		{
// 			mid_buff[6]= 0x00;
// 			xor=0;
// 			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 			mid_buff[7]=xor;
// 			
// 			IWDG_Feed();
// 			UART1nSendChar(1,mid_buff,8);	 //返回失败码		
// 		}
// 	
// }




/*
0xob自动回传巡检任务结果
2字节锁编号+2字节巡检任务编号+0x22(具体任务起始码)+巡检结果 +0x23(具体任务终止码) 
*/
// void auto_back_xunjian(void)
// {
// 		uint8_t i,k,m,total_row,nChar,ID,receive_len;
// 		uint8_t xor=0;
// 		uint8_t mid_buff[12];
// 		uint8_t addr,start_xunjian;
// 	
// 		ID=g_Buff[5];
// 	
// 		mid_buff[0] =0x55;
// 		mid_buff[1] =0x7a;
// 		mid_buff[2] =0x0b;

// 		mid_buff[4] =0x00;
// 		mid_buff[5] =g_Buff[5];		
// 	
// 		receive_len=g_Buff[3]+5;
// 		
// 		for(i =0;i<receive_len-1;i++)
// 			xor =xor^g_Buff[i];	
// 	  if(receive_len==7&&xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
// 		{
// // 				addr= ReadData(XUNJIAN_POINT_H);   
// // 				addr =((addr<<8)&0xff00)+ReadData(XUNJIAN_POINT_L);
// // 				IWDG_Feed();
// // 				delay_ms(500);
// 			
// 				addr= SPI_FLASH_ReadWord(XUNJIAN_POINT);   
// 				total_row=(addr-XUNJIAN_START)/XUNJIAN_LEN;			
// 			
// 				//UART1nSendChar(1,&total_row,1);
// 			
// 				//total_row= SPI_FLAH_Read(XUNJIAN_ROW);   
// 				IWDG_Feed();
// 				delay_ms(500);	
// 			
// 				for(i=0;i<total_row;i++)		//
// 				{		
// 					start_xunjian=XUNJIAN_START+i*XUNJIAN_LEN;
// 					
// 				
// 					SPI_FLASH_Read(mid_buff+6,start_xunjian,4);
// 					
// 					
// 					delay_ms(500);
// 					SPI_FLASH_Read(mid_buff+receive_len-2,start_xunjian+18,1);		//取巡检小任务末尾的巡检结果
// 					
// 					xor=0;
// 					for(m =0;m<11;m++)
// 						xor =xor^mid_buff[m];	
// 					mid_buff[11]=xor;

// 					delay_ms(500);
// 					IWDG_Feed();
// 					UART1nSendChar(1,mid_buff,12);	 //发送巡检结果				
// 					
// 					IWDG_Feed();
// 					UART1Reset(0);		//复位串口接受缓冲区				 
// 					while(1)  //发送完一条授权之后必须等待上位机的返回命令
// 					{
// 						
// 						for(k =0;k<WRITE_ZHI;k++)
// 								delay_ms(500);
// 						IWDG_Feed();
// 						nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  
// 						
// 						if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0b&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xD7)//如果返回0x01就发下一条授权
// 								break;		//只是跳出内部等待循环
// 						else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0b&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x00&&g_Buff[7]==0xD6)
// 						{
// 								i--;		//相当于重发
// 								break;
// 						}
// 			
// 					}
// 				}
// 	  }
// 		else
// 		{
// 			
// 			mid_buff[3] =0x03;
// 			mid_buff[6]= 0x00;
// 			xor=0;
// 			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 			mid_buff[7]=xor;
// 			
// 			IWDG_Feed();
// 			UART1nSendChar(1,mid_buff,8);	 //返回失败码		
// 		}
// } 


/*
oxoc清除所有授权
*/
void clear_all_author(void)
{
		uint8_t i,receive_len;
	  uint8_t xor=0;
		uint8_t mid_buff[8];
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0c;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];	
	
		receive_len=g_Buff[3]+5;
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
	
	  if(xor==g_Buff[receive_len-1])		//判断接受命令校验是否正确
		{

			IWDG_Feed();
			
			
			SPI_FLASH_WriteWord(AUTHOR_ROW,0x00);		 //授权信息行数为0

			SPI_FLASH_WriteWord(AUTHOR_POINT,AUTHOR_START);		//授权指针指向授权区首地址


			mid_buff[6]= 0x01;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);		

		}
		else
		{

			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);			
		}


}



/*
*@功能：PC主动连接钥匙
*/
void pc_to_slave(void)
{
		uint8_t i,xor,receive_len;
		uint8_t mid_buff[8];
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0d;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		receive_len=g_Buff[3]+5;
		
		xor=0;
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
		
		if(xor==g_Buff[6])
		{
			if(bluetooth_uart_switch==1)
			{
				 OLED_Clear();
				 OLED_ShowUnite(16,2,"蓝牙通信中");
			}
				
			//delay_ms(200);
			
			mid_buff[6]= 0x01;	
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  //IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
			{
				delay_ms(500);
				UART3nSendChar(3,mid_buff,8);
			}
		}
	  else
		{
			if(bluetooth_uart_switch==1)
			{
				 OLED_Clear();
				 OLED_ShowUnite(16,2,"蓝牙连接失败");
			}			
			
			
			mid_buff[6]= 0x00;	
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);					
		}

}

/*
*@功能：PC主动结束通信
*/
void pc_end_com(void)
{
		uint8_t i,xor,receive_len;
		uint8_t mid_buff[8];
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0e;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		receive_len=g_Buff[3]+5;
		
		xor=0;
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
		
		if(xor==g_Buff[6])
		{
			
			end_flag=1;		//通信结束，直接退出等待命令状态
			
			mid_buff[6]= 0x01;	//表示成功
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);		
			
			
			
			USB_Cheak_init_flag=0;		//表示非初始化链接状态
		}
	  else
		{
			mid_buff[6]= 0x00;	//表示失败
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);					
		}
		
}




/*
*@功能：发送此条命令清FLASH
*/
void reset_flash(void)
{
		uint8_t i,xor,receive_len;
		uint8_t mid_buff[8];
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x0f;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		receive_len=g_Buff[3]+5;
		
		xor=0;
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
		
		if(xor==g_Buff[6])
		{
			
			mykey_init();
			
			mid_buff[6]= 0x01;	//表示成功
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);		
			
			
			USB_Cheak_init_flag=0;		//表示非初始化链接状态
		}
	  else
		{
			mid_buff[6]= 0x00;	//表示失败
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);					
		}		

}




/*
*@功能：发送此条命令设置开锁密码
*/
void set_unlock_code(void)
{
		uint8_t i,xor,receive_len;
		uint8_t mid_buff[8];
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x10;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];		

		receive_len=g_Buff[3]+5;
		
		xor=0;
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
		
		if(xor==g_Buff[receive_len-1])
		{
			
			SPI_FLASH_Write(g_Buff+6,UNLOCK_CODE,4);
			
			//for debug
// 			SPI_FLASH_Read(item,UNLOCK_CODE,4);
// 			UART1nSendChar(1,item,4);
// 			delay_ms(1000);		

			
					
			mid_buff[6]= 0x01;	//表示成功
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);		
			
			

		}
	  else
		{
			mid_buff[6]= 0x00;	//表示失败
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);					
		}		

}






















/*
开锁状态函数
返回状态
*/
//uint8_t key_state(void)
//{
//		return 1;

//}


