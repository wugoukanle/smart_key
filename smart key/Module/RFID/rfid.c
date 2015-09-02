/**********************************************************************
*												smart key
*										 Module��RFIDģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-21 18:07     1.0             build this file 
*
* ģ�鹦�ܣ�
*				ͨ������2��ȡ�жϵķ�ʽ��ȡRFID������
*				

*	����˵����
* -----------------------------------------------------------
*       GND   ��Դ��
*       VCC   3.3v��Դ
*       TX2   ��PA2
*       RX2   ��PA3             
* ----------------------------------------------------------
*				
* ���䣺	RFIDģ�������ֶ�ȡ��ʽ 1.�����ж϶˿ڶ�ȡά���������
																	2.���ô����ж�ģʽ��ȡ����
**********************************************************************/ 



#include "rfid.h"
#include "usb.h"
#include "oled.h"
#include "Globle_Var.h"
#include "usart.h"
#include "iwdg.h"
#include "delay.h"


UARTBUFF1  m_UB2;


//Τ������
uint8_t byte_cnt=0;
uint8_t bit_cnt=0;
uint32_t wg_data[4]={0xff,0xff,0xff,0xff};
uint32_t wg_buf[4]={0xff,0xff,0xff,0xff};
uint8_t wg_mask=0x80;
uint8_t a;

void resume_wg26_data(void);
void store_tag_bit(uint8_t tag_data_bit );




uint8_t Lock_number[4]={0,0,0,0};		//�ݴ����룬��check_lock_code�и�ֵ����save_rfid_resultʹ��
uint8_t User_Num[2]={0,0};					//�ݴ��û���ţ���check_lock_code�и�ֵ����save_rfid_resultʹ��


/*
*@���ܣ������˿�  ����ڣ���������жϽ���
*				GPIO�����ⲿ�ж�ģʽ��������ʱ������
*/
void Rfid_IO_Init_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //Key_IO--ͨ�Źܽţ�PA0����EXITA����WG0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //Key_IO--ͨ�Źܣ�PA1,��EXITB����WG1
}
 

/*
���ڵ����÷�Ϊ3��

1.���ڳ�ʼ��������ʱ�ӿ�����GPIO���ã�

2.���ڲ�������

3.�����жϿ�����ͳһ����nvic.c�У�

4.�����жϴ�������ͳһ����stm32f10x_it.c�У�

*/
 


/*
*@���ܣ�����2��ʼ������
*/
void Uart2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	//USART_InitTypeDef USART_InitStructure;
	
	//NVIC_InitTypeDef NVIC_InitStructure; 
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);		
	
	
	//����UART2 TX��PA.2��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//����UART2 RX��PA.3��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);


	//�жϳ�ʼ�����ַŵ�nvic.c��
//  ����USART1�����ж�
// 	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);	
	
}


 
 
/*
*@���ܣ�uart2 ������
*				�����ʣ�9600
*				���ݳ��ȣ�8λ
*				ֹͣλ��1λ
*				����żУ��
*				��Ӳ��������
*				ȫ˫��
*@˵�������ó�������RFIDģ��ȷ����
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
	
	//ͬ��ͨ��ʱ�õ�
	//USART_InitStructure.USART_Clock = USART_Clock_Disable;
	//USART_InitStructure.USART_CPOL = USART_CPOL_Low;
	//USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
	//USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

  //USART_StructInit(USART_InitStructure);
  USART_Init(USART2, &USART_InitStructure);
	
  /* Enable USART2 */
  USART_Cmd(USART2, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); /*�����ж�ʹ��*/
}


/*
*@���ܣ�ͨ������2����һ���ַ���һ���ֽڣ�
*/
void UART2SendChar(uint8_t uart,uint8_t data)
{
	
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
			USART_SendData(USART2,data);
	 while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  
	
}


/*
*@���ܣ�ͨ������2����n���ַ���n���ֽڣ�
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
*@���ܣ�����2�Ľ����жϺ������ŵ�stm32f10x_it.c
**/
// void USART2_IRQHandler(void)
// {
//   u8 nChar;
//   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//   {
//     nChar =USART_ReceiveData(USART2);
//     m_UB2.RBuff[m_UB2.cRHead] = nChar;		//�����ܵ��ַ����ڻ�������
// 		++m_UB2.cRHead;
// 		if (m_UB2.cRHead >= UARTRXLEN1)
// 			m_UB2.cRHead = 0;		//�û�������Ϊһ����
//     USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//   }
// }




/*
*@���ܣ���ȡRFID���ڷ��͹��������ݰ�,�ж��Ƿ���ȷ
*
*@ʵ��:����Ϊ5���ֽ�,�����ж�
* �����ջ����������յ�һ����Ŀ���ֽ�ʱ�������ж�
*	���ݵĽ��ܹ�����USART2_IRQHandler�����д���
* �����жϽ����ܵ�������ȫ���ŵ������������ݳ��Ȳ��ܣ�
*	Ȼ��Read_RfidData��������ͨ���������ID���ݵĿ�ͷ��
*	������Ծ�������ƶ�һλ������У��
*	�����ȷ�ͽ��Ӵ�λ�ӿ�ʼ��5���ֽڷ���pdst��
*
*/
uint8_t Read_RfidData(uint8_t uart, uint8_t *pdst, uint8_t max)
{
	//uartû���õ���ֱ���õ���USART2
  uint8_t count = 0,total;
  uint8_t idx,off;
  
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//���ж�
  if(m_UB2.cRHead != m_UB2.cRTail)
  {
      //������Ч�����ܳ���
      if (m_UB2.cRHead >= m_UB2.cRTail)
      {
        total  = m_UB2.cRHead - m_UB2.cRTail;
      } 
			else
			{
        total  = UARTRXLEN2 + m_UB2.cRHead - m_UB2.cRTail ;
      }
      //�ҽ�����
      if (total>=5)//֡��������Ϊ5����������ȷ�ģ���֤������������ID
      {
        
						//���ݳ�������һ֡��Ҫ��
						total = 0;//��¼У���
						for (idx=0; idx<5;idx++)
						{
							off = (m_UB2.cRTail + idx) % UARTRXLEN2;		//ƫ�Ƶ�ַ
							pdst[idx] = m_UB2.RBuff[off];		//�����������ݷŵ�Ŀ��������
							total ^= pdst[idx];//����У��ͣ�Ӧ����żУ��
						}
						if (total==0)//У����ȷ
					  {
							m_UB2.cRTail = (off + 1) % UARTRXLEN2;
						  count =5;
					  }
						else		//ͨ��У�����ҵ�ID���ݵ�ͷ���ص�
						{
								//У���������1���ֽڣ���ʵΪ��֡��ͷ
								m_UB2.cRTail = (m_UB2.cRTail + 1) % UARTRXLEN2;
								count = 0;
						}
       }
	 }
// 	 m_UB2.cRHead=0;
// 	 m_UB2.cRTail=0;
	 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���ж�
	 
	if(count ==5) 
	{
		if(pdst[0]==0&&pdst[1]==0&&pdst[2]==0&&pdst[3]==0)
			count=0;		//��ʱ��count=5��Ч
		else		//������һ����Ϊ��
		{
			UART1nSendChar(1,pdst,5);
			UART2Reset(0);
		}
	}	 

  return count;		//countҪôΪ5ҪôΪ0������0��ʾУ��ʧ��

}


/*
*@���ܣ��建����
*/
void UART2Reset(uint8_t uart)
{		
		//�ȹؽ����ж�
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt

		m_UB2.cFlag = 0;
		m_UB2.cRHead = 0;
		m_UB2.cRTail = 0;
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}








/************************
Τ��26Э��
HID��HiddenIDcode ������
PID��PublicIDcode ������

26λ=2~13λ��У��λ1λ+HID�ĵ�8λ+PID��16λ+14~25λ��У��λ1λ

Τ��Ӧ�����ⲿ�жϣ���ѯ�ǲ��е�
****************************/


// void resume_wg26_data(void)
// {
// 	
// 		uint32_t i;
// 		wg_data[0] = wg_buf[0] << 1;
// 		wg_data[0] &=0xfe;
// 		i = wg_buf[1] >>7 & 0x01;
// 		wg_data[0] |= i;		//��2~9λ����wg_data[0],��HID�ĵͰ�λ
// 		
// 		wg_data[1] = wg_buf[1] << 1;
// 		wg_data[1] &=0xfe;
// 		i = wg_buf[2] >>7 & 0x01;
// 		wg_data[1] |= i;		//��10~17λ����wg_data[1]����PID�ĸ߰�λ
// 		
// 		wg_data[2] = wg_buf[2] << 1;
// 		wg_data[2] &=0xfe;
// 		i = wg_buf[3] >>7& 0x01;
// 		wg_data[2] |= i;		//��wg_data����wg_data[2]����PID�ĵͰ�λ
// 	
// }




// void store_tag_bit(uint8_t tag_data_bit )		//�������Ӧ����Ϊ�ж�׼����
// {
// 		//  ( mask = 0x80 ; mask > 0 ; mask >>= 1 )   //1000 0000=0x80
// 		if( tag_data_bit == 1 )
// 			{wg_buf[byte_cnt] |= wg_mask;}		//��1
// 		else
// 			{wg_buf[byte_cnt] &=~wg_mask;}    //��0              //0111 1111=~0x80   =0        

// 		wg_mask>>= 1;		//wg_mask����1λ
// 		bit_cnt ++;
// 		
// 		if(wg_mask == 0)		//	���wg_mask����1λ֮��Ϊ0
// 		{
// 				wg_mask = 0x80;
// 				byte_cnt++; 
// 		}
// 						//
// 		if(bit_cnt==26)		//������26λ֮����������wg_bufΪwg_data
// 		{
// 			resume_wg26_data();		//��26λΤ������HID����wg_data[0],PID����wg_data[1~2]
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
		//AA  75 +11+06+00 +ID +4�ֽ�RFID+1�ֽ�У��λ
    uint8_t PDA_RFID[11]={0xAA,0x75,0x11,0x06,0x00,0x01,0x00,0x00,0x00,0x00,0x00};


		//��ST_Idle�������Ѿ���⵽��RFID���Ż��������
		for(i =0;i<4;i++)		//Lock_number[4]��Rfid_Buff[6]����ȫ�ֱ���
		 Lock_number[i] =Rfid_Buff[i]; //��ST_Idle()���Ѿ������ݷ���Rfid_Buff[]	

		
		for(i =0;i<4;i++)		//Lock_number[4]��Rfid_Buff[6]����ȫ�ֱ���
		 PDA_RFID[i+6] =Rfid_Buff[i]; //��ST_Idle()���Ѿ������ݷ���Rfid_Buff[]	
		PDA_RFID[10]=PDA_RFID[0]^PDA_RFID[1]^PDA_RFID[2]^PDA_RFID[3]^PDA_RFID[4]^PDA_RFID[5]^PDA_RFID[6]^PDA_RFID[7]^PDA_RFID[8]^PDA_RFID[9];



		//����RFID���ı��,��UART2�����жϣ���RFID���ݷŵ��ŵ���������
		//����read_RfidData�����ݶ���Rfid_Buff[],���������ST_Idle�����е��ù�
		IWDG_Feed();
	    
		OLED_Clear();
		OLED_ShowUnite(16,2,"ID:");
		OLED_ShowNumHex(48,2,Lock_number[0],2,16);
		OLED_ShowNumHex(64,2,Lock_number[1],2,16);
		OLED_ShowNumHex(80,2,Lock_number[2],2,16);
		OLED_ShowNumHex(96,2,Lock_number[3],2,16);		
		
	  //OLED_ShowUnite(16,4,"�˶���Ȩ��");	
		nChar= check_lock_code();		//ƥ���Ϊ1����ƥ���Ϊ0	
		//UART1nSendChar(1,&nChar,1);			 
		//UART1nSendChar(1,"22222",5);				  


		//nChar =1;		//ֻ�ж�ȡ����������Ȩ�ļ��У��ŷ���1	
		if(nChar ==0)
		{
			OLED_ClearArea(0,4,128,6);
			OLED_ShowUnite(36,4,"δ��Ȩ");						
			
			buzzer_unlock_fail();
			
			delay_ms(1500);
			
			unlock_result=2;							//������Ȩ����Ҫ��״̬���			
			
		  save_rfid_result();             //�����뼰����״̬д��洢оƬ��
			
			g_State =ST_IDLE;
			unlock_result=2;		          //������Ȩ����Ҫ��״̬���
			return;
		}		
		else if(nChar==1)
		{ 				
			IWDG_Feed();

			UART3nSendChar(3,PDA_RFID,11);
			
			buzzer_unlock_success();
			COIL_ACTUATE;
			
			OLED_ClearArea(0,4,128,6);			
			OLED_ShowUnite(36,4,"����Ȩ");					
			
			delay_ms(1000);
			
			unlock_result=1;
			
			save_rfid_result();                        //�����뼰����״̬д��洢оƬ��
			
			COIL_NO_ACTUATE;//�ر�SPX2941��Դ�����ر�Կ�׶�������
			
			//UART1nSendChar(1,"33333",5);						
			g_State =ST_IDLE;
			IWDG_Feed();
			
			UART2Reset(2);
			
			return;
		}
		
}








/*
*@���ܣ�����ȡ���������Ƿ�����Ȩ�ļ�����
*
*
*/
uint8_t check_lock_code(void)
{
		uint8_t i,j,total_row;
		uint8_t known_lock_number[4];
		uint32_t lock_number_addr,addr_end;
		uint8_t IN_AUTHOR_OFFSET=4;		//RFID��һ����Ȩ�е�ƫ��


		addr_end= SPI_FLASH_ReadWord(AUTHOR_POINT);   		//�ڽ���Ĳ��빦�����ı����ָ���ֵ
		
		total_row=(addr_end-AUTHOR_START)/AUTHOR_LEN;
		
// 		if(total_row==0)
// 		{
// 			UART1nSendChar(1,"û����Ȩ�ļ�",11);		//���total_row=0��	
// 			return 0;
// 		}

		for(i=0;i<total_row;i++)		//
		{		
				lock_number_addr=AUTHOR_START+i*AUTHOR_LEN+IN_AUTHOR_OFFSET;
					

				SPI_FLASH_Read(known_lock_number,lock_number_addr,4);
				
				IWDG_Feed();
				
				if(Lock_number[0]==known_lock_number[0]&&Lock_number[1]==known_lock_number[1]&&Lock_number[2]==known_lock_number[2]&&Lock_number[3]==known_lock_number[3])
				{					
					SPI_FLASH_Read(User_Num,lock_number_addr-4,2);		//���������RFID����Ȩ�����¼�����û����			
					return 1;		//�����˾ͷ���1
				}
		}
		User_Num[0]=0;
		User_Num[1]=0;
		return 0;

}



/*
*@���ܣ�2�ֽ��û����+4�ֽ�RFID+6�ֽ�ʱ��+1�ֽڿ�������������¼��
*
*				ÿ������Ϊ13�ֽ�
*/
void save_rfid_result(void)  //����Կ��
{
	uint32_t record_addr;
	uint32_t Lock_number_addr;
	uint32_t total_row;
	uint8_t  temp_Lock_number[4];
	
	 
	
	IWDG_Feed();
  record_addr= SPI_FLASH_ReadWord(RECORD_POINT);   //�Ѵ洢������
	
	
	
	SPI_FLASH_Write(User_Num,record_addr,2);				 		//д���ҵ�����Ȩ���û���ŵ���¼�У����û���ҵ�����Ϊ��
	
	SPI_FLASH_Write(Lock_number,record_addr+2,4);		 		//д������RFID����¼��
	
	read_time();  																 			//��ȡʵʱʱ��,�ŵ�time_buff��
	SPI_FLASH_Write(time_buff,record_addr+6,6);			 		//дʵʱʱ�䵽time��¼��

	SPI_FLASH_Write(&unlock_result,record_addr+12,1);		//д�����������¼��

	//for debug	
	SPI_FLASH_Read(item,record_addr,13);
	UART1nSendChar(1,item,13);
	delay_ms(1000);		
	
	
	record_addr=record_addr+RECORD_LEN;   //ÿ����ȨRECORD_LEN=13�ֽ�
	//������Ȩ�û���Ϣָ���λ��
	SPI_FLASH_WriteWord(RECORD_POINT,record_addr);		


	//for debug
	point=SPI_FLASH_ReadWord(RECORD_POINT);		//RECORD_POINT=RECORD_START
	UART1SendwordHex(1, point);
	delay_ms(1000);	



	//Ҫ���ֺ��ַ���ŵ����ݵ����Ǳ�ʾ��ַ��������
	total_row= SPI_FLASH_ReadWord(RECORD_ROW);   		
	total_row=total_row+1;		//��Ȩ��Ϣ������һ
	SPI_FLASH_WriteWord(RECORD_ROW,total_row);					


	//for debug
	row=SPI_FLASH_ReadWord(RECORD_ROW);
	UART1SendwordHex(1, row);
	delay_ms(1000);


}



/*
*@���ܣ��ε�
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
*@���ܣ���~
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
