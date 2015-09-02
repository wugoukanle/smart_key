/**********************************************************************
*												smart key
*										 Module��OLEDģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-019 20:33     1.0             build this file 
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
#include "delay.h"
#include "oledfont.h"
	 

uint32_t oled_pow(uint8_t m,uint8_t n);



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
*@���룺x��ʾ�У�y��ʾҳ��ַ,y=0~7
*/
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);										//����ҳ��ַ��ҳ��ַ��Χ0~7
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);				//�еĸ�4λ����0x10,��ָ��Ҫ��
	OLED_WR_Byte((x&0x0f),OLED_CMD); 						//�еĵ�4λ����0x01��
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
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ���ӵ�2�п�ʼ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<133;n++)
			OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}



/*
*@���ܣ���ָ��λ����ʾһ���ַ�,���������ַ�
*@���룺x:0~127��y:0~7��size:ѡ������ 16/12 
*/
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{      	
		uint8_t c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1)
		{
			x=0;
			y=y+2;
		}
		if(SIZE ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else 
		{	
			OLED_Set_Pos(x,y+1);
			for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
		}
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
*@���ܣ���ʾ����
*/
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
		uint8_t t,adder=0;
		OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
    }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
		{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
		}					
}

/*
*@���ܣ���ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
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
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 //PD3,PD6�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	//PD3,PD6 �����

  //����
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //PC0,1 OUT�������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);						 //PC0,1 OUT  �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PG15 OUT�������	  RST
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG15 OUT  �����




 
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





























