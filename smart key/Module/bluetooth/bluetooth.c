/**********************************************************************
*												smart key
*										 Module������ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-21 18:15     1.0             build this file 
*
* ģ�鹦�ܣ�
*				����ģ��ͨ������3��STM32ͨ��
*				

*	����˵����
* -----------------------------------------------------------
*       GND   ��Դ��
*       VCC   ��3.3V
*       TX3   ��PB10
*       RX2   ��PB11
*				EN    ��PA11			ENΪ���ǹ㲥״̬��������
*				BRST  ��PA12			BRSTΪ�ͣ���ʾ����������ȥ����
* ----------------------------------------------------------
*				
**********************************************************************/

#include "bluetooth.h"
#include "iwdg.h"
#include "Globle_Var.h"
#include "spi_flash.h"
#include "delay.h"
#include "usb.h"

//���ڶ�������Ҫ�õ�
#define ERR_UART_OK     0x00   //����
#define ERR_UART_EMPTY  0x01   //��������

UARTBUFF3  m_UB3;
uint8_t    m_Uart_State3;


/*
���ڵ����÷�Ϊ3��

1.���ڳ�ʼ��������ʱ�ӿ�����GPIO���ã�

2.���ڲ�������

3.�����жϿ�����ͳһ����nvic.c�У�

4.�����жϴ�������ͳһ����stm32f10x_it.c�У�

*/

/*
*@���ܣ�����3��ʼ������
*/
void Uart3_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	//USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure; 
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		
	
	//����UART3 TX��PB10��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//����UART3 RX��PB11��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
// 	//USART3 ģʽ����
// 	USART_InitStructure.USART_BaudRate = 115200;
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
// 	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
// 	USART_InitStructure.USART_Parity = USART_Parity_No;
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1;
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
// 	USART_Init(USART3,&USART_InitStructure);
// 	
// 	//ʹ�ܴ���3�����ж� 
// 	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
// 	
// 	USART_Cmd(USART3,ENABLE);//ʹ��USART1

	
	  //����USART1�����ж�
// 	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);	
	
}



/*
*@���ܣ�uart1 ������
*				�����ʣ�115200
*				���ݳ��ȣ�8λ
*				ֹͣλ��1λ
*				����żУ��
*				��Ӳ��������
*				ȫ˫��
*@˵�������Ը����������
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
	//������ͬ��ģʽ�²��õ�
  //USART_InitStructure.USART_Clock = USART_Clock_Disable;
  //USART_InitStructure.USART_CPOL = USART_CPOL_Low;
  //USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
  //USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

  //USART_StructInit(USART_InitStructure);
  USART_Init(USART3, &USART_InitStructure);
	
  /* Enable USART3 */
  USART_Cmd(USART3, ENABLE);
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); /*�����ж�ʹ��*/
}


/*
*@���ܣ���Ҫ��������EN��BRTS��GPIO���ã�
*				���ڿ���������4����SET_BT_EN��CLR_BT_EN��
*				SET_BT_BRTS��CLR_BT_BRTS
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
*	�ض���c�⺯��printf��USART3
*
*/ 
// int fputc(int ch,FILE *f)
// {

// 	//����һ���ֽ����ݵ�USART1
// 	USART_SendData(USART3,(uint8_t) ch);
// 	
// 	//�ȴ��������
// 	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
// 	
// 	return (ch);

// }

/*
*
*	�ض���c�⺯��scanf��USART1
*
*/ 
// int fgetc(FILE *f)
// {
// 		// �ȴ�����3�������� 
// 		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

// 		return (int)USART_ReceiveData(USART3);
// }




/*
*���ܣ���λ����3������ͷβָ�룬�൱���建����
*/
void UART3Reset(uint8_t uart)
{		
		//�ȹؽ����ж�
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt
		//UARTBUFF  m_UB1;
		m_UB3.cFlag = 0;
		m_UB3.cRHead = 0;
		m_UB3.cRTail = 0;
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}





/*
*@���ܣ�ͨ������3����һ���ַ�
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
*@���ܣ�ͨ������3����n���ַ�
*
*@ע�⣺STM32�Ĵ���������
*/
void UART3nSendChar(uint8_t uart,uint8_t *str,uint8_t len)  //���ڷ����ַ���
{
		
		uint8_t idx;
	

	
 		CLR_BT_BRTS;
 		delay_ms(500);
// 	
// 		USART_ClearFlag(USART3,USART_FLAG_TC);	//Ӳ����λ�󣬴��ڷ��͵��׸�����֮ǰû��read SR�Ĳ�������ֱ��write DR��Ҳ����˵��TCû�б������

	
		for (idx=0;idx<len;idx++)
		{
			//USART_GetFlagStatus(USART3, USART_FLAG_TC);
			USART_SendData(USART3,str[idx]);	
// 			delay_ms(20);
// 			delay_us(500);
			while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 		//�ȴ��������

// 			OLED_Clear();
// 			OLED_ShowUnite(16,2,"һ�ֽڷ��ͳɹ�");			
			
		}
		delay_ms(1000);		//���ڷ������ TS (ms) �� L<80ʱ��TS >= T(BLE���Ӽ��,Ĭ��Ϊ200ms)			
 		SET_BT_BRTS;
 		delay_ms(1000);
		//UART3SendChar(3,0);	

}
// /*
// *@���ܣ�ͨ������3����n���ַ�
// */
// void UART3nSendChar(uint8_t uart,uint8_t *str,uint8_t len)  //���ڷ����ַ���
// {
// 		
// 		uint8_t idx;
//  		CLR_BT_BRTS;
// 		for (idx=0;idx<len;idx++)
// 		{
// 			USART_SendData(USART3,str[idx]);		
// 			while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 		//�ȴ��������
// 		}
//  		SET_BT_BRTS;
// }








/*
*@���ܣ��Ӵ���3�Ļ�������һ���ַ�
*/
uint8_t UART3ReadChar(uint8_t uart)
{

	uint8_t val='*';
	m_Uart_State3 = ERR_UART_OK;	    //��⴮�ڻ������Ƿ��������
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
*@���ܣ��Ӵ��ڻ�������ȡ����len�����ݵ�dst��ָ��ַ
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
*@���ܣ���ȡ��Э���ʽ���͹��������ݣ������н���
*/
uint8_t UART3ReadFrm(uint8_t uart, uint8_t *pdst, uint32_t max)
{
  uint8_t count = 0,total;
  uint8_t idx,off;
  IWDG_Feed();
  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);		//�ؽ����жϣ��ڽ����ж�USART1_IRQHandler()���ϵĽ����ܵ����ݷ���m_UB1
  if (m_UB3.cRHead != m_UB3.cRTail)
   {
      //������Ч�����ܳ���;�õ�����ʱhead������
      if (m_UB3.cRHead >= m_UB3.cRTail)		//���ͷ����β
      {
        total  = m_UB3.cRHead - m_UB3.cRTail;
      } 
			else
			{
        total  = UARTRXLEN3 + m_UB3.cRHead - m_UB3.cRTail ;		//���β����ͷ
      }
      //�ҽ�����
      if (total>4)//֡��������Ϊ4
      {
          if (m_UB3.RBuff[m_UB3.cRTail]!=(uint8_t)0xaa)		//
          {
              m_UB3.cRTail = (m_UB3.cRTail+1)% UARTRXLEN3;		//UARTRXLENΪ254,cRTail��1
          }
					else
					{   
              off = (m_UB3.cRTail + 1) % UARTRXLEN3;		//cTail����һ��λ�õĵ�ַ
              if (m_UB3.RBuff[off]!=(uint8_t)0x75)
              {
                m_UB3.cRTail = (m_UB3.cRTail+2)% UARTRXLEN3;		//�ٽ�cTail��1
              }
							else		//��ʾ���յ��ĵ�һ����aa���ڶ�����75
							{
                  off = (m_UB3.cRTail + 3) % UARTRXLEN3;	//off��ʾ��3������cRTailû�б�
									count=m_UB3.RBuff[off]+5;		//5��ʾ��ͷ2���ַ���1�������֣�1�����ȣ�0x00
//                   count = m_UB1.RBuff[off]<<8;//���ݳ��ȸ��ֽ�		��ʾȡ���������ĸ�8λ
//                   off = (m_UB1.cRTail + 4) % UARTRXLEN;//off��ʾ��4����
//                   count = count + m_UB1.RBuff[off]+7;//���ݳ��ȵ��ֽڣ�������Ӻ�count��ʾ�������ݳ��ȣ�7��ʾ2����ͷ�ַ���1�������֣�2�����ȣ�0x00,����λ
                  if (count <= total && count < max)	//total�ǻ�������Ч���ݵĳ��ȣ�count����������֡�ĳ���,������Զ��count<total
                  {
                      //���ݳ�������һ֡��Ҫ��
                      //total = 0;//��¼У���,total����һ�ֺ���
                      for (idx=0; idx < max-1 && idx<count;idx++)
                      {
                        off = (m_UB3.cRTail + idx) % UARTRXLEN3;		//cRTailҲû�б�
                        pdst[idx] = m_UB3.RBuff[off];		////�����ܻ������İ�����֡����pdst����
                        //total ^= pdst[idx];//����У���
                      }
//                      if (total==0)//У����ȷ���˴�Ϊ��У��
                          m_UB3.cRTail = (off + 1) % UARTRXLEN3;		//У����ȷ�ͽ�cRTail������������֡����һ��λ��
//                       else
//                       {
//                           //У���������2���ֽڣ���ʵΪ��֡��ͷ
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
*@���ܣ����ػ��������ֽ���
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
*@���ܣ�����MAC��ַ��ѯ
*
*				��MAC��ַд���ⲿflash��
*/
uint8_t bluetooth_mac(void)
{
	uint8_t j=0,t=0,count=0;
	uint8_t try_num=1;
	uint8_t char_buf[100];
	uint8_t MAC[6];
	uint8_t MAC_NULL[6]={0,0,0,0,0,0};

	UART3Reset(3);

	for(t=0;t<try_num;t++)				//���ೢ��3��
	{
			UART3nSendChar(3,"TTM:RST-SYSTEMRESET",20);		//��������ATָ��
			delay_ms(50);
		
			UART3nSendChar(3,"TTM:MAC-?",10);		//��������ATָ��
			delay_ms(50);		//��ʱ�ȴ������ظ�
		
			count=UART3GetCount(3);
			UART3nReadChar(3,char_buf,count);			//�����������ݵ�char_buf
		
		
			//for debug
// 			UART1nSendChar(1,&count,1);		//���Ͷ�������������
// 			delay_ms(100);			
		
		
			//for debug
// 			UART1nSendChar(1,char_buf,count);		//���Ͷ�������������
// 			delay_ms(100);					
		
			for(j=0;j<count-1;j++)		//��forѭ���˳��Ļ���j=buf_len-1
				if(char_buf[j]=='0'&&char_buf[j+1]=='x')
						break;								//��break�˳��Ļ���j<buf_len-1
				

			if(j<count-1-12)							//�����ǰ�ɹ�
			{
				//for debug
				//UART1nSendChar(1,char_buf+j,12);		//���ַ���ʽ����MAC��ַ
				//delay_ms(100);
				
				j=j+2;
				CharToHex(char_buf+j,MAC,12);		
				
				SPI_FLASH_Write(MAC,BLUETOOTH_MAC,6);
							
				return 1;//�����ٳ�����
				
			}

			
	}
	
	//ʧ��
	if(t>=try_num)
	{
		
		SPI_FLASH_Write(MAC_NULL,BLUETOOTH_MAC,6);
		return 0;
	}
  
}
