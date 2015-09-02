/**********************************************************************
*												smart key
*										 Module��FHASHģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-21 20:52     1.0             build this file 
*
* ģ�鹦�ܣ�
*				SPI�ӿڵ�2M��СFLASH��оƬ��W25Q16DVSIG
*				
*	����˵����
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
flashоƬʹ��W25Q16DV
�洢�ռ����

flash��С��2M��24λ��ַ
��ַ��ΧΪ0x000000~0x1fffff

��100����Ȩ��ÿ��12�ֽڣ���1200�ֽ�

*/



/*
flash��Ҫ�洢��Ϣ��
1.��Ȩ�ļ�
2.����
3.������¼
4.Ѳ������
5.��������
6.�ֿ�


��������
ǰ512k����,
�м�1M����Ȩ�ļ������롢������¼��Ѳ������ÿ��256k
��512K�����������������ֿ⣬�����뿪������ռǰ256K����ռһ�룩���ֿ�ռ��256K

1k=0x400

64K=0x10000

128K=0x20000

256K=0x40000

512K=0x80000

1M=0x100000


ǰ512K��ַ��Χ
0x000000~0x07FFFF

��Ȩ��ַ��Χ
0x080000~0x0bFFFF(256k)

�����ַ��Χ
0x0C0000~0x0FFFFF(256k)

###0x100000=1M###

������¼��ַ��Χ
0x100000~0x13FFFF(256k)

Ѳ�������ַ��Χ
0x140000~0x17FFFF(256k)




������ַ��Χ
0x180000~0x19FFFF(128k)

���������ַ��Χ
0x1A0000~0x1BFFFF(128k)


�ֿ��ַ��Χ
0x1C0000~0x1FFFFF(256k)
ʵ����ʼ��ַΪ0x1C01BE
0x1BE=446

*/














/*��Ȩ*/
//2�ֽ��û����+2�ֽ������+4�ֽ�����+8�ֽ��û���+8�ֽ�����+1�ֽڿ���״̬
//����������ͼ������û�����+��Ȩ����
#define AUTHOR_ROW    0x180000  //��Ȩ�û���Ϣ����,���10922��

#define AUTHOR_POINT  0x180004  //��ǰ��Ȩ�û���Ϣ��ָ��

#define AUTHOR_START    0x080000   //��Ȩ�ļ�,10485*25B=262125B=0x3FFED
#define AUTHOR_END      0x0BFFED

//��flash��ַ�ĺ�
#define AUTHOR_LEN				25	 
#define AUTHOR_MAX_NUM		10485			//10485=0x285F



/*����*/
//2���ֽ�Ա�����+2�ֽ������+1���ֽڿ���״̬
#define CAIMA_ROW			0x180008	//������������52428��

#define CAIMA_POINT		0x18000C

#define CAIMA_SUO_POINT 0x180010	//����ʱ����Ĵ洢ָ��

#define CAIMA_START			0x0C0000   
#define CAIMA_END				0x0FFFFC


//��flash��ַ�ĺ�
#define CAIMA_LEN				8	 
#define CAIMA_MAX_NUM		52428



/*��¼*/
//4�ֽ�RFID+6�ֽ�time+1�ֽڿ������
#define RECORD_ROW		0x180014  //������¼���20164��

#define RECORD_POINT  0x180018

#define RECORD_START		0x100000		
#define RECORD_END			0x13FFF4

//��flash��ַ�ĺ�
#define RECORD_LEN				13	 
#define RECORD_MAX_NUM		20164



/*Ѳ��*/
#define XUNJIAN_ROW		0x18001C      //��Ѳ���������13797��

#define XUNJIAN_POINT 0x180020

#define XUNJIAN_START		0x140000		//Ѳ������ͽ��
#define XUNJIAN_END			0x17FFFF	

//��flash��ַ�ĺ�
#define XUNJIAN_LEN				19	 
#define XUNJIAN_MAX_NUM		13797




#define INIT_FLAG       0x180024	//��Ȩָ��ĳ�ʼ��

// #define KEY_ENCODE	    0x180028		//Կ�ױ��룬0x00~0x01(0x0001)����ַ�ŵ� �ߵ�ַ�Ÿߣ�,��ʱֻ��0x00һ���ֽ�



#define GROUP_INFORMATION		0x180030 		//������Ϣ10���ֽ�

#define KEY_INFORMATION		  0x180040		//Կ����Ϣ8���ֽ�

#define UNLOCK_CODE					0x180050		//�������룬4���ֽ�

#define BLUETOOTH_MAC				0x180060		//����MAC��ַ��6���ֽ�

#define FIRMWARE_UPGRADE_FLAG		0x180070		//�̼����±�ʾ��1���ֽ�



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

/*ҳдָ��*/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);


void SPI_FLASH_WriteNoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_FLASH_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_FLASH_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);

/*�����õ���������*/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);


/*���ڲ���flash�еĵ�ַ*/
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



uint8_t ReadData(uint8_t address);		//�ⲿeeprom��ͨ��SPI����ȡ����
void WritEDAta(uint32_t address,uint8_t data);		//ͨ��SPI����д���ݵ�eeprom��


void mykey_init(void);








#endif /* __SPI_FLASH_H */

