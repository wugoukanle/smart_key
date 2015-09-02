/**********************************************************************
*												smart key
*										 Module������ͨ��ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-21 18:07     1.0             build this file 
*
* ģ�鹦�ܣ�
*				����ͨ�ţ�ͨ��ת�Ӱ���PCͨ�ţ��������MAX232��ƽת��
*				ʹ�ô���1

*	����˵����
* -----------------------------------------------------------
*       GND   ��Դ��
*       VCC   ��5V
*       TX1   ��PA9
*       RX1   ��PA10              
*
*				USB_BIT��PA1
* ----------------------------------------------------------
*				
**********************************************************************/





#include "usart.h"
#include "iwdg.h"

UARTBUFF1  m_UB1;
uint8_t    m_Uart_State1;


/*
���ڵ����÷�Ϊ3��

1.���ڳ�ʼ��������ʱ�ӿ�����GPIO���ã�

2.���ڲ�������

3.�����жϿ�����ͳһ����nvic.c�У�

4.�����жϴ�������ͳһ����stm32f10x_it.c�У�

*/




/*
*@���ܣ�����1��ʼ������
*/
void Uart1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure; 
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	
	//����UART1 TX��PA.9��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//����UART1 RX��PA.10��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	


	//����1�Ľ����жϷ���nvic.c��
	//����USART1�����ж�
// 	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
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
	//������ͬ��ģʽ�²��õ�
  //USART_InitStructure.USART_Clock = USART_Clock_Disable;
  //USART_InitStructure.USART_CPOL = USART_CPOL_Low;
  //USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
  //USART_InitStructure.USART_LastBit = USART_LastBit_Disable;

  //USART_StructInit(USART_InitStructure);
  USART_Init(USART1, &USART_InitStructure);
	
  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /*�����ж�ʹ��*/
}







/*
*@���ܣ��ض���c�⺯��printf��USART1
*/ 
int fputc(int ch,FILE *f)
{

	//����һ���ֽ����ݵ�USART1
	USART_SendData(USART1,(uint8_t) ch);
	
	//�ȴ��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	
	return (ch);

}

/*
*@���ܣ��ض���c�⺯��scanf��USART1
*/ 
int fgetc(FILE *f)
{
		// �ȴ�����1�������� 
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}



/*
*@���ܣ�USB_BIT�����ж�USB�Ƿ����
*/
void Usb_Power_Init_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    					//PC12��USB_BIT
}
 
 

 
 
/*
*@���ܣ�����1�Ľ����жϺ���
*/
// void USART1_IRQHandler(void)
// {
//   uint8_t nChar;
//   if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//NE�ͱ�ʾ�����ݣ���ʾRDR������
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
*���ܣ���λ����1������ͷβָ�룬�൱���建����
*/
void UART1Reset(uint8_t uart)
{		
		//�ȹؽ����ж�
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);            // Disable USART1 RX interrupt
		//UARTBUFF  m_UB1;
		m_UB1.cFlag = 0;
		m_UB1.cRHead = 0;
		m_UB1.cRTail = 0;
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);            // Enable USART1 RX interrupt
}




/*
*@���ܣ�ͨ������1����һ���ַ�
*/
void UART1SendChar(uint8_t uart, uint8_t data)
{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
				USART_SendData(USART1,data);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  
}





/*
*@���ܣ�ͨ������1����n���ַ�
*/
void UART1nSendChar(uint8_t uart,uint8_t *str,uint8_t len)  //���ڷ����ַ���
{
		uint8_t idx;
		//USART_ClearFlag(USART1,USART_FLAG_TC);//�������Ҫ����Ȼ���USART_FLAG_TCһ��ʼ�͵���SET���ͻḲ�ǵ�һ������
		USART_GetFlagStatus(USART1, USART_FLAG_TC);
		for (idx=0;idx<len;idx++)
		{
			USART_SendData(USART1,str[idx]);		
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 		//�ȴ��������
		}
}


/*
*@���ܣ�ͨ������1����һ��32λ��������λ��ǰ
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
*@���ܣ��ȴ����ͣ�ֱ������������\0
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
*@���ܣ������з��ͣ��ȴ����ͣ�ֱ�����,��ǰ���лس�����
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
*@���ܣ��Ӵ���1�Ļ�������һ���ַ�
*/
uint8_t UART1ReadChar(uint8_t uart)
{

	uint8_t val='*';
	m_Uart_State1 = ERR_UART_OK;	    //��⴮�ڻ������Ƿ��������
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
*@���ܣ��Ӵ��ڻ�������ȡ����len�����ݵ�dst��ָ��ַ
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
*@���ܣ���ȡһ��,'\r'����,���ض�ȡ���ַ�������
*/
uint8_t UART1ReadLine(uint8_t uart, uint8_t *pdst, uint8_t max)
{
		uint8_t count = 0,total;
		uint32_t idx,off;

		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		if (m_UB1.cRHead != m_UB1.cRTail)
		{
			//������Ч�����ܳ���
			if (m_UB1.cRHead >= m_UB1.cRTail)
			{
				total  = m_UB1.cRHead - m_UB1.cRTail;
			} 
			else
		  {
				total  = UARTRXLEN1 + m_UB1.cRHead - m_UB1.cRTail ;
			}
			//�ҽ�����
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
						//�������ݣ����Ա�֤"\r\n"������Ӱ����һ�д���
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
*@���ܣ���ȡ��Э���ʽ���͹��������ݣ������н���
*/
uint8_t UART1ReadFrm(uint8_t uart, uint8_t *pdst, uint32_t max)
{
  uint8_t count = 0,total;
  uint8_t idx,off;
  IWDG_Feed();
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		//�ؽ����жϣ��ڽ����ж�USART1_IRQHandler()���ϵĽ����ܵ����ݷ���m_UB1
  if (m_UB1.cRHead != m_UB1.cRTail)
   {
      //������Ч�����ܳ���;�õ�����ʱhead������
      if (m_UB1.cRHead >= m_UB1.cRTail)		//���ͷ����β
      {
        total  = m_UB1.cRHead - m_UB1.cRTail;
      } 
			else
			{
        total  = UARTRXLEN1 + m_UB1.cRHead - m_UB1.cRTail ;		//���β����ͷ
      }
      //�ҽ�����
      if (total>4)//֡��������Ϊ4
      {
          if (m_UB1.RBuff[m_UB1.cRTail]!=(uint8_t)0xaa)		//
          {
              m_UB1.cRTail = (m_UB1.cRTail+1)% UARTRXLEN1;		//UARTRXLENΪ254,cRTail��1
          }
					else
					{   
              off = (m_UB1.cRTail + 1) % UARTRXLEN1;		//cTail����һ��λ�õĵ�ַ
              if (m_UB1.RBuff[off]!=(uint8_t)0x75)
              {
                m_UB1.cRTail = (m_UB1.cRTail+2)% UARTRXLEN1;		//�ٽ�cTail��1
              }
							else		//��ʾ���յ��ĵ�һ����aa���ڶ�����75
							{
                  off = (m_UB1.cRTail + 3) % UARTRXLEN1;	//off��ʾ��3������cRTailû�б�
									count=m_UB1.RBuff[off]+5;		//5��ʾ��ͷ2���ַ���1�������֣�1�����ȣ�0x00
//                   count = m_UB1.RBuff[off]<<8;//���ݳ��ȸ��ֽ�		��ʾȡ���������ĸ�8λ
//                   off = (m_UB1.cRTail + 4) % UARTRXLEN;//off��ʾ��4����
//                   count = count + m_UB1.RBuff[off]+7;//���ݳ��ȵ��ֽڣ�������Ӻ�count��ʾ�������ݳ��ȣ�7��ʾ2����ͷ�ַ���1�������֣�2�����ȣ�0x00,����λ
                  if (count <= total && count < max)	//total�ǻ�������Ч���ݵĳ��ȣ�count����������֡�ĳ���,������Զ��count<total
                  {
                      //���ݳ�������һ֡��Ҫ��
                      //total = 0;//��¼У���,total����һ�ֺ���
                      for (idx=0; idx < max-1 && idx<count;idx++)
                      {
                        off = (m_UB1.cRTail + idx) % UARTRXLEN1;		//cRTailҲû�б�
                        pdst[idx] = m_UB1.RBuff[off];		////�����ܻ������İ�����֡����pdst����
                        //total ^= pdst[idx];//����У���
                      }
//                      if (total==0)//У����ȷ���˴�Ϊ��У��
                          m_UB1.cRTail = (off + 1) % UARTRXLEN1;		//У����ȷ�ͽ�cRTail������������֡����һ��λ��
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
	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	  IWDG_Feed();
    return count;
}



/*
*@���ܣ����ػ��������ֽ���
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





