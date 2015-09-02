/*******************************************************************************
** �ļ���: 		mian.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.14
** ����: 		wuguoyana
** ��������: 	2011-04-28
** ����:		USART��ʼ����RCC���ã�Ȼ���common.c��ִ�����˵�
** ����ļ�:	stm32f10x.h
** �޸���־��	2011-04-29   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "common.h"

#include "oled.h"
#include "spi_flash.h"
#include "delay.h"

#include "ymodem.h"


/* �������� ------------------------------------------------------------------*/
/* �� ------------------------------------------------------------------------*/
#define LED   GPIO_Pin_8

#define  OLED_ON	          (GPIOB->BSRR = GPIO_Pin_7)	

/* ���� ----------------------------------------------------------------------*/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

/* �������� ------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount);
void LED_Configuration(void);
static void IAP_Init(void);
void KEY_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);

void oled_control_Init(void);

void NVIC_Configuration(void);


void buzzer_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}



void coil_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	//���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}



/* �������� ------------------------------------------------------------------*/

/*******************************************************************************
  * @��������	main
  * @����˵��   ������
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
int main(void)
{
		uint8_t fuf=0;
		uint8_t fu_e=1;			//�̼�����ʹ��
	  uint8_t i=0;
	

		uint8_t hui[32]=
		{0x04,0x04,0xF4,0x54,0x54,0x54,0x54,0xFF,0x54,0x54,0x54,0x54,0xF4,0x04,0x04,0x00,
		0x40,0x30,0x01,0x01,0x39,0x41,0x41,0x45,0x59,0x41,0x41,0x71,0x01,0x08,0x30,0x00};/*"�{",0*/	
	
	
	
	
		SystemTick_Init();
		NVIC_Configuration();
	
	
	
	
	
		oled_control_Init();
		OLED_ON;	
	
		OLED_Init();
	
		SPI_FLASH_Init();		//SPI�ӿڳ�ʼ��
	
    
    FLASH_Unlock();			//STM32�ڲ�Flash ����
	
	
    //LED_Configuration();
	
	
    //���ð���
    KEY_Configuration();//��ť��Ӧ��PB5�ų�ʼ��	
		
    IAP_Init();					//������1��ʼ��,����û�д����жϳ���
	
	
		buzzer_control_Init();
		coil_control_Init();
	
	
	
	
// 		OLED_Clear();
// 		OLED_ShowUnite(24,2,"if updata");
// 		OLED_ShowUnite(28,4,"press OK");		
// 	  delay_ms(500);










// 		while(i<5)
// 		{
// 			OLED_Clear();	
// 			OLED_DrawBMP(32,2,48,4,hui);
// 			OLED_ShowUnite(48,2,"  ��  ");
// 			OLED_ShowUnite(32,4,"  ��  ��");	
// 			delay_ms(500);	
// 			
// 			OLED_Clear();		
// 			OLED_ShowUnite(32,2,"  ��  ��");	
// 			OLED_DrawBMP(32,4,48,6,hui);			
// 			OLED_ShowUnite(48,4,"  ��  ");
// 			delay_ms(500);		
// 	
// 			i++;
// 		}
		
		
// 		OLED_Clear();	
// 		OLED_DrawBMP(32,2,48,4,hui);
// 		OLED_ShowUnite(48,2,"�ϿƼ�");		
// 		delay_ms(800);
		
		
		//���Է���
// 		while(1)
// 		{
// 			Send_Byte(ACK);			//�յ���λ�����͵ĵ�0֮֡����ACK����
// 			Send_Byte(CRC16);		//�����ٷ���C����ʾ�ȴ���λ����������֡
// 		}
	
    //�����Ƿ���
    //if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)  == 0x00)
		
		
		
		/*
		����ʱ�����س���Ĳ��裺
					1.�����ֿ�
					2.����IAP
					3.����Ӧ�ó���
		
		*/
		
		


		//�������ֿ�ʱ���Ƚ�FIRMWARE_UPGRADE_FLAG��־��Ϊ1������������IAP����ʱ��ֱ�ӽ���̼�����
		
		
		SPI_FLASH_Read(&fuf,FIRMWARE_UPGRADE_FLAG,1);
		if(fuf==1)
    {
			
				OLED_Clear();
				//OLED_ShowUnite(16,2,"�̼�������...");
			  OLED_ShowUnite(32,2,"to updata");		
				delay_ms(500);
  

  			//���簴������
        //ִ��IAP�����������Flash����

        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n=              (C) COPYRIGHT 2011 Lierda                             =");
        SerialPutString("\r\n=                                                                    =");
        SerialPutString("\r\n=     In-Application Programming Application  (Version 1.0.0)        =");
        SerialPutString("\r\n=                                                                    =");
        SerialPutString("\r\n=                                   By wuguoyan                      =");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n\r\n");
        Main_Menu();
    }
    //����ִ���û�����
    else
    {
			/*		
			�ڳ�����#define ApplicationAddress    0x8003000 ��*(__IO uint32_t*)ApplicationAddress)
			��ȡ0x8003000��ʼ��0x8003003 ��4���ֽڵ�ֵ, ��Ϊ���ǵ�Ӧ�ó���APP�����ð� �ж���
			���� ������0x08003000 ��ʼ��λ�ã����ж����������һ���ŵľ���ջ����ַ��ֵ
			Ҳ����˵����仰��ͨ���ж�ջ����ֵַ�Ƿ���ȷ���Ƿ���0x2000 0000 - 0x 2000 2000֮�䣩
			���ж��Ƿ�Ӧ�ó����Ѿ������ˣ���ΪӦ�ó���������ļ��տ�ʼ��ȥ��ʼ����ջ�ռ䣬
			���ջ��ֵ���ˣ�˵Ӧ�ó��Ѿ������������ļ��ĳ�ʼ��Ҳִ���ˣ�				
			*/

			
        //�ж��û��Ƿ��Ѿ����س�����Ϊ��������´˵�ַ��ջ��ַ��
        //��û����һ��Ļ�����ʹû�����س���Ҳ�����������ܷɡ�
			
				//*(__IO uint32_t*)ApplicationAddress �� __vector_table[0]��һ����
				//__vector_table[0]��Ӧ�ó���ջ�Ķ�
			
				//X=*(__IO uint32_t*)ApplicationAddress
        //if (( X & 0x2FFE0000 ) == 0x20000000)			
				//Xֻ����0x20000000��0x2001FFFF֮��ʱ���ſ�����(X&0x2FFE0000) == 0x20000000����
			
				//��ջ���ǲ�������0x20000000��ʼ��128K�ķ�Χ�ڣ��������STM32��RAM����
			
        if (( ( *(__IO uint32_t*)ApplicationAddress ) & 0x2FFE0000 ) == 0x20000000)
        {
					
// 						OLED_Clear();
// 						OLED_ShowUnite(4,2,"Execute Program");
// 						delay_ms(500);							
					
					
					
            //SerialPutString("Execute Program\r\n\n");
            //��ת���û�����
            JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
            Jump_To_Application = (pFunction) JumpAddress;

            //��ʼ���û�����Ķ�ջָ��
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);
            Jump_To_Application();		//ִ�и�λ�жϺ���
        }
        else
        {
						OLED_Clear();
						OLED_ShowUnite(4,2,"no user Program");
						delay_ms(500);							
										
            SerialPutString("no user Program\r\n\n");
        }
    }

// 		OLED_Clear();
// 		OLED_ShowUnite(16,2,"please reboot");
// 		delay_ms(500);		
// 		
// 		SerialPutString("please reboot\r\n\n");
//     while (1)
//     {
//     }
}


/*******************************************************************************
  * @��������	LED_Configuration
  * @����˵��   ����ʹ��LED
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void LED_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //ʹ��LED����GPIO��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    //��ʼ��LED��GPIO
    GPIO_InitStructure.GPIO_Pin = LED;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,LED);  //Ϩ��LED2-5
}

/*******************************************************************************
  * @��������	KEY_Configuration
  * @����˵��   ������ʼ��
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void KEY_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    //���ð���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************************************************************
  * @��������	GPIO_Configuration
  * @����˵��   ����ʹ��USART1�����IO�ܽ�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    // ���� USART1 Tx (PA.09) ��Ϊ�������Ų��������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //���� USART1 Tx (PA.10) ��Ϊ�������Ų��Ǹ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
  * @��������	IAP_Init
  * @����˵��   ����ʹ��IAP
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void IAP_Init(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USART1 ���� ------------------------------------------------------------
         USART1 ��������:
          - ������      = 115200 baud
          - �ֳ�        = 8 Bits
          - һ��ֹͣλ
          - ��У��
          - ��Ӳ��������
          - ���ܺͷ���ʹ��
    --------------------------------------------------------------------------*/
    USART_InitStructure.USART_BaudRate = 115200;//115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    GPIO_Configuration();
    USART_Init(USART1, &USART_InitStructure);
    // ʹ�� USART1
    USART_Cmd(USART1, ENABLE);
}



/*******************************************************************************
  * @��������	Delay
  * @����˵��   ����һ����ʱʱ��
  * @�������   nCount: ָ����ʱʱ�䳤��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}


void oled_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_7);
	
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;   //����һ���ṹ��
	
  /*�ж����ȼ�����*/	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				//��ʾֻ����Ӧ���ȼ���û����ռ���ȼ�
	
  /*UART1�жϳ�ʼ������*/	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 			//�ж���������
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
}








#ifdef  USE_FULL_ASSERT

/*******************************************************************************
  * @��������	assert_failed
  * @����˵��   �����ڼ�������������ʱ��Դ�ļ����ʹ�������
  * @�������   file: Դ�ļ���
  				line: ������������
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* �û����������Լ��Ĵ������ڱ��������ļ�������������,
       ���磺printf("�������ֵ: �ļ��� %s �� %d��\r\n", file, line) */

    //��ѭ��
    while (1)
    {
    }
}










#endif

/*******************************�ļ�����***************************************/
