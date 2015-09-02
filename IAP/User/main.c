/*******************************************************************************
** 文件名: 		mian.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 4.14
** 作者: 		wuguoyana
** 生成日期: 	2011-04-28
** 功能:		USART初始化和RCC设置，然后从common.c中执行主菜单
** 相关文件:	stm32f10x.h
** 修改日志：	2011-04-29   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "common.h"

#include "oled.h"
#include "spi_flash.h"
#include "delay.h"

#include "ymodem.h"


/* 类型声明 ------------------------------------------------------------------*/
/* 宏 ------------------------------------------------------------------------*/
#define LED   GPIO_Pin_8

#define  OLED_ON	          (GPIOB->BSRR = GPIO_Pin_7)	

/* 变量 ----------------------------------------------------------------------*/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

/* 函数声明 ------------------------------------------------------------------*/
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
  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}



/* 函数功能 ------------------------------------------------------------------*/

/*******************************************************************************
  * @函数名称	main
  * @函数说明   主函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
int main(void)
{
		uint8_t fuf=0;
		uint8_t fu_e=1;			//固件更新使能
	  uint8_t i=0;
	

		uint8_t hui[32]=
		{0x04,0x04,0xF4,0x54,0x54,0x54,0x54,0xFF,0x54,0x54,0x54,0x54,0xF4,0x04,0x04,0x00,
		0x40,0x30,0x01,0x01,0x39,0x41,0x41,0x45,0x59,0x41,0x41,0x71,0x01,0x08,0x30,0x00};/*"恵",0*/	
	
	
	
	
		SystemTick_Init();
		NVIC_Configuration();
	
	
	
	
	
		oled_control_Init();
		OLED_ON;	
	
		OLED_Init();
	
		SPI_FLASH_Init();		//SPI接口初始化
	
    
    FLASH_Unlock();			//STM32内部Flash 解锁
	
	
    //LED_Configuration();
	
	
    //配置按键
    KEY_Configuration();//按钮对应的PB5脚初始化	
		
    IAP_Init();					//即串口1初始化,但是没有串口中断程序
	
	
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
// 			OLED_ShowUnite(48,2,"  科  ");
// 			OLED_ShowUnite(32,4,"  合  技");	
// 			delay_ms(500);	
// 			
// 			OLED_Clear();		
// 			OLED_ShowUnite(32,2,"  合  技");	
// 			OLED_DrawBMP(32,4,48,6,hui);			
// 			OLED_ShowUnite(48,4,"  科  ");
// 			delay_ms(500);		
// 	
// 			i++;
// 		}
		
		
// 		OLED_Clear();	
// 		OLED_DrawBMP(32,2,48,4,hui);
// 		OLED_ShowUnite(48,2,"合科技");		
// 		delay_ms(800);
		
		
		//测试发送
// 		while(1)
// 		{
// 			Send_Byte(ACK);			//收到上位机发送的第0帧之后发送ACK命令
// 			Send_Byte(CRC16);		//接着再发送C，表示等待上位机发送数据帧
// 		}
	
    //按键是否按下
    //if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)  == 0x00)
		
		
		
		/*
		出厂时，下载程序的步骤：
					1.下载字库
					2.下载IAP
					3.下载应用程序
		
		*/
		
		


		//在下载字库时，先将FIRMWARE_UPGRADE_FLAG标志置为1，所以再下载IAP程序时会直接进入固件更新
		
		
		SPI_FLASH_Read(&fuf,FIRMWARE_UPGRADE_FLAG,1);
		if(fuf==1)
    {
			
				OLED_Clear();
				//OLED_ShowUnite(16,2,"固件更新中...");
			  OLED_ShowUnite(32,2,"to updata");		
				delay_ms(500);
  

  			//假如按键按下
        //执行IAP驱动程序更新Flash程序

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
    //否则执行用户程序
    else
    {
			/*		
			在程序里#define ApplicationAddress    0x8003000 ，*(__IO uint32_t*)ApplicationAddress)
			即取0x8003000开始到0x8003003 的4个字节的值, 因为我们的应用程序APP中设置把 中断向
			量表 放置在0x08003000 开始的位置；而中断向量表里第一个放的就是栈顶地址的值
			也就是说，这句话即通过判断栈顶地址值是否正确（是否在0x2000 0000 - 0x 2000 2000之间）
			来判断是否应用程序已经下载了，因为应用程序的启动文件刚开始就去初始化化栈空间，
			如果栈顶值对了，说应用程已经下载了启动文件的初始化也执行了；				
			*/

			
        //判断用户是否已经下载程序，因为正常情况下此地址是栈地址。
        //若没有这一句的话，即使没有下载程序也会进入而导致跑飞。
			
				//*(__IO uint32_t*)ApplicationAddress 与 __vector_table[0]是一样的
				//__vector_table[0]是应用程序栈的顶
			
				//X=*(__IO uint32_t*)ApplicationAddress
        //if (( X & 0x2FFE0000 ) == 0x20000000)			
				//X只有在0x20000000与0x2001FFFF之间时，才可能有(X&0x2FFE0000) == 0x20000000成立
			
				//即栈顶是不是在以0x20000000开始的128K的范围内，这里便是STM32的RAM区域
			
        if (( ( *(__IO uint32_t*)ApplicationAddress ) & 0x2FFE0000 ) == 0x20000000)
        {
					
// 						OLED_Clear();
// 						OLED_ShowUnite(4,2,"Execute Program");
// 						delay_ms(500);							
					
					
					
            //SerialPutString("Execute Program\r\n\n");
            //跳转至用户代码
            JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
            Jump_To_Application = (pFunction) JumpAddress;

            //初始化用户程序的堆栈指针
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);
            Jump_To_Application();		//执行复位中断函数
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
  * @函数名称	LED_Configuration
  * @函数说明   配置使用LED
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void LED_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //使能LED所在GPIO的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    //初始化LED的GPIO
    GPIO_InitStructure.GPIO_Pin = LED;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,LED);  //熄灭LED2-5
}

/*******************************************************************************
  * @函数名称	KEY_Configuration
  * @函数说明   按键初始化
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void KEY_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    //配置按键
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************************************************************
  * @函数名称	GPIO_Configuration
  * @函数说明   配置使用USART1的相关IO管脚
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    // 配置 USART1 Tx (PA.09) 作为功能引脚并上拉输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //配置 USART1 Tx (PA.10) 作为功能引脚并是浮空输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
  * @函数名称	IAP_Init
  * @函数说明   配置使用IAP
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void IAP_Init(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USART1 配置 ------------------------------------------------------------
         USART1 配置如下:
          - 波特率      = 115200 baud
          - 字长        = 8 Bits
          - 一个停止位
          - 无校验
          - 无硬件流控制
          - 接受和发送使能
    --------------------------------------------------------------------------*/
    USART_InitStructure.USART_BaudRate = 115200;//115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    GPIO_Configuration();
    USART_Init(USART1, &USART_InitStructure);
    // 使能 USART1
    USART_Cmd(USART1, ENABLE);
}



/*******************************************************************************
  * @函数名称	Delay
  * @函数说明   插入一段延时时间
  * @输入参数   nCount: 指定延时时间长度
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}


void oled_control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//两个输出端
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_7);
	
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;   //定义一个结构体
	
  /*中断优先级分组*/	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 				//表示只有响应优先级，没有抢占优先级
	
  /*UART1中断初始化部分*/	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 			//中断向量类型
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
}








#ifdef  USE_FULL_ASSERT

/*******************************************************************************
  * @函数名称	assert_failed
  * @函数说明   报告在检查参数发生错误时的源文件名和错误行数
  * @输入参数   file: 源文件名
  				line: 错误所在行数
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* 用户可以增加自己的代码用于报告错误的文件名和所在行数,
       例如：printf("错误参数值: 文件名 %s 在 %d行\r\n", file, line) */

    //死循环
    while (1)
    {
    }
}










#endif

/*******************************文件结束***************************************/
