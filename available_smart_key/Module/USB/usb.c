/**********************************************************************
*												smart key
*										 Module��ͨ��Э��ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-22 16:12     1.0             build this file 
*
* ģ�鹦�ܣ�
*				�涨������λ����ͨ�Ÿ�ʽ
*				
* ˵����ROW <= (POINT-START)/LEN;��Ϊ�м���Щ���ܱ�ɾ����
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
������Э�鲿��
************************************************************/


/*
*@���ܣ����USB�Ƿ�ɹ��Ľ���������
*       �ȴ���λ������
*       aa 75 0a  00  00 00 D5   ��ȷ��֡	  ��ʱ����ת������Կ�׶����������Ѿ��ɹ�
*       ���һλΪ����ֵ
*/
uint8_t  USB_Cheak_init(void)
{
	
	uint8_t nChar;
	uint8_t usb_code[6]={0x55,0x7A,0xBB,0x01,0x00,0x01};//��λ������������λ�����ʹ�������Խ�����ʼ������
	uint8_t usb_code2[6]={0x55,0x7A,0xCC,0x01,0x00,0x01};//��λ���ظ���λ�������ԭ������λ����Ҫ
	uint8_t state =0;
	//IWDG_Feed();

	//for(i =0;i<20;i++)
	while(1)
	{
		
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,usb_code,6);	 			//���ڷ����ַ���
			else
				UART3nSendChar(3,usb_code,6);				//ͨ��������������
		
		
			IWDG_Feed();
			
			g_WaitTimeOut = 0;
			SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut��g_WaitTimeOut��Ϊtrue
			while( g_WaitTimeOut == 0 )  //�ȴ����ص�GPSͷ�ε��ֽ�
			{
				IWDG_Feed();
			 
				if(bluetooth_uart_switch==0)
					nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//��ȡ��Э���ʽ���͹��������ݣ������н���		
				else
					nChar =UART3ReadFrm(3,g_Buff,MAX_BUFF);	  //ͨ��������ȡ����			 
			 
			 
				if(nChar>=6&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0xaa&&g_Buff[3]==0x01&&g_Buff[4]==0x00&&g_Buff[5]==0x01)		//Ϊʲô��3��	3��ʾ���յ���λ���ķ��������������
				{

					if(bluetooth_uart_switch==0)
						UART1nSendChar(1,usb_code2,6);	 			//���ڷ����ַ���
					else
						UART3nSendChar(3,usb_code2,6);				//ͨ��������������					
					break;
				}		//û�м������������λ��û������ջ�����
			 
			 
				if((USB_CHEAK&(GPIO_Pin_1))==0)
					return state;
			 
			}		//���򻹿��ܿ���ʱ����ʱ�˳�ѭ��
			 
			 
			 
			 //���ܵ���λ��������֮����û�г�ʱ
			 KillTimer(TIMER_TIMEOUT);
			 if( g_WaitTimeOut == 0)		//��ʾδ��ʱ�˳�������ʾ�յ�����λ���Ļظ�
			 {
					state =1;
					IWDG_Feed();
					return state;		//��ȷ�ĳ���Ӧ�������ֻҪĳ�γ��Գɹ��ͷ���state=1
			 }
			}
			//return state;		//20�γ��Զ�ʧ���ˣ��˴�state��0
}




void USB_transmit(void)
{
  uint8_t nChar =0;
	
	if(bluetooth_uart_switch==0)
	{
	
// 			if(USB_Cheak_init_flag==1)
// 			{
// 				OLED_Clear();
// 				OLED_ShowUnite(16,2,"��ʼ��ͨ����..");			
// 				nChar =USB_Cheak_init();		//���ӳ�ʼ��
// 			}
		// 	else if(USB_Cheak_init_flag==0)
		// 	{
		// 		g_State=ST_IDLE;
		// 		return;
		// 	}
		// 	else
		// 		nChar=1;
			
			
// 			if(nChar ==1||(USB_Cheak_init_flag==0))		//��ʾ��ʼ����������ȷֵ�����Ƿǳ�ʼ����ʾ
// 			{
				 OLED_Clear();
				 OLED_ShowUnite(16,2,"PC��ͨ����..");
				
				 Test_Usb_Data();
				
				 IWDG_Feed();
				 
				
				 OLED_Clear();
				 OLED_ShowUnite(32,2,"ͨ�Ž���");
				 delay_ms(2000);
				 OLED_Clear();		

				 g_State =ST_IDLE;
				 return;
				
// 			}
// 			else if(nChar ==0)
// 			{
// 				IWDG_Feed();
// 				//MOS_OFF;  //�ر�MAX3232��Դ
// 				g_State =ST_IDLE;
// 				return;
// 			}

	}
	else
	{
		
		
		
		Test_Bluetooth_Data();
		
		OLED_Clear();
		OLED_ShowUnite(16,2,"����ͨ�Ž���");
// 		delay_ms(2000);
		OLED_Clear();		

		g_State =ST_IDLE;
		return;
	}
	
	
	
}






/*
*@���ܣ�����Կ�׺�PC��(��PAD)��ͨ��
*/
void Test_Usb_Data(void)
{
	
	 uint8_t nChar,usb_bit;
   uint8_t order ;   //������
	 end_flag=0;			 //��Ϊȫ�ֱ���
	 usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
   while(usb_bit==1) 
   {
				IWDG_Feed();
				UART1Reset(0);		//��λ���ڽ��ܻ�����				 
				while(1)  				//�ȴ����ص�USbͷ�ε��ֽ�
				{
					IWDG_Feed();
					
					
						nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//��ȡ��Э���ʽ���͹��������ݣ������н���		
				
					
				
					if(nChar >=3)										//���ﲻ�����ж�ͷ�����ַ���
						break;
					if(end_flag==1)
						return;												//ֱ�ӷ��أ���ʱ����USB_BIT�Ƿ��Ѿ��γ�
					usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
					if(usb_bit==0)		//���USB_BIT�Ѿ��ε�����ֱ�ӷ���
						return;
				}
				
				
				order =g_Buff[2];  //ȡ����������
				
				switch(order)
				{
					case 0x01:register_key();break;
					case 0x02:mac_query();break;
					case 0x03:left_author_space();break;
					case 0x04:time_verify();break;
					
					case 0x05:authorization();break;
					case 0x06:back_record();break;
					
					//case 0x07:caima();break;
					//case 0x08:auto_back_caima();break;		//�Զ��ش���Ȩ���Զ��ش����붼��Կ���������Ϸ�
					
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
   uint8_t order ;   //������
	
 	 uint8_t pc_to_slave_ack[8]={0x55,0x7A,0x0D,0x03,0x00,0x01,0x01,0x21};
// 	 UART3nSendChar(3,pc_to_slave_ack,8);
// 	 delay_ms(100);

// 	 UART3nSendChar(3,pc_to_slave_ack,8);
// 	 delay_ms(100);

// 	 UART3nSendChar(3,pc_to_slave_ack,8);
// 	 delay_ms(100);

	 end_flag=0;			 //��Ϊȫ�ֱ���
   while(1) 
   {
				//IWDG_Feed();
				//UART3Reset(0);		//��λ���ڽ��ܻ�����				 
				while(1)  				//�ȴ����ص�USbͷ�ε��ֽ�
				{
					//IWDG_Feed();
// 				  UART3nSendChar(3,pc_to_slave_ack,8);
 				  delay_ms(100);					
					
					
				  m_UB3.RBuff;
					nChar =UART3ReadFrm(3,g_Buff,MAX_BUFF);	  //ͨ��������ȡ����						
					
				
					if(nChar>=3)										//���ﲻ�����ж�ͷ�����ַ���
						break;
					if(end_flag==1)						
						return;												//ֱ�ӷ���


				}
				
				
				order =g_Buff[2];  //ȡ����������
				
				switch(order)
				{
					case 0x01:register_key();break;
					case 0x02:mac_query();break;
					case 0x03:left_author_space();break;
					case 0x04:time_verify();break;
					
					case 0x05:authorization();break;
					case 0x06:back_record();break;
					
					//case 0x07:caima();break;
					//case 0x08:auto_back_caima();break;		//�Զ��ش���Ȩ���Զ��ش����붼��Կ���������Ϸ�
					
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
*@���ܣ�ע��Կ����Ϣ
*				0x01��λ��������λ���û���Ϣ
*				10�ֽڷ�����Ϣ+8�ֽ�Կ����Ϣ
*				��������Ϣ����flash���б��ݣ��ػ��󲻶�ʧ
*/
void register_key(void)
{
		uint8_t i,ID,receive_len;
		uint8_t xor =0;		      //У���
		uint8_t mid_buff[8];
		uint32_t addr;
	  uint32_t row;
		
		uint8_t temp_group_inf[10]={0};
		uint8_t temp_key_inf[8]={0};
	
		ID=g_Buff[5];	//g_Buff��ȫ�ֱ���

	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x01;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =ID;	
	
		ID=g_Buff[5];
	
		receive_len=g_Buff[3]+5;		//��ʾ��������ĳ���
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
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
			
				mid_buff[6]=0x01;		//���سɹ�
				xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
				mid_buff[7]=xor;
				IWDG_Feed();
			
				if(bluetooth_uart_switch==0)
					UART1nSendChar(1,mid_buff,8);	 //ͨ�����ڷ��سɹ���ȷ����		
				else
					UART3nSendChar(3,mid_buff,8);	 //ͨ���������سɹ���ȷ����							

		}
		else
		{

			mid_buff[6]= 0x00;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 //ͨ�����ڷ��سɹ���ȷ����		
			else
				UART3nSendChar(3,mid_buff,8);	 //ͨ���������سɹ���ȷ����	
		}	
	

}


/*
*@���ܣ�0x02 MAC��ַ��ѯ

*/
void mac_query(void)
{	
		uint8_t i=0,j=0,t=0,command_len,count=0;
		uint8_t xor =0;		//У���
		const uint8_t buf_len=50;
	  const uint8_t try_num=3;
		uint8_t char_buf[buf_len];
		uint8_t MAC[6];
		uint8_t mid_buff[13];	
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x02;
		mid_buff[3] =0x08;		//����
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];			//�˿ں�
	
		command_len=g_Buff[3]+5;
		
		for(i =0;i<command_len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[command_len-1])		//�жϽ�������У���Ƿ���ȷ
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
							UART1nSendChar(1,mid_buff,13);	 //ͨ�����ڷ���MAC��ַ		
						else
							UART3nSendChar(3,mid_buff,13);	 //ͨ����������MAC��ַ						
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
			
			
			
// 				for(t=0;t<try_num;t++)				//���ೢ��3��
// 				{
// 						UART3nSendChar(3,"TTM:MAC-?",10);		//��������ATָ��
// 						delay_ms(100);		//��ʱ�ȴ������ظ�
// 					
// 						count=UART3GetCount(3);
// 						UART3nReadChar(3,char_buf,count);			//�����������ݵ�char_buf
// 					
// 						//for debug
//  						//UART1nSendChar(1,char_buf,count);		//���Ͷ�������������
//  						//delay_ms(100);					
// 					
// 						for(j=0;j<count-1;j++)		//��forѭ���˳��Ļ���j=buf_len-1
// 							if(char_buf[j]=='0'&&char_buf[j+1]=='x')
// 									break;								//��break�˳��Ļ���j<buf_len-1
// 							

// 						if(j<count-1-12)							//�����ǰ�ɹ�
// 						{
// 							//for debug
//  							//UART1nSendChar(1,char_buf+j,12);		//���ַ���ʽ����MAC��ַ
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
// 								UART1nSendChar(1,mid_buff,13);	 //ͨ�����ڷ���MAC��ַ		
// 							else
// 								UART3nSendChar(3,mid_buff,13);	 //ͨ����������MAC��ַ								
// 							
// 							
// 							break;//�����ٳ�����
// 							
// 						}
// 						
// 				}
// 				
// 				//ʧ��
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
*@���ܣ����ַ�ת��Ϊʮ������
				bufferΪ�ַ�����ַ��destΪʮ���������ݵ�ַ��buf_lenΪbuffer���ַ��ĳ���

        "0xB4994C861F26"��"B4994C861F26"���ֵ����������ַ�ת��Ϊһ����

*/
void CharToHex(uint8_t * buffer, uint8_t * dest, uint32_t buf_len)
{
	 uint32_t i=0;
	 uint32_t j=0;
	 uint8_t temp;
	 while(i<buf_len)
	 {
		temp=buffer[i];
		if((temp>=0x30)&&(temp<=0x39))				//temp��0��9֮��
		{
			 temp=temp-0x30;
			 dest[j]=temp<<4;
		}
		else if((temp>=0x41)&&(temp<=0x46))		//temp��A��F֮��
		{
			 temp=temp-0x41+0x0A;
			 dest[j]=temp<<4;
		}
		else if((temp>=0x61)&&(temp<=0x66))		//temp��a��f֮��
		{
			 temp=temp-0x61+0x0A;
			 dest[j]=temp<<4;
		}
		else
		{
			 dest[j]=0x00;
		}
		
	

		temp=buffer[i+1];
		if((temp>=0x30)&&(temp<=0x39))				//temp��0��9֮��
		{
			 temp=temp-0x30;
			 dest[j]=dest[j]|temp;
		}
		else if((temp>=0x41)&&(temp<=0x46))		//temp��A��F֮��
		{
			 temp=temp-0x41+0x0A;
			 dest[j]=dest[j]|temp;
		}
		else if((temp>=0x61)&&(temp<=0x66))  //temp��a��f֮��
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
*@���ܣ�0x03���洢�ռ��ѯ
*
*@˵�������ص���ʵ��ʣ�������Ȩ������
*/
void left_author_space(void)
{		
		uint8_t i,receive_len;
		uint8_t mid_buff[11];
		uint16_t left_space;
	  //uint32_t addr;
		uint8_t xor =0;		//У���
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x03;

		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];			
	
		receive_len=g_Buff[3]+5;
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
		if(xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
		{
				//addr= SPI_FLASH_ReadWord(AUTHOR_POINT);   

				//for debug
// 				UART1SendwordHex(1, addr);
// 				delay_ms(1000);			
			
			
				//left_space=(AUTHOR_END-addr)/AUTHOR_LEN;

				left_space=(AUTHOR_MAX_NUM-SPI_FLASH_ReadWord(AUTHOR_ROW));		//����ʣ��ռ���ֽ���,�����Ŀռ��׼ȷ�����ǵ�ɾ������Ȩ��
				
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
*@���ܣ�0x04�Ե���Կ�׽���Уʱ
*				6���ֽڵ�ʱ�䣨YYMMDDHHMMSS��
*/
void time_verify(void)
{
  uint8_t time_date[6];
  uint8_t mid_buff[13];
  uint8_t i,k,receive_len;
  uint8_t xor =0;		//У���
  IWDG_Feed();
  mid_buff[0] =0x55;		//��ʾ��λ��������λ��
  mid_buff[1] =0x7a;
  mid_buff[2] =0x04;		//������

  mid_buff[4] =0x00;
  mid_buff[5] =0x01;		//ID
  IWDG_Feed();
	
	receive_len=g_Buff[3]+5;
	
	for(i =0;i<receive_len-1;i++)
		xor =xor^g_Buff[i];	
	if(xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
	{
			for(i =0;i<6;i++)
				time_date[i] =g_Buff[6+i];		//g_Buff[7~12] 6�����鵥Ԫ
			
			set_time(time_date[0],time_date[1],time_date[2],time_date[3],time_date[4],time_date[5]);//������λ����ʱ������Կ�׵�ʱ��
			delay_ms(500);
			IWDG_Feed();
			read_time();    //����ʵʱ��ʱ��,����time_buff��
			for( i =0;i<6;i++)
				mid_buff[6+i] =time_buff[i];
		  mid_buff[3] =0x08;		//����
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
		mid_buff[3] =0x03;		//����
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
*@���ܣ�0x05����Ȩ�ļ�����Ϣд��EEPROM
*				ÿ��дһ����Ȩ�ļ�
*				�ظ����ܳɹ�����
*				2���ֽ��û����+2�ֽ������ +4�ֽ���RFID
*/
void authorization(void)
{
		uint8_t data_len,i,receive_len;
		uint8_t xor=0;
		uint8_t mid_buff[8];
		uint32_t addr,total_row;
		data_len=g_Buff[3];		//g_Buff��ȫ�ֱ���
	
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x05;
		mid_buff[3] =0x03;
		mid_buff[4] =0x00;
		mid_buff[5] =g_Buff[5];
	
		receive_len=g_Buff[3]+5;
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
		{	
			
				addr= SPI_FLASH_ReadWord(AUTHOR_POINT);  //��ʼ�� AUTHOR_POINT=AUTHOR_START
				IWDG_Feed();
		
				SPI_FLASH_Write((g_Buff+6),addr,(data_len-2));//��ô�������ľ���ô��
			
				//for debug
// 				SPI_FLASH_Read(item,addr,25);
// 			  UART1nSendChar(1,item,25);
// 				delay_ms(1000);			

			
			
				

							
				addr=addr+AUTHOR_LEN;   //ÿ����ȨAUTHOR_LEN=25�ֽ�
				//������Ȩ�û���Ϣָ���λ��
				SPI_FLASH_WriteWord(AUTHOR_POINT,addr);		
				
				//for debug
// 				point=SPI_FLASH_ReadWord(AUTHOR_POINT);		//AUTHOR_POINT=AUTHOR_START;AUTHOR_START=0x080000
// 				UART1SendwordHex(1, point);
// 				delay_ms(1000);


				
				//Ҫ���ֺ��ַ���ŵ����ݵ����Ǳ�ʾ��ַ��������
				total_row= SPI_FLASH_ReadWord(AUTHOR_ROW);   		
				total_row=total_row+1;		//��Ȩ��Ϣ������һ
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
*@���ܣ��ش���¼
*				0x06	�������в�����¼��Ϣ,��Ҫ�ǿ���״̬��
*				ÿ֡: 55 +7A +06+0F(���ݳ���)+00+ID+2�ֽ��û����+4���ֽ�RFID+6�ֽ�ʱ��+1�ֽڿ���״̬+1�ֽ�У��λ
*
*				2�ֽ��û����+4���ֽ�RFID+6�ֽ�ʱ��+1�ֽڿ���״̬
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
	
		ID=0x01;//����ID����01
		
		mid_buff[0] =0x55;
		mid_buff[1] =0x7a;
		mid_buff[2] =0x06;

		mid_buff[4] =0x00;
		mid_buff[5] =ID;		
			
	
	
		receive_len=g_Buff[3]+5;		//�����Ϊ7
		
		for(i =0;i<receive_len-1;i++)
			xor =xor^g_Buff[i];	
	  if(xor==g_Buff[receive_len-1]&&receive_len==7)		//�жϽ�������У���Ƿ���ȷ,���������Ϊ7��ȷ�������Ϊ8
		{			
			
				addr = SPI_FLASH_ReadWord(RECORD_POINT);  		//RECORD_POINT=RECORD_START
				
				//for debug
// 				UART1SendwordHex(1, addr);
// 				delay_ms(1000);
				////OLED_ShowNumHex(0,6,addr,8,16);
			
				total_row=(addr-RECORD_START)/RECORD_LEN;		//ÿ����Ȩ25�ֽ�
			
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
						for(i=0;i<total_row;i++)		//��ʱi>=1
						{
							
								start_addr=RECORD_START+i*RECORD_LEN;		
							  
								//for debug
// 								UART1SendwordHex(1,start_addr);				
// 								delay_ms(1000);		
							
								SPI_FLASH_Read(mid_buff+6,start_addr,13);					
							
								
								mid_buff[3] =send_len-5;		//�����Ϊ7
								xor=0;
								for(m =0;m<send_len-1;m++)
									xor =xor^mid_buff[m];						
								mid_buff[send_len-1]=xor;
															
								//���͵�һ����¼
								if(bluetooth_uart_switch==0)
									UART1nSendChar(1,mid_buff,send_len);	 	
								else
									UART3nSendChar(3,mid_buff,send_len);								
							

								IWDG_Feed();
								UART1Reset(0);		//��λ���ڽ��ܻ�����										
								while(1)  //������һ����Ȩ֮�����ȴ���λ���ķ�������
								{
//											UART1Reset(0);		//��λ���ڽ��ܻ�����		(���ڸ�λ���ܷ�������)										
									
// 											g_WaitTimeOut = 0;
// 											SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);		//Do_TimeOut��g_WaitTimeOut��Ϊtrue
// 											while( g_WaitTimeOut == 0 );
// 											KillTimer(TIMER_TIMEOUT);		
									
											//delay_ms(500);									
									
											if(bluetooth_uart_switch==0)
												nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  ��ȡ��Э���ʽ���͹��������ݣ������н���		
											else
												nChar =UART3ReadFrm(3,g_Buff,MAX_BUFF);	  //ͨ��������ȡ����										
									
									
									
									
											//���û��������nChar=0
																												
											if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xDA)//�������0x01�ͷ���һ����Ȩ
											{
												IWDG_Feed();
												//UART1nSendChar(1,"���ܳɹ�",5);	
												
												//for debug
// 				 								UART1SendwordHex(1,i);				
// 				 								delay_ms(1000);													
												
												
												if(i==total_row-1)		//����������һ�������û�м�¼�ɷ���ֱ�ӷ���������
												{
													
														if(bluetooth_uart_switch==0)
															UART1nSendChar(1,record_end_cmd,8);	 	
														else
															UART3nSendChar(3,record_end_cmd,8);													
													
													
														return;
												}
												
												break;		//ֻ�������ڲ��ȴ�ѭ����������һ����Ȩ
											}	
											else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x00&&g_Buff[7]==0xDB)//�������0x01��ɾ��������Ȩ���ٷ���һ��
											{


												i=i-1;		//�����ڲ��ȴ�ѭ��֮�󣬻�ֱ������������ѭ����ֱ�ӽ��������Զ��ش�
												break;
// 												IWDG_Feed();
// 												UART1nSendChar(1,"����ʧ��",5);
												
// 												;		//ʲôҲ���������ȴ�
											}
											else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x06&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x02&&g_Buff[7]==0xDB)//�������0x01��ɾ��������Ȩ���ٷ���һ��
											{
												if(bluetooth_uart_switch==0)
													UART1nSendChar(1,slave_to_host_ACK,8);	 	
												else
													UART3nSendChar(3,slave_to_host_ACK,8);	
												return;
// 												IWDG_Feed();
// 												UART1nSendChar(1,"ֱ�ӽ���",5);
											
// 												;		//ʲôҲ���������ȴ�
											}											

								}//�ڲ�whileѭ������

						 }//forѭ������
				}//else����
		}
		else
		{
	
			mid_buff[3] =0x03;
			mid_buff[6]= 0x00;
			xor=0;
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
			
			IWDG_Feed();
			UART1nSendChar(1,mid_buff,8);	 //����ʧ����		
		}
}






/*
*@���ܣ�0x07�豸����
*				2���ֽ�Ա�����+2�ֽ������ 
*/
// void caima(void)
// {
// 		uint8_t data_len,i,total_row,receive_len;
// 		uint8_t xor;
// 		uint8_t mid_buff[8];
// 		uint8_t addr;
// 		data_len=g_Buff[3];		//g_Buff��ȫ�ֱ���
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
// 	  if(xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
// 		{	

// 				addr= SPI_FLASH_ReadWord(CAIMA_POINT);   
// 		
// 				SPI_FLASH_Write((g_Buff+6),addr,(data_len-2));
// 				
// 				
// 				addr=addr+CAIMA_LEN;   //������ÿ��5���ֽ�
// 				//������Ȩ�û���Ϣָ���λ��
// 				IWDG_Feed();
// 				SPI_FLASH_WriteWord(CAIMA_POINT,addr);		

// 				
// 				
// 				//Ҫ���ֺ��ַ���ŵ����ݵ����Ǳ�ʾ��ַ��������
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
// 				UART1nSendChar(1,mid_buff,8);	 //����ȷ����
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
// 				UART1nSendChar(1,mid_buff,8);	 //����ʧ����		
// 		}

// }





/*
*@���ܣ�0x08�Զ��ش�����
*				2���ֽ�Ա�����+2�ֽ������ +4���ֽڵ�����
*				��4���ֽڵ�������Կ�ײ���ʱ������
*/
// void auto_back_caima(void)
// {
// 		uint8_t i,j,k,m,ID,total_row,nChar,receive_len;
// 		uint8_t xor=0;
// 		uint8_t mid_buff[15];
// 		uint8_t addr,start_addr;
// 		
// 		ID=0x01;//����ID����01
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
// 	  if(receive_len==7&&xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ,��������������ȷ������������
// 		{		

// 				addr= SPI_FLASH_ReadWord(CAIMA_POINT);   
// 				
// 				total_row=(addr-CAIMA_START)/8;
// 				
// 			  if(total_row==0)
// 					 UART1nSendChar(1,"00000",5);		//���total_row=0��

// 				for(i=0;i<total_row;i++)
// 				{
// 						start_addr=CAIMA_START+i*8;
// 						
// 						for(j=0;j<8;j++)		//8���ֽڣ��û���š�����š�����
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
// 						UART1nSendChar(1,mid_buff,15);	 //����һ����Ȩ�ļ�
// 						
// 						for(k =0;k<WRITE_ZHI;k++)
// 							delay_ms(500);
// 						IWDG_Feed();

// 						UART1Reset(0);		//��λ���ڽ��ܻ�����				 
// 						while(1)  
// 						{
// 							IWDG_Feed();
// 							nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  
// 							if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x08&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xD4)
// 								break;		//����յ�ȷ�ϣ������ȴ�������һ����������
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
// 			UART1nSendChar(1,mid_buff,8);	 //����ʧ����		
// 		}
// }



/*
*@���ܣ�0x09ɾ���豸����ɾ����
*				2�ֽ�����
*@˵������ɾ������ȨȫΪ1���ڲ�����Ȩʱ�ǲ���ƥ��ģ���Ϊû��RFIDȫΪ1
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
	  if(xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
		{
				temp_addr= SPI_FLASH_ReadWord(AUTHOR_POINT);   
			
				known_max_row=(temp_addr-AUTHOR_START)/AUTHOR_LEN;		//ֻ���AUTHOR_START~AUTHOR_POINT֮���������Ȩ������ɾ������Ȩȫ��Ϊ0
			
			
				addr=AUTHOR_START;
				for(i=0;i<known_max_row;i++)		//��ʵ���Բ������е���Ȩ
				{
						SPI_FLASH_Read(keycode,addr+2,2);
					
						//for debug
// 						UART1nSendChar(1,keycode,2);
// 						delay_ms(1000);					
					
					
						if(keycode[0]==g_Buff[6]&&keycode[1]==g_Buff[7])		//��������Ų����û����Ҹ��û�û�б�ɾ��
								break;
						
						addr=addr+AUTHOR_LEN;		//��ǰ��Ų�ƥ�䣬�����һ����Ȩ
				}
				
				
				
				
				if(i<known_max_row)		//����ҵ���Ӧ�������
				{
					
						SPI_FLASH_Write(clear_author,addr,AUTHOR_LEN);		//���ҵ�����Ȩȫ����1
					

						total_row= SPI_FLASH_ReadWord(AUTHOR_ROW);   			//����ʵ�ʵ�����
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
				else		//û�鵽��Ӧ�������
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
		else		//����У�鲻��ȷ
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
0x0a��λ��������λ��Ѳ������
2�ֽ������+2�ֽ�Ѳ��������+14���ֽھ������� +1�ֽ�Ѳ����
*/
// void download_xunjian(void)
// {
// 		uint8_t data_len,i,total_row,receive_len;
// 		uint8_t xor=0;
// 		uint8_t mid_buff[8];
// 		uint8_t addr;
// 		data_len=g_Buff[3];		//g_Buff��ȫ�ֱ���

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
// 	  if(receive_len>8&&xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
// 		{
// 			addr= SPI_FLASH_ReadWord(XUNJIAN_POINT);   
//       //��ô�������ľ���ô��,ע���ǰ����ݳ������ŵ�
// 			SPI_FLASH_Write((g_Buff+6),addr,(data_len-2));
// 			
// 			
// 			addr=addr+19;			 //ÿһ��Ѳ������19���ֽ�			
// 			//����Ѳ��С����ָ���λ��
// 			IWDG_Feed();
// 			SPI_FLASH_WriteWord(XUNJIAN_POINT,addr);		//#define AUTHOR_START  0x0b4f 
// 		
// 			
// 		
// 			//Ҫ���ֺ��ַ���ŵ����ݵ����Ǳ�ʾ��ַ��������
// 			//����Ѳ��С�����������ֻ��53��������ֻ���������ĵ�8λ
// 			IWDG_Feed();			
// 			total_row= SPI_FLASH_ReadWord(XUNJIAN_ROW) ;   		
// 			total_row=total_row+1;		//Ѳ��С����������һ
// 			SPI_FLASH_WriteWord(XUNJIAN_ROW,total_row);				
// 			

// 			IWDG_Feed();
// 	
// 			mid_buff[6]= 0x01;
// 			xor=0;
// 			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 			mid_buff[7]=xor;
// 			IWDG_Feed();
// 			UART1nSendChar(1,mid_buff,8);	 //����ȷ����
// 		}
// 		else
// 		{
// 			mid_buff[6]= 0x00;
// 			xor=0;
// 			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
// 			mid_buff[7]=xor;
// 			
// 			IWDG_Feed();
// 			UART1nSendChar(1,mid_buff,8);	 //����ʧ����		
// 		}
// 	
// }




/*
0xob�Զ��ش�Ѳ��������
2�ֽ������+2�ֽ�Ѳ��������+0x22(����������ʼ��)+Ѳ���� +0x23(����������ֹ��) 
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
// 	  if(receive_len==7&&xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
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
// 					SPI_FLASH_Read(mid_buff+receive_len-2,start_xunjian+18,1);		//ȡѲ��С����ĩβ��Ѳ����
// 					
// 					xor=0;
// 					for(m =0;m<11;m++)
// 						xor =xor^mid_buff[m];	
// 					mid_buff[11]=xor;

// 					delay_ms(500);
// 					IWDG_Feed();
// 					UART1nSendChar(1,mid_buff,12);	 //����Ѳ����				
// 					
// 					IWDG_Feed();
// 					UART1Reset(0);		//��λ���ڽ��ܻ�����				 
// 					while(1)  //������һ����Ȩ֮�����ȴ���λ���ķ�������
// 					{
// 						
// 						for(k =0;k<WRITE_ZHI;k++)
// 								delay_ms(500);
// 						IWDG_Feed();
// 						nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//#define  MAX_BUFF      520  
// 						
// 						if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0b&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x01&&g_Buff[7]==0xD7)//�������0x01�ͷ���һ����Ȩ
// 								break;		//ֻ�������ڲ��ȴ�ѭ��
// 						else if(nChar==8&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0b&&g_Buff[3]==0x03&&g_Buff[4]==0x00&&g_Buff[5]==ID&&g_Buff[6]==0x00&&g_Buff[7]==0xD6)
// 						{
// 								i--;		//�൱���ط�
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
// 			UART1nSendChar(1,mid_buff,8);	 //����ʧ����		
// 		}
// } 


/*
oxoc���������Ȩ
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
	
	  if(xor==g_Buff[receive_len-1])		//�жϽ�������У���Ƿ���ȷ
		{

			IWDG_Feed();
			
			
			SPI_FLASH_WriteWord(AUTHOR_ROW,0x00);		 //��Ȩ��Ϣ����Ϊ0

			SPI_FLASH_WriteWord(AUTHOR_POINT,AUTHOR_START);		//��Ȩָ��ָ����Ȩ���׵�ַ


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
*@���ܣ�PC��������Կ��
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
				 OLED_ShowUnite(16,2,"����ͨ����");
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
				 OLED_ShowUnite(16,2,"��������ʧ��");
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
*@���ܣ�PC��������ͨ��
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
			
			end_flag=1;		//ͨ�Ž�����ֱ���˳��ȴ�����״̬
			
			mid_buff[6]= 0x01;	//��ʾ�ɹ�
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);		
			
			
			
			USB_Cheak_init_flag=0;		//��ʾ�ǳ�ʼ������״̬
		}
	  else
		{
			mid_buff[6]= 0x00;	//��ʾʧ��
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
*@���ܣ����ʹ���������FLASH
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
			
			mid_buff[6]= 0x01;	//��ʾ�ɹ�
			xor =mid_buff[0]^mid_buff[1]^mid_buff[2]^mid_buff[3]^mid_buff[4]^mid_buff[5]^mid_buff[6];
			mid_buff[7]=xor;
		  IWDG_Feed();
			if(bluetooth_uart_switch==0)
				UART1nSendChar(1,mid_buff,8);	 	
			else
				UART3nSendChar(3,mid_buff,8);		
			
			
			USB_Cheak_init_flag=0;		//��ʾ�ǳ�ʼ������״̬
		}
	  else
		{
			mid_buff[6]= 0x00;	//��ʾʧ��
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
*@���ܣ����ʹ����������ÿ�������
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

			
					
			mid_buff[6]= 0x01;	//��ʾ�ɹ�
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
			mid_buff[6]= 0x00;	//��ʾʧ��
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
����״̬����
����״̬
*/
//uint8_t key_state(void)
//{
//		return 1;

//}


