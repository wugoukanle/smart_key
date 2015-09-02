/**********************************************************************
*												smart key
*										 Module��������
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       ����     2015-06-018 14:55     1.0             build this file 
**********************************************************************/


#include "stm32f10x.h"

#include "nvic.h"
#include "delay.h"

#include "oled.h"
#include "bmp.h"

#include "usart.h"
#include "rfid.h"
#include "bluetooth.h"

#include "usb.h"

#include "timer.h"

#include "Globle_var.h"

#include "Li_Power.h"

#include "RTC_time.h"

#include "sw_key.h"

#include "wkup.h"


void ST_Idle(void);


void GPIO_Configuration(void);

void USB_BIT_Init(void);

void bluetooth_control_Init(void);

void max232_control_Init(void);

void oled_control_Init(void);

void buzzer_control_Init(void);

void coil_control_Init(void);



/*��������ȫ�ֱ���*/
	uint8_t char_buf[50];
	uint8_t count;









/*
*@���� ���������
*@����	��
*@����	��
*/
int main(void)
{
	

	
	
	
	
  /*���Ա�������*/	
	/*******************************************/
	uint8_t t=' ';
	uint8_t usb_code[6]={0x55,0x7A,0xBB,0x01,0x00,0x01};//��λ������������λ�����ʹ�������Խ�����ʼ������
  uint8_t p[6]={0x30,0x31,0x32,0x33,0x34,0x35};
	uint8_t hanzi[11];
	uint8_t *phz;
	uint32_t flash_data;
	uint8_t flash_temp[4];
	uint8_t status;
  uint32_t addr;
	uint8_t nChar;
// 	uint8_t char_buf[50];
// 	uint8_t count;

	uint32_t batt_test=0;
	/*********************************************/
	
	//�����ж��������λ���� 0x5000�����ڲ�flash��ǰ20K
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000);		


	
	
	SystemTick_Init();
	NVIC_Configuration();
		

	//������ʼ��
	Init_Sw_Key_Io();
 	Exit_Line_Init();	



	EXTILine_Mask(EXTI_Line5);
	EXTILine_Mask(EXTI_Line8);	
	EXTILine_Mask(EXTI_Line15);	

	//���������������������3m��������ֹػ�״̬���������3m�����˳�ͣ��ģʽ������
	WKUP_Init();		
	
	EXTILine_Unmask(EXTI_Line5);
	EXTILine_Unmask(EXTI_Line8);
	EXTILine_Unmask(EXTI_Line15);
	
	
	
  //���û�н���ͣ��ģʽ��������ʾ
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ
	
	
  longpress=0;
	stop_flag=0;
	
	SPI_FLASH_Init();
	//�������к���ʾlogo
	OLED_Init();
	SPI_FLASH_Init();	
	
	
	OLED_Clear();	
	OLED_DrawBMP(32,2,48,4,hui);
	OLED_ShowUnite(48,2,"�ϿƼ�");		
	//delay_ms(800);	




	
	USB_BIT_Init();
	
	//Ϊ���͹��ĵĿ���λ
	/****************************************/
	bluetooth_control_Init();

	max232_control_Init();

	oled_control_Init();
	OLED_ON;
	
	buzzer_control_Init();
	BUZZER_OFF;

	coil_control_Init();	
	COIL_NO_ACTUATE;
	/****************************************/
	
	





	Init_Timer();
	
	//ʵʱʱ�ӳ�ʼ��
	RTC_init();
	
	//��ʼ�����������Ź�
	//iwdg_init();
	
	//����1 USB��ʼ��������
	Uart1_Init();
	USART1_Configuration();
	
	//����2 RFID��ʼ��������
	Uart2_Init();
	USART2_Configuration();	
	
	//����3 bluetooth��ʼ��������
	Uart3_Init();
	USART3_Configuration();	
	bluetooth_Configuration();	//������EN��BRST����
	CLR_BT_EN;	//��������
	

	
	
	ADC_GPIO_Configuration();
	ADC_Configuration();
	

	
	//flash����ָ���ʼ��
	//SPI_FLASH_WriteWord(INIT_FLAG,1);   //�˴�������flash
 	SPI_FLASH_WriteWord(INIT_FLAG,0);		  //�˴����ز���flash
	
	if(SPI_FLASH_ReadWord(INIT_FLAG) ==0x01)		//����ʼ����־����1���ͳ�ʼ��POINT��ROW
	{
	  mykey_init();		//��ʼ��Կ�ײ�����ͷָ�� ---����趨��ʲô�������������
	}	
	


	

	
// 	 OLED_ClearArea(0,4,128,6);	
// 	 OLED_ShowUnite(24,4,"��ʼ������");		
// 	 mac_query_result=bluetooth_mac();
	
	
	
	
	
	/*��ʱ��������*/
	/**********************************************/
// 	GPIO_Configuration();
// 	while(1)
// 	{
// 		GPIO_SetBits(GPIOB,GPIO_Pin_0);
// 		delay_ms(1000);
// 		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
// 		delay_ms(3000);	
// 	}
	/**********************************************/

	
	/*OLED����*/
	/**********************************************/
//   while(1)
//   {
// 		OLED_Clear();
// 		OLED_ShowChinese(0,0,0);  //��
// 		OLED_ShowChinese(18,0,1); //��
// 		OLED_ShowChinese(36,0,2); //԰
// 		OLED_ShowChinese(54,0,3); //��
// 		OLED_ShowChinese(72,0,4); //��
// 		OLED_ShowChinese(90,0,5); //��
// 		OLED_ShowChinese(108,0,6);//��
// 		OLED_ShowString(0,3,"1.3' OLED TEST");
// 		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
// 	  //OLED_ShowString(20,4,"2014/05/01");  
// 		OLED_ShowString(0,6,"ASCII:");  
// 		OLED_ShowString(63,6,"CODE:");  
// 		OLED_ShowChar(48,6,t);//��ʾASCII�ַ�	   
// 		t++;
// 		if(t>'~')
// 			t=' ';
// 		OLED_ShowNum(103,6,t,3,16);//��ʾASCII�ַ�����ֵ 	
// 			
//     delay_ms(8000);
//     //OLED_Clear();
// 		
// 		OLED_DrawBMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
// 		delay_ms(800);
// 		
//     //OLED_Display_On();
//     //delay_ms(800);		
// 		
// 		OLED_DrawBMP(0,0,128,8,BMP2);
// 		delay_ms(800);	
// 		
//    //OLED_Display_Off();	
//    //delay_ms(800);				
//   }
	/**********************************************/
	
	
	
	
	/*OLED+�ֿ����*/
	/**********************************************/	
// 	phz="�ֿ�";
// 	hanzi[0]=*phz;
// 	hanzi[1]=*(phz+1);
// 	hanzi[2]=*(phz+2);
// 	hanzi[3]=*(phz+3);
// 	hanzi[4]=*(phz+4);	
// 	
// 	
// 	OLED_ShowChinese(0,0,"��");
// 	OLED_ShowChinese(16,0,"��");
// 	
// 	OLED_ShowUnite(0,2,"��ܲ���");
// 	OLED_ShowUnite(0,4,"ABCDEF");		
// 	OLED_ShowUnite(0,6,"123456");		
	
	
	/**********************************************/	
	
/*
		STM32F103C8������3��
		����1ͨ��USB�ӳ����������λ��PCͨ��
		����2ͨ�����ڶ�RFID������
		����3ͨ��������PDAͨ��
*/	
	
	
 	/*����1 USB����*/
	/**********************************************/
	
// 	while(1)
// 	{
// 		UART1nSendChar(1,"12345",5);
// 		delay_ms(1000);
// 	}
	//UART1nSendChar(1,usb_code,6);	
	
	//USB_Cheak_init();
	
	//Test_Usb_Data();


// 	UART1SendwordHex(1, 0x123456);

//   UART1nSendChar(1,"����",5);	

// 	while(1);
	
	/**********************************************/
	
	
	
	
 	/*����2 RFID����*/
	/**********************************************/
// 	while(1)
// 	{
// 			OLED_ShowUnite(32,2,"RFID");
// 			delay_ms(1000);	
// 			UART1nSendChar(1,"12345",5);
// 			//OLED_Clear();	    //����
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			m_UB2;
// 			if(nChar ==5) 
// 			{
// 				if(Rfid_Buff[0]==0&&Rfid_Buff[1]==0&&Rfid_Buff[2]==0&&Rfid_Buff[3]==0)
// 					;
// 				else		//������һ����Ϊ��
// 				{
// 					UART1nSendChar(1,Rfid_Buff,5);
// 					OLED_ShowNumHex(32,4,Rfid_Buff[0],2,16);
// 					OLED_ShowNumHex(48,4,Rfid_Buff[1],2,16);
// 					OLED_ShowNumHex(64,4,Rfid_Buff[2],2,16);
// 					OLED_ShowNumHex(80,4,Rfid_Buff[3],2,16);
// 					delay_ms(1000);						
// 					break;
// 				}
// 			}
// 	}
// 	while(1);		
	
	
	
	
	
	/**********************************************/	
	
	
	
	
 	/*����3 bluetooth����*/
	/**********************************************/
// 	CLR_BT_EN;
// 	CLR_BT_BRTS;
// 	
// 	while(1)
// 	{
// 		UART3nSendChar(3,"012345",6);
// 		delay_ms(1000);
// 	}
// 		while(1)
// 		{
// 			UART3nSendChar(3,"TTM:MAC-?",9);

// 			count=UART3GetCount(3);
// 			UART3nReadChar(3,char_buf,count);
// 			
// 			
// 			
// 			
// 			UART1nSendChar(1,char_buf,count);

// 			delay_ms(1000);
// 		}

	/**********************************************/	
	
	
	/*flash����*/
	/**********************************************/	
	
	
//   SPI_FLASH_WriteWord(AUTHOR_ROW,0x456789);
// 	
// 	SPI_FLASH_BufferRead(flash_temp,AUTHOR_ROW,4);	
// 	
// 	flash_data=SPI_FLASH_ReadWord(AUTHOR_ROW);

// 	UART1nSendChar(1,flash_temp,4);	
//   
// 	OLED_ShowNum(0,6,flash_data,8,16);	
// 	
// 	//SPI_FLASH_WriteEnable();
//   status=SPI_FLASH_ReadStatusReg();
// 	OLED_ShowNum(112,6,status,1,16);	
	
//  SPI_FLASH_BulkErase();
	
// 	SPI_FLASH_BufferWrite("98765",0x100002,6);
// 	SPI_FLASH_BufferRead(p,0x100002,6);
	
	
// 	SPI_FLASH_Write("98765",0x1f0005,6);
// 	SPI_FLASH_Read(p,0x1f0005,6);	
// 	while(1)
// 	{
// 		UART1nSendChar(1,p,6);	
// 		delay_ms(1000);
// 	}
	
	/**********************************************/	
	


	//�˵�����
	/**********************************************/	
	
  //Main_Lcd_Show();   //��������ʾ	
	//Main_Lcd_Show2();	
	
// 	mode=1;
// 	
// 	OLED_ShowChar(0,0,'A');
// 	
// 	OLED_ShowChinese(0,2,"��");
// 	
// 	OLED_ShowUnite(0,4,"��ܲ���");
// 	
// 	while(1);
// 	
// 	mode=0;
// 	delay_ms(1000);

	//ShowMenu(HsMenu);
	
	
// 	Choose_MenuOp1();

//  Choose_MenuOp4(); 

// 	Choose_MenuOp4_1();
//  Choose_MenuOp4_2();
// 	Choose_MenuOp4_22();
// 	Choose_MenuOp4_3();
// 	Choose_MenuOp4_4();

// 	Choose_MenuOp5();

// 	Choose_MenuOp5_1();
// 	Choose_MenuOp5_3();
// 	Choose_MenuOp5_4();	

// 		while(1)
// 		{
// 			Main_Oled_Time();	
// 			Main_Oled_Power();
// 			delay_ms(1000);
// 		}	

	

// 		while(1);
	/**********************************************/		
	
	
	
	
	
	//����������
  /**********************************************/	
// 	while(1)
// 	{
// 		BUZZER_OFF;
// 		delay_ms(1000);
// 		
// 		BUZZER_ON;
// 		delay_ms(1500);	
// 	}
	
	/**********************************************/		
	
	
	
	
	
	//Э�����
	/**********************************************/	
	//OLED_Clear();
// 	OLED_ShowUnite(32,2,"ͨ����..");
// 	OLED_ShowUnite(0,4,"��ܲ���");
// 	delay_ms(1000);
// 	addr=0xFFFF;
// 	OLED_ShowNumHex(0,6,addr,8,16);
	
	//USB_Cheak_init();
  //USB_transmit();

//  		bluetooth_uart_switch=1;
// 		Test_Usb_Data();
//  		while(1);


	


//   SPI_FLASH_WriteWord(AUTHOR_POINT,AUTHOR_START);		

//   addr= SPI_FLASH_ReadWord(AUTHOR_POINT);   

//   OLED_ShowNum(0,0,addr,8,16);
	/**********************************************/	
	
	
	
	
	//ADC����
	/**********************************************/		
	
// 	while(1)
// 	{
// 		OLED_Clear();
// 		batt_test=ADC_GetConversionValue(ADC1);
// 		OLED_ShowNum(24,2,batt_test,8,16);
// 		delay_ms(1000);
// 	}
// 	
	
	
	/**********************************************/		
	
	
	
	//��ʱ��2����
	/**********************************************/
	
	
// 	GPIO_Configuration();
// 	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
// 	delay_ms(10);
// 	GPIO_SetBits(GPIOB,GPIO_Pin_6);
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,1,Do_TimeOut,THREE_MIN);
// 	while(g_WaitTimeOut == 0)
// 	{
// 		GPIO_ResetBits(GPIOB,GPIO_Pin_6);

// 	}	
//   KillTimer(TIMER_TIMEOUT);	
// 	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
// 	while(g_WaitTimeOut == 0)
// 	{
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			if(nChar ==5)
// 			{
// 					OLED_ShowUnite(32,80,Rfid_Buff);
// 					++rfidrow;
// 					delay_ms(1000);		
// 					for(i=0;i<4;i++)
// 					{
// 							for(k =0;k<40;k++)
// 								msDelay(500);
// 							WritEDAta(CAIMA_START+8*rfidrow+4+i,Rfid_Buff[i]);
// 					}				
// 					break;
// 			}

// 	}	
//   KillTimer(TIMER_TIMEOUT);		
	/**********************************************/	
	
	
	
	
	
	
	
// 	OLED_ClearArea(0,4,128,6);
// 	OLED_ShowUnite(24,4,"��ʼ�����");		
// 	delay_ms(100);
	
	
	//bluetooth_uart_switch=0;
// 	for(int iter=0;iter<3;iter++)
// 		Power_Cheak();	

  //��ѭ������
	g_State=ST_IDLE;
	while(1)																					 
	{   
		switch (g_State)				//g_State��һ��ȫ�ֺ���
		{
			
			 case ST_IDLE:    			//#define  ST_IDLE          0XFF
			 ST_Idle();		    			//����ʱ��״̬��⣬�ɸı�g_State
			 break;			
			
			 case ST_USB: 					//#define  ST_USB	          0XFD         
			 USB_transmit();				//USB����
			 break;
			 	 
// 			 case ST_KEY:
// 			 Key_Trismit();  				//��Կ�׵�RFID���ж��Ƿ��Ѿ�������
// 			 break;
			 
			 case ST_LCDMENU:   		//#define  ST_LCDMENU      0XFA			��ѡ��˵�����
			 Choose_MenuOp1();
			 break;
			 
			 default:break;

			}
	 }		
}


/*
*@���ܣ��ڿ���״̬�ļ�������
*1.����Ƿ��а�������
*2.����Ƿ����RFID
*3.����Ƿ���ܵ����ڴ�����ָ��
*4.����Ƿ���USB���ӵ��������
*5.���﮵�ص�ѹ�Ƿ�͵�
   
��5�����񰴸��Ե�ʱ����ת�����������޳�ͻ
*/
void ST_Idle(void)
{
  
  uint16_t ss_num=0;
	uint16_t power_num=0;
	uint16_t usb_bit;
	
  uint8_t key_r=KEYNO;
	uint8_t usb_r=0;
	uint8_t bt_r=0;
	
	uint8_t time_num=0;
	uint8_t p_c=0;
  uint8_t ss_flag=0;
	
	
  ResetKey();		//KeyNum��һ��ȫ�ֱ���
	//Init_KeyNumuber();		//Key_Num =KEYINIT
	
	//����flsah�е���Ϣ���������˵��е���Ϣ
	SPI_FLASH_Read(HsMenu[1].Name,GROUP_INFORMATION,10);
	SPI_FLASH_Read(HsMenu[2].Name,KEY_INFORMATION,8);	
	
	

// 	//for debug
// 	SPI_FLASH_Read(item,GROUP_INFORMATION,10);
// 	UART1nSendChar(1,item,10);
// 	delay_ms(1000);



// 	//for dedug
// 	SPI_FLASH_Read(item,KEY_INFORMATION,8);
// 	UART1nSendChar(1,item,8);			
// 	delay_ms(1000);		
	
	
	
	
	//for dedug
// 	UART1nSendChar(1,HsMenu[1].Name,10);			
// 	delay_ms(1000);				
// 	UART1nSendChar(1,HsMenu[2].Name,8);			
// 	delay_ms(1000);		
	
	
	
  OLED_Clear();	
	ShowMenuNoInverse(HsMenu);	
	Main_Oled_Power();
	Main_Oled_Time();
  //Choose_MenuOp1();   //��������ʾ	


  while(1)
  {
// 		if(g_State!=ST_IDLE)
// 			return;
		
		if(time_num==20)
		{
			Main_Oled_Time();
			time_num=0;
		}
		++time_num;
		delay_ms(50);
		
		
		//�������¼�⣬����а����Ͱ��¾ͽ���˵�����
    key_r =GetKey();	 //�ж��Ƿ��а�������		
		if(ss_flag==0&&key_r==KEYNO)  //��ʱ����
		{
			ss_num++;

			if(ss_num>=choosetime*6)//���³���10����  300/50=6
			{
				OLED_Display_Off();
				ss_flag=1;
				ss_num=0;
			}
		}
		if(ss_flag==1&&(key_r ==KEYCLEAR||key_r ==KEYSURE||key_r ==KEYDOWN||key_r == KEYUP))
		{
			OLED_Display_On();
			ss_flag=0;
			ResetKey();
			key_r=KEYNO;
		}		
    if(key_r !=KEYNO&&key_r !=KEYINIT)
    {
       //ResetKey();  //���갴��ֵ�󣬽�����ֵ��λ

		   g_State =ST_LCDMENU;
		   return; 
							//�����ⰴ�������ѡ��˵�
    }

		
				
// 		//RFID���
// 		nChar =Read_RfidData(2,Rfid_Buff,0xff);			//�ǳ���Ҫ��һ�㣬�����Trismit�����Ĺؼ�
// 		if(nChar >=3)
// 		{
// 		 g_State =ST_KEY;return;	//���ID��Ϣ����,���������rfid���ݣ���g_State��Ϊst_key
// 		}
		
		
		
		
		//�����λ��������λ������Ӳ˵��������USBͨ����
		usb_r =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//USB����1��g_Buff��һ��ȫ�ֱ�����#define  MAX_BUFF   520 
		if(usb_r==7&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0d&&g_Buff[3]==0x02&&g_Buff[4]==0x00&&g_Buff[5]==0x01&&g_Buff[6]==0xD1)//�������0x00�ͷ���һ����Ȩ
		{	
			usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
			if(usb_bit==1)			//USB����
			{
				bluetooth_uart_switch=0;				
				pc_to_slave();			//��λ��������λ���Ĵ���
			  g_State =ST_USB; 		//����USBͨ��

				return;		  
			}
		}		


	
		
		bt_r =UART3ReadFrm(3,g_Buff,MAX_BUFF);		//��������2��g_Buff��һ��ȫ�ֱ�����#define  MAX_BUFF   520 
		if(bt_r==7&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0d&&g_Buff[3]==0x02&&g_Buff[4]==0x00&&g_Buff[5]==0x01&&g_Buff[6]==0xD1)//�������0x00�ͷ���һ����Ȩ
		{	
				bluetooth_uart_switch=1;				
				pc_to_slave();			//��λ��������λ���Ĵ���ʹ�ý��ܵ���g_Buff
						
// 				m_UB3;
// 				m_UB3.RBuff;
	
				
			  g_State =ST_USB; 		//����USBͨ��

				return;		  
		}	


	 usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
	 if(usb_bit==1)     //USB�Ѿ�����,���������л���USB
		 bluetooth_uart_switch=0;				
		
		
		
		//��ص������
		
		power_num++;		
		if( power_num==0x15)		//0x15*20ʱPower_Cheak()�Ż᷵����Чֵ 
		{
			power_num =0;
			IWDG_Feed();
			
			Main_Oled_Power();		//���µ�ص���
			
			p_c=Power_Cheak();		//�˺�������20�βŷ�����Чֵ
			if(p_c==4)  //����ص�ѹ�������ص�ѹ���ͣ������ػ�ģʽ����������ʾ
			{
				
// 				Main_Oled_Time();
// 				Main_Oled_Power();				
				
				OLED_Show_LowBattry();  
				
				//��ʱ1s��ػ�
				g_WaitTimeOut = 0;
				SetTimer(TIMER_TIMEOUT,100,Do_TimeOut,THREE_MIN);   
				while( g_WaitTimeOut == 0 )
				{
					IWDG_Feed();
				}
				KillTimer(TIMER_TIMEOUT);
					
				Sys_Enter_Standby();
			}
		}


  }
}








void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	//���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}



void USB_BIT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	//���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void bluetooth_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	
}


void max232_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_6);
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
  GPIO_SetBits(GPIOB,GPIO_Pin_1);
}




