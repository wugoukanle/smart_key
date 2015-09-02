/**********************************************************************
*												smart key
*										 Module：FHASH模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-21 20:52     1.0             build this file 
*
* 模块功能：
*				SPI接口的2M大小FLASH，芯片是W25Q16DVSIG
*				
*	连接说明：
* ----------------------------------------------------------
*          PA4-SPI1-NSS  : W25X16-CS  
*          PA5-SPI1-SCK  : W25X16-CLK 
*          PA6-SPI1-MISO : W25X16-DO  
*          PA7-SPI1-MOSI : W25X16-DIO   
* ----------------------------------------------------------
*				
**********************************************************************/


#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"



/*
flash芯片使用W25Q16DV
存储空间分配

flash大小共2M，24位地址
地址范围为0x000000~0x1fffff

存100组授权，每组12字节，共1200字节

*/



/*
flash主要存储信息：
1.授权文件
2.采码
3.操作记录
4.巡检任务
5.开机画面
6.字库


初步分配
前512k留用,
中间1M存授权文件、采码、操作记录、巡检任务，每个256k
后512K存变量、开机画面和字库，变量与开机画面占前256K（各占一半），字库占后256K

1k=0x400

64K=0x10000

128K=0x20000

256K=0x40000

512K=0x80000

1M=0x100000


前512K地址范围
0x000000~0x07FFFF

授权地址范围
0x080000~0x0bFFFF(256k)

采码地址范围
0x0C0000~0x0FFFFF(256k)

###0x100000=1M###

操作记录地址范围
0x100000~0x13FFFF(256k)

巡检任务地址范围
0x140000~0x17FFFF(256k)




变量地址范围
0x180000~0x19FFFF(128k)

开机画面地址范围
0x1A0000~0x1BFFFF(128k)


字库地址范围
0x1C0000~0x1FFFFF(256k)
实际起始地址为0x1C01BE
0x1BE=446

*/














/*授权*/
//2字节用户编号+2字节锁编号+4字节锁码+8字节用户名+8字节组名+1字节开锁状态
//分两条命令传送即传送用户命令+授权命令
#define AUTHOR_ROW    0x180000  //授权用户信息条数,最大10922条

#define AUTHOR_POINT  0x180004  //当前授权用户信息的指针

#define AUTHOR_START    0x080000   //授权文件,10485*25B=262125B=0x3FFED
#define AUTHOR_END      0x0BFFED

//非flash地址的宏
#define AUTHOR_LEN				25	 
#define AUTHOR_MAX_NUM		10485			//10485=0x285F



/*采码*/
//2个字节员工编号+2字节锁编号+1个字节开锁状态
#define CAIMA_ROW			0x180008	//采码的条数最大52428条

#define CAIMA_POINT		0x18000C

#define CAIMA_SUO_POINT 0x180010	//采码时锁码的存储指针

#define CAIMA_START			0x0C0000   
#define CAIMA_END				0x0FFFFC


//非flash地址的宏
#define CAIMA_LEN				8	 
#define CAIMA_MAX_NUM		52428



/*记录*/
//4字节RFID+6字节time+1字节开锁结果
#define RECORD_ROW		0x180014  //操作记录最大20164条

#define RECORD_POINT  0x180018

#define RECORD_START		0x100000		
#define RECORD_END			0x13FFF4

//非flash地址的宏
#define RECORD_LEN				13	 
#define RECORD_MAX_NUM		20164



/*巡检*/
#define XUNJIAN_ROW		0x18001C      //大巡检任务最大13797个

#define XUNJIAN_POINT 0x180020

#define XUNJIAN_START		0x140000		//巡检任务和结果
#define XUNJIAN_END			0x17FFFF	

//非flash地址的宏
#define XUNJIAN_LEN				19	 
#define XUNJIAN_MAX_NUM		13797




#define INIT_FLAG       0x180024	//授权指针的初始化

// #define KEY_ENCODE	    0x180028		//钥匙编码，0x00~0x01(0x0001)（地址放低 高地址放高）,暂时只用0x00一个字节



#define GROUP_INFORMATION		0x180030 		//分组信息10个字节

#define KEY_INFORMATION		  0x180040		//钥匙信息8个字节

#define UNLOCK_CODE					0x180050		//解锁密码，4个字节

#define BLUETOOTH_MAC				0x180060		//蓝牙MAC地址，6个字节

#define FIRMWARE_UPGRADE_FLAG		0x180070		//固件更新标示，1个字节



#define SPI_FLASH_SPI                           SPI1
#define SPI_FLASH_SPI_CLK                       RCC_APB2Periph_SPI1
#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_CS_PIN                        GPIO_Pin_4                  /* PC.04 */
#define SPI_FLASH_CS_GPIO_PORT                  GPIOA                       /* GPIOC */
#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA




#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)




















void SPI_FLASH_Init(void);

void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);

/*页写指令*/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);


void SPI_FLASH_WriteNoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_FLASH_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_FLASH_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);

/*最有用的两个函数*/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);


/*用于操作flash中的地址*/
void SPI_FLASH_WriteWord(uint32_t ReadAddr, uint32_t Data);
uint32_t SPI_FLASH_ReadWord(uint32_t ReadAddr);


uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);

void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
uint8_t SPI_FLASH_ReadByte(void);

uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);

void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);



uint8_t ReadData(uint8_t address);		//外部eeprom中通过SPI总线取数据
void WritEDAta(uint32_t address,uint8_t data);		//通过SPI总线写数据到eeprom中


void mykey_init(void);








#endif /* __SPI_FLASH_H */

