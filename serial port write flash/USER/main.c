
#include "stm32f10x.h"

#include "nvic.h"
#include "delay.h"
#include "usart.h"
#include "spi_flash.h"
#include "Globle_Var.h"


uint8_t temp[16];


int main(void)
{
	uint32_t TAddr;
	uint8_t fuf=1;
	
	
	SystemTick_Init();
	NVIC_Configuration();
	
	
	//串口1 USB初始化及配置
	Uart1_Init();
	USART1_Configuration();	
	
	SPI_FLASH_Init();
	
	
// 	SPI_FLASH_BulkErase();
// 	SPI_FLASH_Write("0123456789",GWriteAddr,11);
// 	
// 	TAddr=GWriteAddr;
//   while (TAddr<(0x1C01BE+32*5))
//   {
// 		SPI_FLASH_Read(temp,TAddr,16);
// 		UART1nSendChar(1,temp,16);
// 		UART1SendChar(1,'\r');
// 		delay_ms(1000);
// 		TAddr+=16;
//   }
// 	while(1);

		//操作步骤
		//波特率设置为9600
		//友善串口助手设置为发送为hex，接收为ASCII，并设置格式化发送
		//然后将字库发送过去（复制粘贴到发送去）









		
		UART1nSendChar(1,"串口助手设置为：9600波特率",26);	
		UART1SendChar(1,'\r');
		UART1SendChar(1,'\n');
		
		UART1nSendChar(1,"             ",13);
		UART1nSendChar(1,"串口助手设置为：ASCII接收",25);	
		UART1SendChar(1,'\r');
		UART1SendChar(1,'\n');
		
		
		UART1nSendChar(1,"             ",13);
		UART1nSendChar(1,"串口助手设置为：HEX发送",23);
		UART1SendChar(1,'\r');
		UART1SendChar(1,'\n');

		delay_ms(1000);
		UART1nSendChar(1,"请发送字库",11);

		SPI_FLASH_BulkErase();
		
		
		SPI_FLASH_Write(&fuf,FIRMWARE_UPGRADE_FLAG,1);			//固件更新标志置一
		
		
		
		while(1)
		{
				if(GWriteAddr==0x1ffffe)
				{
						UART1nSendChar(1,"字库下载结束",13);		//预计时间为6.32分
						break;
				}
		}
}


