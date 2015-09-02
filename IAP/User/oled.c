/**********************************************************************
*												smart key
*										 Module��OLEDģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-07-15 17:55     1.0             build this file 
*
* ģ�鹦�ܣ�
*				OLED�ײ������Ͳ˵�ҳ��

*	����˵����
* -----------------------------------------------------------
*       GND  ��Դ��
*       VCC  ��5V��3.3v��Դ
*       D0   ��PB13��SCL��
*       D1   ��PB15��SDA��
*       RES  ��PB4
*       DC   ��PB12
*       CS   ��PB9              
* ----------------------------------------------------------
*				���ֽӷ�������SPI2��ģ��SPI����ģ��ʹ��ģ��SPI
**********************************************************************/

#include "oled.h"
#include "oledfont.h"
#include "delay.h"
#include "spi_flash.h"



uint8_t mode=0;

const uint8_t asc_table[] = "0123456789ABCDEF";

	 
uint32_t GHZAddr=0x1C01BE;		//������ģ��flash�е���ʼ��ַ
uint8_t result=0;








// }





/*
*@���ܣ�0:4�ߴ���ģʽ����SSD1106д��һ���ֽڡ�
*@���룺dat:Ҫд�������/����
*				cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
*/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{	
	uint8_t i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)		//�Ӹ�λ��ʼ����
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();		//������������
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 


/*
*@���룺x��ʾ��,x=0~128��y��ʾҳ��ַ,y=0~7
*/
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);										//����ҳ��ַ��ҳ��ַ��Χ0~7
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);				//�еĸ�4λ����0x10,��ָ��Ҫ��
	OLED_WR_Byte((x&0x0f)+2,OLED_CMD); 									//�еĵ�4λ��
}   	  



/*
*@���ܣ�����OLED��ʾ    
*/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}



/*
*@���ܣ��ر�OLED��ʾ     
*/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF		����Ӧ����8A
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}



/*
*@���ܣ���������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
*/
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00|0x10,OLED_CMD);      //������ʾλ�á��е͵�ַ���ӵ�2�п�ʼ
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<134;n++)
			OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}



/*
*@���ܣ�������������ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
*/
void OLED_ClearArea(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1 )  
{  
		uint8_t x,y;
		
		//���е�ַת��Ϊҳ��ַ
		if(y1%8==0)
			y=y1/8;      
		else
			y=y1/8+1;
		
		for(y=y0;y<y1;y++)
		{
			OLED_Set_Pos(x0,y);
			for(x=x0;x<x1;x++)
			{      
				OLED_WR_Byte(0,OLED_DATA);	    	
			}
		}
}



/*
*@���ܣ���ָ��λ����ʾһ���ַ�,���������ַ�
*@���룺x:0~127��y:0~7��size:ѡ������ 16/12 
*/
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{      	
		uint8_t c=0,i=0;	
		uint8_t matrix[16];
	
	  c=chr-' ';//�õ�ƫ�ƺ��ֵ	
	
		if(mode==0)
			for(i=0;i<16;i++)
				matrix[i]=F8X16[c*16+i];
		else		
			for(i=0;i<16;i++)
				matrix[i]=~F8X16[c*16+i];

	
		if(x>Max_Column-1)
		{
			x=0;
			y=y+2;
		}

		
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)															//д��һ���8���ֽ�
			OLED_WR_Byte(matrix[i],OLED_DATA);
		
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)															//д��һ���8���ֽ�
			OLED_WR_Byte(matrix[i+8],OLED_DATA);

}

/*
*@���ܣ�m^n����
*/
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  


/*
*@���ܣ���ʾlen������
*@���룺 x,y :�������	 
*			  len :���ֵ�λ��
*       size:�����С
*       num:��ֵ(0~4294967295);
*/		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}
			else
				enshow=1; 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 



/*
*@���ܣ���ʾlen������
*@���룺 x,y :�������	 
*			  len :���ֵ�λ��
*       size:�����С
*       num:��ֵ(0~0xFF FF FF FF);
*/		  
void OLED_ShowNumHex(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp=0;
	uint8_t enshow=0;	
  uint32_t formwork=0,tempresult=0;	
	for(t=0;t<len;t++)
	{
		formwork=0xF<<((len-t-1)*4);
		tempresult=num&formwork;
		temp=tempresult>>((len-t-1)*4);
		
		//��λ���㲻��ʾ
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}
			else
				enshow=1; 
		}
		
		
	 	OLED_ShowChar(x+(size/2)*t,y,asc_table[temp]); 
	}
} 









/*
*@���ܣ���ʾһ���ַ��Ŵ�
*/
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr)
{
	uint8_t j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j]);
		x+=8;
		if(x>120)
		{
			x=0;
			y+=2;		//��ʾ��������
		}
		j++;
	}
}



/*
*@���ܣ���ʾһ������
*/
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t *pzh)
{      			    
		uint8_t t,ac,bc;	//ac��ʾarea code�����룬bc��ʾbit code��λ��
	  uint32_t off_addr,abs_addr;
		uint8_t matrix[32];		//�ݴ����ģ
	
		//uint8_t matrix[32]={0x10,0x60,0x02,0x0C,0xC0,0x02,0x1E,0xE2,0x02,0x02,0x02,0xE2,0x1E,0x00,0x00,0x00,
		//					0x04,0x04,0x7C,0x03,0x80,0x80,0x40,0x20,0x13,0x0C,0x13,0x20,0x40,0x80,0x80,0x00};
	
		ac=*pzh-160;
		bc=*(pzh+1)-160;
		off_addr=(94*(ac-1)+(bc-1))*32;
		abs_addr=GHZAddr+off_addr;
		
		SPI_FLASH_BufferRead(matrix,abs_addr,32);
	
		if(mode==1)
			for(t=0;t<32;t++)
				matrix[t]=~matrix[t];
		
		OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(matrix[t],OLED_DATA);
    }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
		{	
				OLED_WR_Byte(matrix[t+16],OLED_DATA);
		}					
}




/*
��ʾָ��λ�ú��ֺ��ַ�
*/
void OLED_ShowUnite(uint8_t x,uint8_t y,uint8_t *phz)
{
    uint8_t number=0;
    
    while((*phz != 0)&&(number < 20))                                         //��⵽������ʾ�˳�
    {
        if(*phz < 0x80)                                                       //ASCII�ַ�д����
        {
            OLED_ShowChar(x,y,*phz);                                   				//���˵��ո�
            phz++;
            x += 8;
        }
        else                                                                   //����д
        {
					OLED_ShowChinese(x,y,phz);
					phz += 2;
					x += 16;
        }
        number++;
    }
    
}



/*
*@���ܣ���ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
*/
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
		uint32_t j=0;
		uint8_t x,y;
		
		//���е�ַת��Ϊҳ��ַ
		if(y1%8==0)
			y=y1/8;      
		else
			y=y1/8+1;
		
		for(y=y0;y<y1;y++)
		{
			OLED_Set_Pos(x0,y);
			for(x=x0;x<x1;x++)
			{      
				OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
			}
		}
} 


/*
*@���ܣ���ʼ��SSD1106					    
*/
void OLED_Init(void)
{ 	
 
 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,B,C�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	   
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  

	
	
	
	
	
  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel  cmd11
	
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address cmd1
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address cmd2
	
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F) cmd4
	
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration  cmd17
	OLED_WR_Byte(0x12,OLED_CMD);
	
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  


















