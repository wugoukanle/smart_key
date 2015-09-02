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
*				SPI�ӿڵ�2M��СFLASH��оƬ��W25Q16DVSIG,��ַ��Χ��0~0x1FFFFFh
*				
*	����˵����
* -----------------------------------------------------------
*          PA4-SPI1-NSS  : W25X16-CS  
*          PA5-SPI1-SCK  : W25X16-CLK 
*          PA6-SPI1-MISO : W25X16-DO  
*          PA7-SPI1-MOSI : W25X16-DIO   
* ----------------------------------------------------------
*				
* ����˵���������õ�����������SPI_FLASH_BufferWrite������SPI_FLASH_BufferRead����
**********************************************************************/



#include "spi_flash.h"
#include "iwdg.h"


/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF

//��������
uint8_t SPI_FLASH_BUF[4096];			


/*
*@Function Name  : SPI_FLASH_Init
*@Description    : Initializes the peripherals used by the SPI FLASH driver.
*/
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /*!< SPI_FLASH_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  /*!< AFIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* SPI1 configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);
}


/*��������*/


/*
*@Function Name  : SPI_FLASH_SectorErase
*@Description    : Erases the specified FLASH sector.
*@Input          : SectorAddr: address of the sector to erase.
*/
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Sector Erase instruction */
  SPI_FLASH_SendByte(W25X_SectorErase);
  /* Send SectorAddr high nibble address byte */
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}



/*
*@unction Name  : SPI_FLASH_BulkErase
*@Description    : Erases the entire FLASH.
*/
void SPI_FLASH_BulkErase(void)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Bulk Erase instruction  */
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}



/*
*@Function Name  : SPI_FLASH_PageWrite
*@Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
*@Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
*/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
	
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send "Write to Memory " instruction */
  SPI_FLASH_SendByte(W25X_PageProgram);
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     //printf("\n\r Err: SPI_FLASH_PageWrite too large!");
  }

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}


/*
*@���ܣ��޼���дSPI FLASH 
*				����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
*				�����Զ���ҳ���� 
* 			��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
*@������pBuffer:���ݴ洢��
*				WriteAddr:��ʼд��ĵ�ַ(24bit)
*				NumByteToWrite:Ҫд����ֽ���(���65535)
*				CHECK OK
*/
void SPI_FLASH_WriteNoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		SPI_FLASH_PageWrite(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 



/*
*@���ܣ�дSPI FLASH  
*				��ָ����ַ��ʼд��ָ�����ȵ�����
*				�ú�������������!
*@������pBuffer:���ݴ洢��
*				WriteAddr:��ʼд��ĵ�ַ(24bit)
*				NumByteToWrite:Ҫд����ֽ���  
*/
void SPI_FLASH_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    

	secpos=WriteAddr/4096;//������ַ 0~511 for w25x16
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		SPI_FLASH_Read(SPI_FLASH_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SPI_FLASH_SectorErase(secpos*4096);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  //��Ҫд��������д�뻺����SPI_FLASH_BUF���൱��һ��������ʣ��ռ�
			}
			SPI_FLASH_WriteNoCheck(SPI_FLASH_BUF,secpos*4096,4096);//������������д����������,�����ͽ���д�����
		}
		else 
			SPI_FLASH_WriteNoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 
		
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

			pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	 
			
			NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;				  //��һ����������д����
		}	 
	};	 	 
}



/*
*@���ܣ���ȡSPI FLASH  
				��ָ����ַ��ʼ��ȡָ�����ȵ�����
*@������pBuffer:���ݴ洢��
				ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
				NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
*/
void SPI_FLASH_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  
  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();	
}


/*
*@Function Name  : SPI_FLASH_BufferWrite
*@Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
*@Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
*@���䣺	���������Խ��д����ÿ�����ֻ��д256�ֽڵ�Ҫ�����ã�����û�м���
*				��Ϊҳд�������Ҫ��дʹ�ܺ��Ѳ���(��д��λ�õ�������FFh)
*/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;								//д��ַ�����ҳ��ƫ���ֽ���
  count = SPI_FLASH_PageSize - Addr;										//д��ַ�����ҳ��ʣ���ֽ���
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;			//д���������ҳ������������
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;		//д���������ҳ��ģ�������ֽ���

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);		//�Ƚ���һҳ��ʣ�ಿ��д��
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}



/*
*@Function Name  : SPI_FLASH_BufferRead
*@Description    : Reads a block of data from the FLASH.
*@Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
*/
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  
  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*
*@���ܣ���һ��32λ����д��flash�дӸ�����ַ��ʼ��4���ֽڣ���С�˸�ʽ
*@˵���� ��С�˸�ʽ����ַ�ŵͣ��ߵ�ַ�Ÿ�
*			  ��4���ֽ������һ���γ�һ���������൱��uint32_t
*/
void SPI_FLASH_WriteWord(uint32_t WriteAddr, uint32_t Data)
{
		uint8_t temp[4]={0,0,0,0};
	  temp[0]=Data&0xFF;
	  temp[1]=(Data&0xFF00)>>8;
	  temp[2]=(Data&0xFF0000)>>16;
	  temp[3]=(Data&0xFF000000)>>24;
	
		SPI_FLASH_Write(temp,WriteAddr,4);
}

/*
*@���ܣ�����flash�и�����ַ��ʼ������4���ֽ���ϳ�һ��32λ��������С�˸�ʽ
*@˵���� ��С�˸�ʽ����ַ�ŵͣ��ߵ�ַ�Ÿ�
*			  ��4���ֽ������һ���γ�һ���������൱��uint32_t
*/
uint32_t SPI_FLASH_ReadWord(uint32_t ReadAddr)
{
		uint8_t temp[4]={0,0,0,0};
		uint32_t Data=0;
		SPI_FLASH_Read(temp,ReadAddr,4);
		
		Data=temp[0]+temp[1]*(1<<8)+temp[2]*(1<<16)+temp[3]*(1<<24);
		return Data;
}



/*������ID����*/
/*
*@Function Name  : SPI_FLASH_ReadID
*@Description    : Reads FLASH identification.
*@Return         : FLASH identification
*/
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* Read a byte from the FLASH */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}
/*
*@Function Name  : SPI_FLASH_ReadID
*@Description    : Reads FLASH identification.
*@Return         : FLASH identification
*/
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}




/*��������������һ�����ʹ�õģ�����Զ�ȡ��ĳ��ַ��ʼ����������
�����ʹ����󣬼�һ��SPI_FLASH_CS_HIGH����*/



/*
*@Function Name  : SPI_FLASH_StartReadSequence
*@Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
*@Input          : - ReadAddr : FLASH's internal address to read from.
*/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}
/*
*@Function Name  : SPI_FLASH_ReadByte
*@Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
*@Return         : Byte Read from the SPI Flash.
*/
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}








/*
*@Function Name  : SPI_FLASH_SendByte
*@Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
*@Input          : byte : byte to send.
*@Return         : The value of the received byte.
*@����					 �� ����һ���ؼ��Եĺ���,�൱������SPI��ʱ�ӣ�����ֱ����������
*/
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}


/*
*@Function Name  : SPI_FLASH_SendHalfWord
*@Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
*@Input          : Half Word : Half Word to send.
*@Return         : The value of the received Half Word.
*@����					 �� �������һ�η��͵��������ֽ�,����ֱ����������
*/
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}



/*
*@Function Name  : SPI_FLASH_WriteEnable
*@Description    : Enables the write access to the FLASH.
*/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Write Enable" instruction */
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}



/*
*@Function Name  : SPI_FLASH_WaitForWriteEnd
*@Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
*/
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}


/*
*@���ܣ���ȡ״̬�Ĵ�����ֵ
*@˵�������κ�״̬�¶��ܶ�ȡ״̬�Ĵ�����ֵ
*/
uint8_t SPI_FLASH_ReadStatusReg(void)
{
  uint8_t FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

	/* Send a dummy byte to generate the clock needed by the FLASH
	and put the value of the status register in FLASH_Status variable */
	FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();		//������״̬�Ĵ�������
	
	return FLASH_Status;
}




/*
*@���ܣ��������ģʽ
*/
void SPI_Flash_PowerDown(void)   
{ 
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Power Down" instruction */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}   


/*
*@����:����
*/
void SPI_Flash_WAKEUP(void)   
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Power Down" instruction */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   




/*
��װ�����W25Q16������
*/


uint8_t ReadData(uint8_t address)		//�ⲿflash��ͨ��SPI����ȡ����
{
	
	
	
	return 0;
}



void WritEDAta(uint32_t address,uint8_t data)		//ͨ��SPI����д���ݵ�flash��
{

	
	
}



void mykey_init(void)
{
  
  IWDG_Feed();
	
	
	/*AUTHOR_ROW*/
  SPI_FLASH_WriteWord(AUTHOR_ROW,0x00);
	/*AUTHOR_POINT*/	
  IWDG_Feed();
  SPI_FLASH_WriteWord(AUTHOR_POINT,AUTHOR_START);		

	
	
	
	
// 	/*CAIMA_ROW*/
//   SPI_FLASH_WriteWord(CAIMA_ROW,0x00);
// 	/*CAIMA_POINT*/	
//   IWDG_Feed();
//   SPI_FLASH_WriteWord(CAIMA_POINT,CAIMA_START);		//
	
	
	
	
	/*RECORD_ROW*/
  SPI_FLASH_WriteWord(RECORD_ROW,0x00);
	/*RECORD_POINT*/	
  IWDG_Feed();
  SPI_FLASH_WriteWord(RECORD_POINT,RECORD_START);		//
	
	
	
	
// 	/*XUNJIAN_ROW*/
//   SPI_FLASH_WriteWord(XUNJIAN_ROW,0x00);
// 	/*XUNJIAN_POINT*/	
//   IWDG_Feed();
//   SPI_FLASH_WriteWord(XUNJIAN_POINT,XUNJIAN_START);		//
	
	
	
	/*KEY_ENCODE*/
//   IWDG_Feed();
//   SPI_FLASH_WriteWord(KEY_ENCODE,0x00);		//KEY_ENCODEͷָ���ʼ����־λ
	
	
	
	
	
 
  
}
