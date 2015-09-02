
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
	
	
	//����1 USB��ʼ��������
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

		//��������
		//����������Ϊ9600
		//���ƴ�����������Ϊ����Ϊhex������ΪASCII�������ø�ʽ������
		//Ȼ���ֿⷢ�͹�ȥ������ճ��������ȥ��









		
		UART1nSendChar(1,"������������Ϊ��9600������",26);	
		UART1SendChar(1,'\r');
		UART1SendChar(1,'\n');
		
		UART1nSendChar(1,"             ",13);
		UART1nSendChar(1,"������������Ϊ��ASCII����",25);	
		UART1SendChar(1,'\r');
		UART1SendChar(1,'\n');
		
		
		UART1nSendChar(1,"             ",13);
		UART1nSendChar(1,"������������Ϊ��HEX����",23);
		UART1SendChar(1,'\r');
		UART1SendChar(1,'\n');

		delay_ms(1000);
		UART1nSendChar(1,"�뷢���ֿ�",11);

		SPI_FLASH_BulkErase();
		
		
		SPI_FLASH_Write(&fuf,FIRMWARE_UPGRADE_FLAG,1);			//�̼����±�־��һ
		
		
		
		while(1)
		{
				if(GWriteAddr==0x1ffffe)
				{
						UART1nSendChar(1,"�ֿ����ؽ���",13);		//Ԥ��ʱ��Ϊ6.32��
						break;
				}
		}
}


