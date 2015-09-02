/**********************************************************************
*												smart key
*										 Module：主程序
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       沈涛     2015-06-018 14:55     1.0             build this file 
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

#include "spi_flash.h"

void ST_Idle(void);


// void GPIO_Configuration(void);

void USB_BIT_Init(void);

void bluetooth_control_Init(void);

void max232_control_Init(void);

void oled_control_Init(void);

void buzzer_control_Init(void);

void coil_control_Init(void);



/*调试所用全局变量*/
	uint8_t char_buf[50];
	uint8_t count;









/*
*@功能 主函数入口
*@参数	空
*@返回	空
*/
int main(void)
{
	

	
	
	
	
  /*测试变量部分*/	
	/*******************************************/
	uint8_t t=' ';
	uint8_t usb_code[6]={0x55,0x7A,0xBB,0x01,0x00,0x01};//下位机反复的向上位机发送此命令，用以建立初始化连接
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
	
	
	
	uint8_t bt_count=0;
	uint8_t bt[50]={0x54,0x54,0x4D,0x3A,0x42,0x50,0x53,0x2D,0x31,0x31,0x35,0x32,0x30,0x30};
	
	uint8_t usb_count=0;
	uint8_t usb[50]={0};	
	
	/*********************************************/
	
	uint8_t fu_enable=1;
	
	
	//设置中断向量表的位置在 0x3000
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);		


	
	
	SystemTick_Init();
	NVIC_Configuration();
		

	
	
	USB_BIT_Init();
	
	//为降低功耗的控制位
	bluetooth_control_Init();

	max232_control_Init();
// 	MAX232_ON;

// 	oled_control_Init();
// 	OLED_ON;
	
	buzzer_control_Init();
	BUZZER_OFF;

	coil_control_Init();	
	COIL_NO_ACTUATE;
	
	
	
	OLED_Init();

	Init_Timer();
	
	//实时时钟初始化
	RTC_init();
	
	//初始化并启动看门狗
	//iwdg_init();
	
	//串口1 USB初始化及配置
	Uart1_Init();
	USART1_Configuration();
	
	//串口2 RFID初始化及配置
	Uart2_Init();
	USART2_Configuration();	
	
	//串口3 bluetooth初始化及配置
	Uart3_Init();
	USART3_Configuration();	
	bluetooth_Configuration();	//蓝牙的EN和BRST配置
	CLR_BT_EN;	//开启蓝牙
	
	SPI_FLASH_Init();
	
	
	ADC_GPIO_Configuration();
	ADC_Configuration();
	
	
	//flash区域指针初始化
	//SPI_FLASH_WriteWord(INIT_FLAG,1);   //此次下载清flash
 	SPI_FLASH_WriteWord(INIT_FLAG,0);		  //此次下载不清flash
	
	if(SPI_FLASH_ReadWord(INIT_FLAG) ==0x01)		//当初始化标志等于1，就初始化POINT和ROW
	{
	  mykey_init();		//初始化钥匙操作的头指针 ---如何设定在什么情况下重新设置
	}	
	
 	mac_query_result=bluetooth_mac();
	
	

	
	SPI_FLASH_Write(&fu_enable,FIRMWARE_UPGRADE_FLAG,1);			//固件更新标志置一
	
	
	
	
	
	
	
// 	SPI_FLASH_Write(HsMenu[1].Name,GROUP_INFORMATION,10);	
// 	SPI_FLASH_Write(HsMenu[2].Name, KEY_INFORMATION, 8);		


	/*延时函数测试*/
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

	
	/*OLED测试*/
	/**********************************************/
//   while(1)
//   {
// 		OLED_Clear();
// 		OLED_ShowChinese(0,0,0);  //中
// 		OLED_ShowChinese(18,0,1); //景
// 		OLED_ShowChinese(36,0,2); //园
// 		OLED_ShowChinese(54,0,3); //电
// 		OLED_ShowChinese(72,0,4); //子
// 		OLED_ShowChinese(90,0,5); //科
// 		OLED_ShowChinese(108,0,6);//技
// 		OLED_ShowString(0,3,"1.3' OLED TEST");
// 		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
// 	  //OLED_ShowString(20,4,"2014/05/01");  
// 		OLED_ShowString(0,6,"ASCII:");  
// 		OLED_ShowString(63,6,"CODE:");  
// 		OLED_ShowChar(48,6,t);//显示ASCII字符	   
// 		t++;
// 		if(t>'~')
// 			t=' ';
// 		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
// 			
//     delay_ms(8000);
//     //OLED_Clear();
// 		
// 		OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
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
	
	
	
	
	/*OLED+字库测试*/
	/**********************************************/	
// 	phz="字库";
// 	hanzi[0]=*phz;
// 	hanzi[1]=*(phz+1);
// 	hanzi[2]=*(phz+2);
// 	hanzi[3]=*(phz+3);
// 	hanzi[4]=*(phz+4);	
// 	
// 	
// 	OLED_ShowChinese(0,0,"汉");
// 	OLED_ShowChinese(16,0,"字");
// 	
// 	OLED_ShowUnite(0,2,"你很不错");
// 	OLED_ShowUnite(0,4,"ABCDEF");		
// 	OLED_ShowUnite(0,6,"123456");		
	
	
	/**********************************************/	
	
/*
		STM32F103C8串口有3个
		串口1通过USB接充电器，与上位机PC通信
		串口2通过串口读RFID的数据
		串口3通过蓝牙与PDA通信
*/	
	
	
 	/*串口1 USB测试*/
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

//   UART1nSendChar(1,"汉字",5);	

// 	while(1);
	
	/**********************************************/
	
	
	
	
 	/*串口2 RFID测试*/
	/**********************************************/
// 	while(1)
// 	{
// 			OLED_ShowUnite(32,2,"RFID");
// 			delay_ms(1000);	
// 			UART1nSendChar(1,"12345",5);
// 			//OLED_Clear();	    //清屏
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			m_UB2;
// 			if(nChar ==5) 
// 			{
// 				if(Rfid_Buff[0]==0&&Rfid_Buff[1]==0&&Rfid_Buff[2]==0&&Rfid_Buff[3]==0)
// 					;
// 				else		//至少有一个不为零
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
	
	
	
	
 	/*串口3 bluetooth测试*/
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
	
	
	/*flash测试*/
	/**********************************************/	
	
	
//  SPI_FLASH_WriteWord(AUTHOR_ROW,0x456789);
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
	
// 	SPI_FLASH_CS_LOW();
// 	SPI_FLASH_Write("98765",0x1f0005,6);
// 	SPI_FLASH_Read(p,0x1f0005,6);	
// 	while(1)
// 	{
// 		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
// 		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
// 		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
// 		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
// 		
// 		
// 		//SPI_FLASH_CS_LOW();
// 		UART1nSendChar(1,p,6);
// 		delay_ms(1000);
// 	}
	
	/**********************************************/	
	


	//菜单测试
	/**********************************************/	
	
  //Main_Lcd_Show();   //主界面显示	
	//Main_Lcd_Show2();	
	
// 	mode=1;
// 	
// 	OLED_ShowChar(0,0,'A');
// 	
// 	OLED_ShowChinese(0,2,"我");
// 	
// 	OLED_ShowUnite(0,4,"你很不错");
// 	
// 	while(1);
// 	
// 	mode=0;
// 	delay_ms(1000);

	//ShowMenu(HsMenu);
	
	
// 	Choose_MenuOp1();
//   Choose_MenuOp4();
// 	Choose_MenuOp4_1();
//   Choose_MenuOp4_2();
// 	Choose_MenuOp4_22();
// 	Choose_MenuOp4_3();
// 	Choose_MenuOp5();
// 	Choose_MenuOp5_1();
// 	Choose_MenuOp5_3();
	
// 		while(1)
// 		{
// 			Main_Oled_Time();	
// 			Main_Oled_Power();
// 			delay_ms(1000);
// 		}	

	

// 		while(1);
	/**********************************************/		
	
	
	
	
	
	//蜂鸣器测试
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
	
	
	
	
	
	//协议测试
	/**********************************************/	
	//OLED_Clear();
// 	OLED_ShowUnite(32,2,"通信中..");
// 	OLED_ShowUnite(0,4,"你很不错");
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
	
	
	
	
	//ADC测试
	/**********************************************/		
	
// 	while(1)
// 	{
// 		OLED_Clear();
// 		batt_test=ADC_GetConversionValue(ADC1);
// 		OLED_ShowNum(24,2,batt_test,8,16);
// 		delay_ms(1000);
// 	}
	
	
	
	/**********************************************/		
	
// 		g_WaitTimeOut = FALSE;
//     SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
// 		while(g_WaitTimeOut == FALSE));
// 		KillTimer(TIMER_TIMEOUT);	
// 	
	
	
	
//		MAX232_ON;
	
	//直接开锁测试
	/**********************************************/
// 		while(1)
// 		{
// 			MAX232_OFF;
// 			
// 		g_WaitTimeOut = 0;
//     SetTimer(TIMER_TIMEOUT,50,Do_TimeOut,THREE_MIN);
// 		while(g_WaitTimeOut == 0);
// 		KillTimer(TIMER_TIMEOUT);		

// 			
// 			MAX232_ON;
// 			
// 		g_WaitTimeOut = 0;
//     SetTimer(TIMER_TIMEOUT,30,Do_TimeOut,THREE_MIN);
// 		while(g_WaitTimeOut == 0);
// 		KillTimer(TIMER_TIMEOUT);		
// 			
// 		}

	// ;	
	
	//COIL_NO_ACTUATE;
	
	//蓝牙波特率设置
	/**********************************************/	
	
// 	while(1)
// 	{
// 		usb_count=UART1GetCount(1);
// 		if(usb_count!=0)
// 		{
// 			UART1nReadChar(1,usb,usb_count);
// 			UART1Reset(1);	
// 			UART3nSendChar(3,usb,usb_count);
// 		}
// 		
// 		delay_ms(500);
// 		m_UB3;
// 		bt_count=UART3GetCount(3);			//蓝牙缓冲区接受数据计数
// 		if(bt_count!=0)
// 		{
// 			UART3nReadChar(3,bt,bt_count);	//读取蓝牙缓冲区的数据
// 			UART3Reset(3);			
// 			UART1nSendChar(1,bt,bt_count);	//转从串口USB发送给上位机
// 		}
// 	

// 		delay_ms(1000);
// 	}

// 		while(1)
// 		{
// 			UART3nSendChar(3,"TTM:MAC-?",9);	//读取蓝牙缓冲区的数据
// 			delay_ms(1000);
// 			m_UB3;
// 			bt_count=UART3GetCount(3);			//蓝牙缓冲区接受数据计数
// 			if(bt_count!=0)
// 			{
// 				UART3nReadChar(3,bt,bt_count);	//读取蓝牙缓冲区的数据
// 				UART3Reset(3);	
// 				UART1nSendChar(1,bt,bt_count);	//转从串口USB发送给上位机
// 			}
// 		}


// 		while(1)
// 		{
// 			UART3nSendChar(3,bt,14);	//读取蓝牙缓冲区的数据
// 			delay_ms(1000);
// 			delay_ms(1000);	
// 			delay_ms(1000);				
// 		}
	/**********************************************/		
	
	
	//bluetooth_uart_switch=0;
	

  //主循环测试
	g_State=ST_IDLE;
	while(1)																					 
	{   
		switch (g_State)				//g_State是一个全局函数
		{
			
			 case ST_IDLE:    			//#define  ST_IDLE          0XFF
			 ST_Idle();		    			//空闲时做状态检测，可改变g_State
			 break;			
			
			 case ST_USB: 					//#define  ST_USB	          0XFD         
			 USB_transmit();				//USB传输
			 break;
			 	 
			 case ST_KEY:
			 Key_Trismit();  				//读钥匙的前导码，判断是否已经连接上
			 break;
			 
			 case ST_LCDMENU:   		//#define  ST_LCDMENU      0XFA			多选择菜单操作
			 Choose_MenuOp1();
			 break;
			 
			 default:break;

			}
	 }		
}


/*
*@功能：在空闲状态的几个任务
*1.检测是否有按键按下
*2.检测是否读到RFID
*3.检测是否接受到串口传来的指令
*4.检测是否有USB连接到计算机上
*5.检测锂电池电压是否低电
   
共5个任务按各自的时间轴转动，基本上无冲突
*/
void ST_Idle(void)
{
  
  uint8_t i=0,j=0;
	uint16_t usb_bit;
  uint8_t nChar;
	uint8_t time_flag=0;
  
  KeyNum =KEYNO;		//KeyNum是一个全局变量
	Init_KeyNumuber();		//Key_Num =KEYINIT
	
	//根据flsah中的信息，更改主菜单中的信息
	SPI_FLASH_Read(HsMenu[1].Name,GROUP_INFORMATION,10);
	SPI_FLASH_Read(HsMenu[2].Name,KEY_INFORMATION,8);	
	
	

// 				//for debug
// 				SPI_FLASH_Read(item,GROUP_INFORMATION,10);
// 				UART1nSendChar(1,item,10);
// 				delay_ms(1000);
// 			
// 			

// 				//for dedug
// 				SPI_FLASH_Read(item,KEY_INFORMATION,8);
// 				UART1nSendChar(1,item,8);			
// 				delay_ms(1000);		
	
	
	
	
	//for dedug
// 	UART1nSendChar(1,HsMenu[1].Name,10);			
// 	delay_ms(1000);				
// 	UART1nSendChar(1,HsMenu[2].Name,8);			
// 	delay_ms(1000);		
	
	
	
  OLED_Clear();	
	ShowMenuNoInverse(HsMenu);	
	Main_Oled_Power();
	Main_Oled_Time();
  //Choose_MenuOp1();   //主界面显示	


  while(1)
  {
		
		if(time_flag==20)
		{
			Main_Oled_Time();
			time_flag=0;
		}
		++time_flag;
		delay_ms(50);
		
		
		//按键按下检测，如果有按键就按下就进入菜单操作
    KeyNum =GetKey();	 //判断是否有按键操作		
    if(KeyNum !=KEYNO&&KeyNum !=KEYINIT)
    {
      KeyNum =KEYNO;  //读完按键值后，将按键值复位
			if(shutdown_flag==1)
			{
				 Init_KeyNumuber();
				 OLED_Clear();	 
				 shutdown_flag=0;
				 delay_ms(1000);
			   OLED_ShowUnite(36,2,"开机中...");
			   delay_ms(1000);
				 OLED_Clear();
				 Main_Oled_Power();
				 Main_Oled_Time();
			}
			else if (shutdown_flag==0)
			{
				 g_State =ST_LCDMENU;
				 return; 
			}				//按任意按键进入可选择菜单
    }

		
				
		//RFID检测
		nChar =Read_RfidData(2,Rfid_Buff,0xff);			//非常重要的一点，是理解Trismit（）的关键
		if(nChar >=3)
		{
		 g_State =ST_KEY;return;	//检测ID信息数据,如果读到了rfid数据，则将g_State置为st_key
		}
		
		
		
		
		//检测上位机连接下位机命令，从菜单界面进入USB通信中
		nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//USB串口1，g_Buff是一个全局变量；#define  MAX_BUFF   520 
		if(nChar==7&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0d&&g_Buff[3]==0x02&&g_Buff[4]==0x00&&g_Buff[5]==0x01&&g_Buff[6]==0xD1)//如果返回0x00就发下一条授权
		{	
			usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
			if(usb_bit==1)			//USB插上
			{
				bluetooth_uart_switch=0;				
				pc_to_slave();			//上位机连接下位机的处理
			  g_State =ST_USB; 		//进入USB通信

				return;		  
			}
		}		


// 		nChar =UART1ReadFrm(1,g_Buff,MAX_BUFF);		//USB串口1，g_Buff是一个全局变量；#define  MAX_BUFF   520 
// 		if(nChar==7&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0d&&g_Buff[3]==0x02&&g_Buff[4]==0x00&&g_Buff[5]==0x01&&g_Buff[6]==0xD1)//如果返回0x00就发下一条授权
// 		{	
// 			usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
// 			if(usb_bit==1 && USB_Cheak_init_flag==0)			//USB插上，且非初始化连接
// 			{
// 				bluetooth_uart_switch=0;				
// 				pc_to_slave();			//上位机连接下位机的处理
// 			  g_State =ST_USB; 		//进入USB通信

// 				return;		  
// 			}
// 		}	
		

		
		
		nChar =UART3ReadFrm(3,g_Buff,MAX_BUFF);		//蓝牙串口2，g_Buff是一个全局变量；#define  MAX_BUFF   520 
		if(nChar==7&&g_Buff[0]==0xaa&&g_Buff[1]==0x75&&g_Buff[2]==0x0d&&g_Buff[3]==0x02&&g_Buff[4]==0x00&&g_Buff[5]==0x01&&g_Buff[6]==0xD1)//如果返回0x00就发下一条授权
		{	
				bluetooth_uart_switch=1;				
				pc_to_slave();			//上位机连接下位机的处理；使用接受到的g_Buff
						
// 				m_UB3;
// 				m_UB3.RBuff;
	
				
			  g_State =ST_USB; 		//进入USB通信

				return;		  
		}	


		
		
		//USB连接检测
// 		i++;
// 		if(i ==0x3)
// 		{	
// 			 i=0;
// 			 usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
// 			 if(usb_bit==1 && USB_Cheak_init_flag==1)     //USB已经插上，且将进行初始化连接
// 			 {
// 				 bluetooth_uart_switch=0;
// 				 MAX232_ON;   															//打开MAX3232供电模块--MOS管控制
// 				 g_State =ST_USB; 													//即USB初始插上，则进行初始化连接
// 				 return;		  							
// 			 }
// 			 else if(usb_bit==0)													//USB没有插上，则此次检测完之后将继续进入idle状态
// 			 {
// 				 bluetooth_uart_switch=1;
// 				 g_State=ST_IDLE;
// 				 USB_Cheak_init_flag=1;
// 			 }				 
// 		}
		
	 usb_bit=(USB_CHEAK&(GPIO_Pin_1))>>1;
	 if(usb_bit==1)     //USB已经插上,即从蓝牙切换到USB
		 bluetooth_uart_switch=0;				
		
		
		
		//电池电量检测
		
		j++;		
		if( j==0xfffe)
		{
			j =0;
			IWDG_Feed();
			if(Power_Cheak() ==1)  //检测电池电压，如果电池电压过低，则进入关机模式，并进行提示
			{
				OLED_Show_LowBattry();  
				g_WaitTimeOut = 0;
				SetTimer(TIMER_TIMEOUT,8,Do_TimeOut,THREE_MIN);   
				while( g_WaitTimeOut == 0 )
				{
					IWDG_Feed();
				}
				KillTimer(TIMER_TIMEOUT);
				while(1)
					IWDG_Feed();
			}
		}


  }
}








// void GPIO_Configuration(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//普通推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// }



void USB_BIT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;			//普通推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void bluetooth_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//普通推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	
}


void max232_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//普通推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_6);
}


void oled_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//普通推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_7);
}

void buzzer_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//两个输出端
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}



void coil_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	//两个输出端
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_1);
}




