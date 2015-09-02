/**********************************************************************
*												smart key
*										 Module：OLED模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-07-15 17:55     1.0             build this file 
*
* 模块功能：
*				OLED底层驱动和菜单页面

*	连接说明：
* -----------------------------------------------------------
*       GND  电源地
*       VCC  接5V或3.3v电源
*       D0   接PB13（SCL）
*       D1   接PB15（SDA）
*       RES  接PB4
*       DC   接PB12
*       CS   接PB9              
* ----------------------------------------------------------
*				这种接法可以用SPI2或模拟SPI，此模块使用模拟SPI
**********************************************************************/

#include "oled.h"
#include "oledfont.h"
#include "delay.h"
#include "spi_flash.h"



uint8_t mode=0;

const uint8_t asc_table[] = "0123456789ABCDEF";

	 
uint32_t GHZAddr=0x1C01BE;		//汉字字模在flash中的起始地址
uint8_t result=0;








// }





/*
*@功能：0:4线串行模式，向SSD1106写入一个字节。
*@输入：dat:要写入的数据/命令
*				cmd:数据/命令标志 0,表示命令;1,表示数据;
*/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{	
	uint8_t i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)		//从高位开始传送
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();		//上升沿锁数据
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 


/*
*@输入：x表示列,x=0~128，y表示页地址,y=0~7
*/
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);										//设置页地址，页地址范围0~7
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);				//列的高4位，或0x10,是指令要求
	OLED_WR_Byte((x&0x0f)+2,OLED_CMD); 									//列的低4位，
}   	  



/*
*@功能：开启OLED显示    
*/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}



/*
*@功能：关闭OLED显示     
*/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF		？，应该是8A
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}



/*
*@功能：清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
*/
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00|0x10,OLED_CMD);      //设置显示位置—列低地址，从第2列开始
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<134;n++)
			OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}



/*
*@功能：清屏的区域，起始点坐标(x,y),x的范围0～127，y为页的范围0～7
*/
void OLED_ClearArea(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1 )  
{  
		uint8_t x,y;
		
		//把行地址转化为页地址
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
*@功能：在指定位置显示一个字符,包括部分字符
*@输入：x:0~127，y:0~7，size:选择字体 16/12 
*/
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{      	
		uint8_t c=0,i=0;	
		uint8_t matrix[16];
	
	  c=chr-' ';//得到偏移后的值	
	
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
		for(i=0;i<8;i++)															//写上一半的8个字节
			OLED_WR_Byte(matrix[i],OLED_DATA);
		
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)															//写下一半的8个字节
			OLED_WR_Byte(matrix[i+8],OLED_DATA);

}

/*
*@功能：m^n函数
*/
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  


/*
*@功能：显示len个数字
*@输入： x,y :起点坐标	 
*			  len :数字的位数
*       size:字体大小
*       num:数值(0~4294967295);
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
*@功能：显示len个数字
*@输入： x,y :起点坐标	 
*			  len :数字的位数
*       size:字体大小
*       num:数值(0~0xFF FF FF FF);
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
		
		//高位的零不显示
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
*@功能：显示一个字符号串
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
			y+=2;		//表示换到下行
		}
		j++;
	}
}



/*
*@功能：显示一个汉字
*/
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t *pzh)
{      			    
		uint8_t t,ac,bc;	//ac表示area code即区码，bc表示bit code即位码
	  uint32_t off_addr,abs_addr;
		uint8_t matrix[32];		//暂存的字模
	
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
显示指定位置汉字和字符
*/
void OLED_ShowUnite(uint8_t x,uint8_t y,uint8_t *phz)
{
    uint8_t number=0;
    
    while((*phz != 0)&&(number < 20))                                         //检测到结束标示退出
    {
        if(*phz < 0x80)                                                       //ASCII字符写操作
        {
            OLED_ShowChar(x,y,*phz);                                   				//过滤掉空格
            phz++;
            x += 8;
        }
        else                                                                   //汉字写
        {
					OLED_ShowChinese(x,y,phz);
					phz += 2;
					x += 16;
        }
        number++;
    }
    
}



/*
*@功能：显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
*/
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
		uint32_t j=0;
		uint8_t x,y;
		
		//把行地址转化为页地址
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
*@功能：初始化SSD1106					    
*/
void OLED_Init(void)
{ 	
 
 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,B,C端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	   
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
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
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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


















