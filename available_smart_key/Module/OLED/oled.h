/**********************************************************************
*												smart key
*										 Module：OLED模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-019 20:33     1.0             build this file 
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



#ifndef __OLED_H
#define __OLED_H		

#include "stm32f10x.h"
#include "sw_key.h"



// #define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	  


//使用4线串行接口时OLED端口定义					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_9)//CS  PB9
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_4)//RES  PB4
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//DC  PB12
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//CLK  PB13
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//DIN  PB15
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


typedef  void (*HsMenuFun)(void);

#define MAX_MENU_PERPAGE 4
#define MAX_MENUNAME    20

typedef struct tgMenuItem {
    uint8_t row;                  //显示开始位置
    uint8_t col;
    uint8_t Name[MAX_MENUNAME];   //菜单名称
    HsMenuFun pFunc;            //菜单功能
} MENU_ITEM;




extern uint32_t choosetime;
extern uint8_t KeyNum;
extern uint8_t OLED_buf_batt0[64];



#define MAX_MENU      10
extern MENU_ITEM HsMenu[MAX_MENU];

//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    

void OLED_Clear(void);
void OLED_ClearArea(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1 );

void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);

void OLED_ShowNum(uint8_t x,uint8_t y,u32 num,uint8_t len,uint8_t size);
void OLED_ShowNumHex(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);

void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	 
void OLED_Set_Pos(uint8_t x, uint8_t y);

void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t *pzh);
//void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t no);

void OLED_ShowUnite(uint8_t x,uint8_t y,uint8_t *phz);


void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[]);
void OLED_Init(void);






void hs_menu(void);
void tool_menu(void);
void set_menu(void);
void shut_down_key(void);
void password(void);
void power_off_oled(void);
void read_rfid(void);
void open_lock(void);
void close_lock(void);
void screensaver_time(void);
void version(void);


void ShowMenu(MENU_ITEM *Menu);
void ShowMenuNoInverse(MENU_ITEM *Menu); //主菜单显示

void OLED_ShowN(uint8_t row, uint8_t col, uint8_t *str);
void OLED_Show(uint8_t row, uint8_t col, uint8_t *str);


void Choose_MenuOp1(void);
void Choose_MenuOp4(void);
void Choose_MenuOp4_1(void);
void Choose_MenuOp4_2(void);
void Choose_MenuOp4_22(void);
void Choose_MenuOp4_3(void);

void Choose_MenuOp5(void);
void Choose_MenuOp5_1(void);
void Choose_MenuOp5_2(void);
void Choose_MenuOp5_3(void);

void Main_Oled_Power(void);
void Main_Oled_Time(void);



#endif  
	 



