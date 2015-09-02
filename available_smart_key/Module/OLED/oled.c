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

#include "oled.h"
#include "delay.h"
#include "oledfont.h"
#include "spi_flash.h"
#include "Globle_Var.h"
#include "timer.h"
#include "Li_Power.h"
#include "string.h"
#include "RTC_time.h"


const uint8_t asc_table[] = "0123456789ABCDEF";

	 
uint32_t GHZAddr=0x1C01BE;		//汉字字模在flash中的起始地址
uint32_t choosetime=200;
uint8_t code[4]={'0','0','0','0'};
uint8_t rightcode[4]={1,2,3,4};//正确密码
uint8_t result=0;
uint8_t KeyNum;
uint8_t zuming[8];
uint8_t zuming1[2];
uint8_t zuming2[2];
uint8_t zuming3[2];
uint8_t zuming4[2];




uint8_t OLED_buf_batt0[64]={
0x00,0xFE,0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0x02,0x02,0xFA,0xFA,0xFA,0xFA,0xFA,
0xFA,0xFA,0x02,0x02,0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0x3E,0x20,0x20,0x20,0xE0,
0x00,0x7F,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x40,0x40,0x5F,0x5F,0x5F,0x5F,0x5F,
0x5F,0x5F,0x40,0x40,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x7C,0x04,0x04,0x04,0x07};
/*电池图1.BMP",0*//* (32 X 16 )*/	

	
	

uint8_t OLED_buf_batt1[]={
0x00,0xFE,0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0x02,0x02,0xFA,0xFA,0xFA,0xFA,0xFA,
0xFA,0xFA,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x3E,0x20,0x20,0x20,0xE0,
0x00,0x7F,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x40,0x40,0x5F,0x5F,0x5F,0x5F,0x5F,
0x5F,0x5F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C,0x04,0x04,0x04,0x07};
/*电池图2.BMP",0*//* (32 X 16 )*/


uint8_t OLED_buf_batt2[]={
0x00,0xFE,0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0xFA,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x3E,0x20,0x20,0x20,0xE0,
0x00,0x7F,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C,0x04,0x04,0x04,0x07};
/*电池图3.BMP",0*//* (32 X 16 )*/

uint8_t OLED_buf_batt3[]={
0x00,0xFE,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x3E,0x20,0x20,0x20,0xE0,
0x00,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C,0x04,0x04,0x04,0x07};
/*电池图4.BMP",0*//* (32 X 16 )*/








/*界面*/

#define MAX_MENU      10



// HsMenu[1].Name[MAX_MENUNAME]={0};
// HsMenu[2].Name[MAX_MENUNAME]={0};

MENU_ITEM HsMenu[MAX_MENU]={   //主菜单
    {4,4,"",0},	//其结构的第0个为特别的，表示可选菜单项的起始项，和最后项，并始终将可选项放在最后
    {32,2,"",0},
		{32,4,"",0},
		{0,6,"菜单",tool_menu},
		{96,6,"设置",set_menu},
};



MENU_ITEM TOOLMENU[MAX_MENU]={
		{3,4,"",0},
    {0,6,"确定",0},
		{96,6,"取消",0},
    {32,2,"密码开锁",password},
    {48,4,"关机",shut_down_key},
};


MENU_ITEM SETMENU[MAX_MENU]={
		{3,4,"",0},
		{0,6,"确定",hs_menu},
		{96,6,"取消",hs_menu},
    {32,2,"屏保设置",screensaver_time},
		{48,4,"关于",version},//pfun
};


MENU_ITEM SETSUCCESS[MAX_MENU]={
		{2,2,"",0},
		{32,4,"设置成功",0},
		{0,6,"返回",0},
};


MENU_ITEM MIMA[MAX_MENU]={
		{4,7,"",0},  
		{24,2,"请输入密码",0},
    {0,6,"确定",0},
		{96,6,"取消",0},
    {32,4,"0",0},		//f_CurSel=4
    {48,4,"0",0},		//f_CurSel=5
    {64,4,"0",0},		//f_CurSel=6
    {80,4,"0",0},		//f_CurSel=7
};



MENU_ITEM SHUTDOWNKEY[MAX_MENU]={
		{2,3,"",0},
		{32,4,"是否关机",0},
    {0,6,"是",power_off_oled},
		{112,6,"否",0},
};



MENU_ITEM OPENLOCK[MAX_MENU]={
		{2,2,"",0},
    {32,2,"开锁中",0},
    {0,6,"返回",close_lock},
    
};

MENU_ITEM READRFID[MAX_MENU]={
		{3,4,"",0},
		{48,2,"读RFID",0},
		{32,4," ",0},
    {0,6,"开始",0},
		{96,6,"返回",tool_menu},
};



MENU_ITEM TIME[MAX_MENU]={
		{3,5,"",0},
		{0,6,"确定",hs_menu},
		{96,6,"取消",hs_menu},
    {16,2,"10秒",0},
		{48,2,"30秒",0},
		{80,2,"60秒",0},
};

MENU_ITEM VERSION[MAX_MENU]={
		{4,4,"",0},
		{0,2,"版本：V1.0 2015",0},
		{0,4,"武汉百景互动科技",0},
		{0,6,"返回",0}
}; 








/*菜单项的处理函数*/

//主界面菜单
void hs_menu(void)
{
	Choose_MenuOp1();
	return;
}
//显示工具菜单
void tool_menu(void)
{
	Choose_MenuOp4();
	return;
}
	
	
//显示设置菜单
void set_menu(void)
{
	Choose_MenuOp5();
	return;
}


void shut_down_key(void)
{
	Choose_MenuOp4_3();;
	return;
}



void password(void)
{
	Choose_MenuOp4_22();;
	return;
}

void power_off_oled(void)
{
	OLED_Display_Off();
	guanjiflag=1;
	return;
}

void read_rfid(void)
{
	Choose_MenuOp4_1();
	return;
}	

void open_lock(void)
{
	Choose_MenuOp4_2();
	return;
}

void close_lock(void)
{
	OLED_Display_Off();
	Choose_MenuOp4();
	return;
}


void screensaver_time(void)
{
	Choose_MenuOp5_1();
	return;
}

void version(void)
{
	Choose_MenuOp5_3();
	return;
}





// void ShowMenu(const MENU_ITEM *Menu) //主菜单显示
// {
//     uint8_t i,j,idx;
// 	
// 		const uint8_t window_len=2;
// 		uint8_t window[window_len];
// 	
// 	
// 		uint8_t optional_start,optional_end,optional_num,optional_show_len;
// 		optional_start=Menu[0].row;
// 		optional_end=Menu[0].col;
// 		optional_num=optional_end-optional_start+1;
// 	
// 	
// 	
// 		//非可选菜单项显示
//     for (idx=1;idx<optional_start;idx++)
// 			  OLED_Show(Menu[idx].row,Menu[idx].col,(uint8_t*)Menu[idx].Name);		//未选中的行正常显示

// 	
// 		//确定窗口菜单项
// 		if(optional_num<=window_len)			                //如果可选菜单数小于等于窗口长度
// 		{
// 				for(i=0;i<optional_num;i++)
// 					window[i]=optional_start+i;
// 				optional_show_len=optional_num;
// 		}
// 		else																							//如果可选菜单数大于窗口长度
// 		{
// 				if(f_CurSel>optional_end-window_len+1)				//特殊情况，f_CurSel在可选菜单尾时
// 				{
// 						//可选菜单区的尾后一部分
// 					  for(i=0;i<optional_end-f_CurSel+1;i++)
// 					    window[i]=f_CurSel+i;
// 		
// 					  //可选菜单区的前面一部分
// 						for(j=0;j<optional_num-(optional_end-f_CurSel+1);j++)
// 						{
// 							window[i]=optional_start+j;
// 							i++;
// 						}		
// 				}
// 				else    																			//一般情况，f_CurSel其他位置
// 				{
// 						for(i=0;i<window_len;i++)
// 							window[i]=f_CurSel+i;
// 				}
// 				optional_show_len=window_len;				
// 				
// 		}


//     for (idx=0;idx<optional_show_len;idx++)
//     {
//        if (window[idx]==f_CurSel)
//            OLED_ShowN(Menu[window[idx]].row, 2+idx*2, (uint8_t*)Menu[window[idx]].Name);		//会将选中的行反白显示
//        else
//            OLED_Show(Menu[window[idx]].row, 2+idx*2, (uint8_t*)Menu[window[idx]].Name);		//未选中的行正常显示
//     }
// }

void ShowMenu(MENU_ITEM *Menu) //主菜单显示
{
    uint8_t idx;
	
    for (idx=1;idx<=Menu[0].col;idx++)
    {
       if (idx==f_CurSel)
           OLED_ShowN(Menu[idx].row,Menu[idx].col,(uint8_t*)Menu[idx].Name);		//会将选中的行反白显示
       else
           OLED_Show(Menu[idx].row,Menu[idx].col,(uint8_t*)Menu[idx].Name);		//未选中的行正常显示
    }
		//循环完之后所有菜单项全部显示
}

void ShowMenuNoInverse(MENU_ITEM *Menu) //主菜单显示
{
    uint8_t idx;

    for (idx=1;idx<=Menu[0].col;idx++)
    {
				 OLED_Show(Menu[idx].row,Menu[idx].col,(uint8_t*)Menu[idx].Name);		//未选中的行正常显示
    }
		//循环完之后所有菜单项全部显示
}


void  OLED_ShowN(uint8_t row, uint8_t col, uint8_t *str)
{
   mode=1;  //反白显示
   OLED_ShowUnite(row,col,str);
	 mode =0;
}



void OLED_Show(uint8_t row, uint8_t col, uint8_t *str)
{
   OLED_ShowUnite(row,col,(uint8_t*)str);
}





/*主菜单界面操作函数*/
void Choose_MenuOp1(void)
{
  uint8_t flag=0,i=0,j=0,nChar,temp_f_CurSel,flagshijian=0,keycode;


	OLED_Clear();	    //清屏
  f_CurSel =HsMenu[0].row;	
	ShowMenuNoInverse(HsMenu);
	Main_Oled_Power();
	Main_Oled_Time();	
	
	
	
  g_WaitTimeOut = 0;
  SetTimer(TIMER_TIMEOUT,1,Do_TimeOut,THREE_MIN);
  while(g_WaitTimeOut == 0)
  {
		Init_KeyNumuber();			
		nChar = GetKey();
		if(guanjiflag==1)
		{
			break;
    }
		else
		{

				if(nChar ==KEYINIT)  //超时关屏
				{
					i++;
					delay_ms(100);
					if(i>=choosetime)//按下超过10秒钟
					{
						OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
						flag=1;
						i=0;
					}
				}
				
				if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
				{
					OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	
					flag=0;
					delay_ms(1000);
					nChar =KEYINIT;
				}
				
				if(flag==0&&nChar == KEYUP)	//上翻	++
				{
						 i=0;
						//ResetTimer(TIMER_TIMEOUT);		//还没实现
						if (f_CurSel>HsMenu[0].row)		//HsMenu[0].col是最大
								--f_CurSel;
						else if(f_CurSel==HsMenu[0].row) //HsMenu[0].row是最小
								f_CurSel = HsMenu[0].col;		//HsMenu[0].row是最小
						OLED_Clear();
						ShowMenuNoInverse(HsMenu);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
						Main_Oled_Power();
						Main_Oled_Time();	
						
						delay_ms(500);
						Init_KeyNumuber();

				}
				if(flag==0&&nChar ==KEYDOWN)//下翻	--
				{
							i=0;
							//ResetTimer(TIMER_TIMEOUT);
							if (f_CurSel<HsMenu[0].col)
									++f_CurSel;
							else
									f_CurSel =  HsMenu[0].row;
							OLED_Clear();
							ShowMenuNoInverse(HsMenu);
							Main_Oled_Power();
							Main_Oled_Time();	
							
							Init_KeyNumuber();
							delay_ms(500);

				}
				if(flag==0&&nChar ==KEYSURE) //确定
				{
							 i=0;
							//KillTimer(TIMER_TIMEOUT);
							//g_WaitTimeOut = 0;
							OLED_Clear();
							temp_f_CurSel=f_CurSel;
							Choose_MenuOp4();		//f_CurSel表示当前的行
							f_CurSel=temp_f_CurSel;				
							
							ShowMenuNoInverse(HsMenu);
							Main_Oled_Power();
							Main_Oled_Time();	

							delay_ms(500);
							Init_KeyNumuber();
					 //  break;

				} 
				if( flag==0&&nChar ==KEYCLEAR)//取消
				{
					i=0;
					j++;
					//KeyNum =KEYNO;
					Choose_MenuOp5();
					delay_ms(100);
					ShowMenuNoInverse(HsMenu);
					Main_Oled_Power();
					Main_Oled_Time();					
				}
				
				
				if(j>=30)//按下超过3秒钟
				{
					j=0;
					temp_f_CurSel=f_CurSel;
					//Choose_MenuOp4_3();
					//if(f_CurSel==3)	
					//break;
					f_CurSel=temp_f_CurSel;				
					ShowMenu(HsMenu);	
				}
// 				else if (nChar !=KEYCLEAR&&j>0&&j<20)
// 				{
// 					OLED_Clear();
// 					g_State = ST_IDLE;
// 					ResetKey();
// 					break;
// 				}   
			}
			++flagshijian;
  }
  g_State =ST_IDLE;
  return;
}
 
 






/*
*@功能：密码开锁和关机界面
*/
void Choose_MenuOp4(void)
{
  uint8_t flag=0,i=0,j=0,nChar,temp_f_CurSel;
 
  f_CurSel =TOOLMENU[0].row;


	OLED_Clear();	 
	ShowMenu(TOOLMENU);
	Main_Oled_Power();
	Main_Oled_Time();		
	
  while(1)
  {
		Init_KeyNumuber();
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else
		{
			if(nChar ==KEYINIT)  //超时关屏
			{
				i++;
				//delay_ms(100);
				delay_ms(100);
				if(i>=choosetime)//按下超过10秒钟
				{
					OLED_Display_Off();
					flag=1;
					i=0;
				}
			}
			
			if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))	//按任意键开屏
			{
				OLED_Display_On();
				flag=0;
				delay_ms(1000);
				nChar =KEYINIT;
			}
			
			
			if(nChar == KEYUP)	//上翻	++
			{
					i=0;
					j=0;
				 if (f_CurSel>TOOLMENU[0].row)					//TOOLMENU[0].row=3最小
							--f_CurSel;
					else if(f_CurSel<=TOOLMENU[0].row) 		//TOOLMENU[0].row=3最小
							f_CurSel= TOOLMENU[0].col;				//TOOLMENU[0].col=4最大
					OLED_Clear();
					ShowMenu(TOOLMENU);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
					Main_Oled_Power();
					Main_Oled_Time();	
					Init_KeyNumuber();
					delay_ms(100);
			}
			if(nChar ==KEYDOWN)//下翻	--
			{
						i=0;
						j=0;

						if (f_CurSel<TOOLMENU[0].col)			//TOOLMENU[0].col=4是最大
								++f_CurSel;
						else if(f_CurSel>=TOOLMENU[0].col)
								f_CurSel=TOOLMENU[0].row;		//TOOLMENU[0].row=3是最小
						
						OLED_Clear();
						ShowMenu(TOOLMENU);
						Main_Oled_Power();
						Main_Oled_Time();							
						Init_KeyNumuber();
						delay_ms(100);					
			}
			if(nChar ==KEYSURE) //确定
			{
					  i=0;
					  j=0;

						OLED_Clear();
						temp_f_CurSel=f_CurSel;
						TOOLMENU[f_CurSel].pFunc();
						f_CurSel=temp_f_CurSel;
						//ShowMenu(OPENLOCK);//f_CurSel表示当前的行
						Init_KeyNumuber();
						ShowMenu(TOOLMENU);	
						Main_Oled_Power();
						Main_Oled_Time();					
						delay_ms(100);
			} 
			if( flag==0&&nChar ==KEYCLEAR)//取消
			{
						i=0;
						j++;
						//KeyNum =KEYNO;
						delay_ms(100);
			}
			
			
			if(j>=30)//按下超过3秒钟
			{
				j=0;
				temp_f_CurSel=f_CurSel;
				Choose_MenuOp4_3();
				//if(f_CurSel==2)	
				//break;
				f_CurSel=temp_f_CurSel;				
				ShowMenu(TOOLMENU);	
				Main_Oled_Power();
				Main_Oled_Time();					
			}
			else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
			{
						i=0;
						j=0;
						OLED_Clear();
						g_State = ST_IDLE;
						ResetKey();
						break;
			}
	  }
  }
	
  g_State =ST_IDLE;
  return;
}



/*
*@功能：读RFID界面
*/
void Choose_MenuOp4_1(void)
{

	uint8_t flag=0,j=0,nChar,temp_f_CurSel;

  f_CurSel =READRFID[0].row;
 
	ShowMenu(READRFID);
  g_WaitTimeOut = 0;
  SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
// 	while(g_WaitTimeOut == 0)
// 	{
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			if(nChar ==5)
// 			{
// 					OLED_ShowUnite(32,80,Rfid_Buff);
// 					++rfidrow;
// 					delay_ms(1000);		
// 					for(i=0;i<4;i++)
// 					{
// 							for(k =0;k<40;k++)
// 								msDelay(500);
// 							WritEDAta(CAIMA_START+8*rfidrow+4+i,Rfid_Buff[i]);
// 					}				
// 					break;
// 			}

// 	}	
  KillTimer(TIMER_TIMEOUT);
					
	
  while(1)
  {
		Init_KeyNumuber();
		nChar = GetKey();   //读取按键缓冲区
		if(nChar ==KEYINIT)
		{
			delay_ms(100);
			
		}
		 if(nChar == KEYUP)	//上翻	++
		{
				
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel>READRFID[0].row)
						--f_CurSel;
				else if(f_CurSel==READRFID[0].row) 
						f_CurSel = READRFID[0].col;	
				OLED_Clear();
				ShowMenu(READRFID);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				OLED_ShowUnite(32,80,Rfid_Buff);
				//InsertKey(KEYNO); 
				Init_KeyNumuber();

		} if(nChar ==KEYDOWN)//下翻	--
		{
					
					//ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel<READRFID[0].col)
							++f_CurSel;
					else
							f_CurSel =  READRFID[0].row;
					OLED_Clear();
					ShowMenu(READRFID);
					Init_KeyNumuber(); 

		}
		if(nChar ==KEYSURE) //确定
		{
					 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
			    if(f_CurSel==3)
					{
						ShowMenu(READRFID);
						g_WaitTimeOut = 0;
            SetTimer(TIMER_TIMEOUT,10,Do_TimeOut,THREE_MIN);
// 						while(g_WaitTimeOut == 0)
// 						{
// 								nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 								if(nChar ==5) 
// 								{
// 										OLED_ShowUnite(32,80,Rfid_Buff);
// 										++rfidrow;
// 										delay_ms(1000);						
// 										for(i=0;i<4;i++)
// 										{
// 												for(k =0;k<40;k++)
// 													msDelay(500);
// 												WritEDAta(CAIMA_START+8*rfidrow+4+i,Rfid_Buff[i]);
// 										}										
// 										break;
// 								}
// 						}	

          KillTimer(TIMER_TIMEOUT);
          }
					else
				  {
						OLED_Clear();
						g_State = ST_IDLE;
						ResetKey();
						break;
					}
					Init_KeyNumuber();

		}
		 if( nChar ==KEYCLEAR)//取消
		{
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
  g_State =ST_IDLE;
  return;
}




/*
*@功能：开锁界面
*/
void Choose_MenuOp4_2(void)
{
  uint8_t nChar, temp_f_CurSel;
  //OLED_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =OPENLOCK[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(OPENLOCK);		//显示菜单
	//void InsertKey(uint8_t nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	
	
// 	while(1)
// 	{
// 			OLED_ShowUnite(16,56,"reading RFID");
// 			delay_ms(1000);	
// 			nChar =Read_RfidData(2,Rfid_Buff,0xff);
// 			//m_UB1;
// 			if(nChar ==5) 
// 			{
// 				if(Rfid_Buff[0]==0&&Rfid_Buff[1]==0&&Rfid_Buff[2]==0&&Rfid_Buff[3]==0)
// 					;
// 				else		//至少有一个不为零
// 				{
// 					//UARTnSendChar(1,Rfid_Buff,5);
// 					//OLED_ShowUnite(56,80,Rfid_Buff);
// 					OLED_ShowUnite(36,80, Rfid_Buff);
// 					delay_ms(1000);						
// 					break;
// 				}
// 			}
// 	}		
// 	
	
	
	//result=check_suoma();
	if(result==1)
	{
		COIL_ACTUATE;
		OLED_ShowUnite(32,4,"已授权");			
	}
	else
		OLED_ShowUnite(32,4,"未授权");	
	
  while(1)
  {
		
		//OLED_ShowUnite(32,0,lcd_buf2);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(nChar ==KEYINIT)
		{
			
				
			//OLED_ShowUnite(96,0,lcd_buf_batt0	
			/*if(Power_Cheak() ==0)
					{
				  OLED_ShowUnite(96,0,lcd_buf_batt0);	
					}
			else if(Power_Cheak() ==1)
					{
				  OLED_ShowUnite(96,0,lcd_buf_batt3);	
					}
			     else if(Power_Cheak() ==2)
					{
				  OLED_ShowUnite(96,0,lcd_buf_batt1);	
					}
			          else if(Power_Cheak() ==3)
					{
				  OLED_ShowUnite(96,0,lcd_buf_batt2);	
					}
			if(USB_Cheak_init()==1)  //USB_CHEAK ==(GPIO_Pin_12)
			{
				OLED_ShowUnite(96,0,lcd_buf_signal);
      }
			else
			{
				OLED_ShowUnite(96,0,lcd_buf_nosignal)
      }*/
			//InsertKey(KEYNO);		//Key_Num =nChar;
			delay_ms(100);
		}
		else if(nChar == KEYUP)	//上翻	++
		{
				 
				//ResetTimer(TIMER_TIMEOUT);		//还没实现
				if (f_CurSel<OPENLOCK[0].col)		//HsMenu[0].col是最大
						++f_CurSel;
				else if(f_CurSel==OPENLOCK[0].col) //HsMenu[0].row是最小
						f_CurSel = OPENLOCK[0].row;		//HsMenu[0].row是最小
				OLED_Clear();
				ShowMenu(OPENLOCK);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
				//OLED_ShowUnite(32,0,lcd_buf2);	
				//OLED_ShowUnite(96,0,lcd_buf_batt0);	
				/*if(Power_Cheak() ==0)
					{
				  OLED_ShowUnite(96,0,lcd_buf_batt0);	
						}
					if(Power_Cheak() ==1)
					{
				  OLED_ShowUnite(96,0,lcd_buf_batt3);	
						}
					if(Power_Cheak() ==2)
					{
				  OLED_ShowUnite(96,0,lcd_buf_batt1);	
						}
						if(Power_Cheak() ==3)
					{
				  OLED_ShowUnite(96,0,lcd_buf_batt2);	
						}
						if(USB_Cheak_init()==1)
			     {
				     OLED_ShowUnite(96,0,lcd_buf_signal);
           }
			      else
			     {
				     OLED_ShowUnite(96,0,lcd_buf_nosignal);
           }*/
				Init_KeyNumuber();

		}
// 		else if(nChar ==KEYDOWN)//下翻	--
// 		{
// 					
// 					//ResetTimer(TIMER_TIMEOUT);
// 					if (f_CurSel<HsMenu[0].col)
// 							++f_CurSel;
// 					else
// 							f_CurSel =  HsMenu[0].row;
// 					OLED_Clear();
// 					ShowMenu(HsMenu);
// 					OLED_ShowUnite(32,0,lcd_buf2);	
// 					/*if(Power_Cheak() ==0)
// 					{
// 				  OLED_ShowUnite(96,0,lcd_buf_batt0);	
// 						}
// 					if(Power_Cheak() ==1)
// 					{
// 				  OLED_ShowUnite(96,0,lcd_buf_batt3);	
// 						}
// 					if(Power_Cheak() ==2)
// 					{
// 				  OLED_ShowUnite(96,0,lcd_buf_batt1);	
// 						}
// 						if(Power_Cheak() ==3)
// 					{
// 				  OLED_ShowUnite(96,0,lcd_buf_batt2);	
// 						}
// 			if(USB_Cheak_init()==1)
// 			{
// 				OLED_ShowUnite(96,0,lcd_buf_signal);
//       }
// 			else
// 			{
// 				OLED_ShowUnite(96,0,lcd_buf_nosignal);
//       }*/
// 					Init_KeyNumuber();
// 					delay_ms(1);
//           GPIOC->IDR= GPIO_Pin_8;

// 		}
		else if(nChar ==KEYSURE) //确定
		{
					 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
			    temp_f_CurSel=f_CurSel;
					OPENLOCK[f_CurSel].pFunc();		//f_CurSel表示当前的行
			    f_CurSel=temp_f_CurSel;
					Init_KeyNumuber();

		}else if( nChar ==KEYCLEAR)//取消
		{
					//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}


/*
*@功能：密码界面
*/
void Choose_MenuOp4_22(void)
{
  uint8_t flag=0,i=0,j=0,nChar, temp_f_CurSel=0;

  f_CurSel =MIMA[0].row;

	OLED_Clear();	    //清屏
	ShowMenu(MIMA);		//显示菜单

	
  while(1)
  {
		
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else
		{
				if(nChar ==KEYINIT)  //超时关屏
				{
					i++;
					delay_ms(100);
					if(i>=choosetime)//按下超过10秒钟
					{
						OLED_Display_Off();
						flag=1;
						i=0;
					}
				}
				if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
				{
					OLED_Display_On();
					flag=0;
					delay_ms(100);
					nChar =KEYINIT;
				}
				if(nChar == KEYUP)	//上翻	++
				{
						i=0;
						j=0;

						++MIMA[f_CurSel].Name[0];
						if(MIMA[f_CurSel].Name[0]>57)
							MIMA[f_CurSel].Name[0]='0';
						code[f_CurSel-4]=MIMA[f_CurSel].Name[0];
						Init_KeyNumuber();
						ShowMenu(MIMA);	
						delay_ms(100);
				}
				if(nChar ==KEYDOWN)//下翻	--
				{
						i=0;
						j=0;
						--MIMA[f_CurSel].Name[0];
						if(MIMA[f_CurSel].Name[0]<48)
							MIMA[f_CurSel].Name[0]='9';
						code[f_CurSel-4]=MIMA[f_CurSel].Name[0];
						Init_KeyNumuber();
						ShowMenu(MIMA);	
						delay_ms(100);
				}
				if(nChar ==KEYSURE) //确定
				{
						i=0;
					  j=0;

						if (f_CurSel<MIMA[0].col)      //HsMenu[0].col是最大
						{					  
								++f_CurSel;
								Init_KeyNumuber();
								ShowMenu(MIMA);	
								delay_ms(100);
						}
						else if(f_CurSel>=MIMA[0].col) //HsMenu[0].row是最小
						{   
								SPI_FLASH_Read(rightcode,UNLOCK_CODE,4);
							
								//for debug
// 					 			SPI_FLASH_Read(item,UNLOCK_CODE,4);
// 					 			UART1nSendChar(1,item,4);
// 					 			delay_ms(1000);								
// 							
// 					 			UART1nSendChar(1,rightcode,4);
// 					 			delay_ms(1000);							
								
								if(code[0]==(rightcode[0]+48)&&code[1]==(rightcode[1]+48)&&code[2]==(rightcode[2]+48)&&code[3]==(rightcode[3]+48))
								{
										OLED_Clear();
										OLED_ShowUnite(32,2,"密码正确！");
										OLED_ShowUnite(40,4,"开锁中");
										code[0]='0';
										code[1]='0';									
										code[2]='0';									
										code[3]='0';									
										MIMA[4].Name[0]='0';
										MIMA[5].Name[0]='0';
										MIMA[6].Name[0]='0';
										MIMA[7].Name[0]='0';
										delay_ms(100);
										COIL_ACTUATE;		
										delay_ms(5000);		
										COIL_NO_ACTUATE;
										OLED_Clear();
										break;														
								}
								else 
								{
										OLED_Clear();
										OLED_ShowUnite(24,2,"密码错误！");
										OLED_ShowUnite(24,4,"请重新输入");
										code[0]='0';
										code[1]='0';									
										code[2]='0';									
										code[3]='0';									
										MIMA[4].Name[0]='0';
										MIMA[5].Name[0]='0';
										MIMA[6].Name[0]='0';
										MIMA[7].Name[0]='0';
										delay_ms(2000);
										OLED_Clear();
										f_CurSel = MIMA[0].row;		//HsMenu[0].row是最小
										ShowMenu(MIMA);
										Init_KeyNumuber();
								}
						}
				}
				if( flag==0&&nChar ==KEYCLEAR)//取消
				{
							i=0;
							j++;
							//KeyNum =KEYNO;
							delay_ms(100);
				}
				
				
				
				
				if(j>=30)//按下超过3秒钟
				{
					j=0;
					temp_f_CurSel=f_CurSel;
					Choose_MenuOp4_3();
					//if(f_CurSel==2)	
					//break;
					f_CurSel=temp_f_CurSel;				
					ShowMenu(MIMA);	
				}
				else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
				{
					i=0;
					j=0;//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;
				}
     }//else
  }//while循环

  g_State =ST_IDLE;
  return;
}



/*
*@功能：关机界面
*/
void Choose_MenuOp4_3(void)
{
  uint8_t flag=0,i=0,j=0,nChar,temp_f_CurSel;

  f_CurSel =SHUTDOWNKEY[0].row;

	delay_ms(100);
	OLED_Clear();	 
	ShowMenuNoInverse(SHUTDOWNKEY);
	Main_Oled_Power();
	Main_Oled_Time();		
	
  while(1)
  {
		Init_KeyNumuber();
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else
		{
				if(nChar ==KEYINIT)  //超时关屏
				{
					i++;
					//delay_ms(100);
					delay_ms(100);
					if(i>=choosetime)//按下超过10秒钟
					{
						OLED_Display_Off();
						flag=1;
						i=0;
					}
				}
				if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
				{
					OLED_Display_On();
					flag=0;
					delay_ms(1000);
					nChar =KEYINIT;
				}
				
				
				
				//此时只有确定和取消两个键有效
				if(nChar ==KEYSURE) //确定
				{
					i=0;
					j=0; 
					OLED_Clear();
					if(f_CurSel==3)
						break;
					else
							SHUTDOWNKEY[f_CurSel].pFunc();
					
					if(guanjiflag==1)
					break;
							//ShowMenu(OPENLOCK);
									//f_CurSel表示当前的行
							Init_KeyNumuber();
					//if(guanjiflag==0)
							//ShowMenu(SHUTDOWNKEY);	
					//else 
							//ShowMenu(KAIJI);

				}
				else if( nChar ==KEYCLEAR)//取消
				{
							OLED_Clear();
							g_State = ST_IDLE;
							ResetKey();
							break;
				}
			}
  }
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}


/*显示界面的入口*/
/*void Choose_MenuOp4_4(void)
{
  uint8_t flag=0,j=0,epi,epr,hangj,jj,k=0,nChar,temp_f_CurSel=0,tempdata[2],tempsuoma[4],recordsuoma[4];
	uint8_t row,recordrow;
	int i=0;
  //OLED_Clear(White);
  //ResetKey();			//Key_Num =KEYNO
  f_CurSel =JIANCHA[0].row;
//   g_WaitTimeOut = 0;
//   SetTimer(TIMER_TIMEOUT,3,Do_TimeOut,THREE_MIN);
//   while( g_WaitTimeOut == 0 )  //超时等待

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(JIANCHA);		//显示菜单
	//void InsertKey(uint8_t nChar){ Key_Num =nChar;}	
  //InsertKey(KEYINIT);   //在按键缓冲区，插入一个虚拟的初始化按键
	row=ReadData(AUTHOR_ROW_L);
	recordrow=ReadData(RECORD_ROW_L);
	for(epi=0,hangj=0;epi<row;epi++,hangj++)
	{
	tempdata[0]=ReadData(AUTHOR_START+2+epi*24);
	tempdata[1]=ReadData(AUTHOR_START+3+epi*24);
	tempsuoma[0]=ReadData(AUTHOR_START+4+epi*24);
  tempsuoma[1]=ReadData(AUTHOR_START+5+epi*24);
	tempsuoma[2]=ReadData(AUTHOR_START+6+epi*24);
	tempsuoma[3]=ReadData(AUTHOR_START+7+epi*24);
		for(epr=0;epr<recordrow;epr++)
		{
			recordsuoma[0]=ReadData(RECORD_START+0+epr*5);
			recordsuoma[1]=ReadData(RECORD_START+1+epr*5);
			recordsuoma[2]=ReadData(RECORD_START+2+epr*5);
			recordsuoma[3]=ReadData(RECORD_START+3+epr*5);
			if(tempsuoma[0]==recordsuoma[0]&&tempsuoma[1]==recordsuoma[1]&&tempsuoma[2]==recordsuoma[2]&&tempsuoma[3]==recordsuoma[3])
			{
				if(ReadData(RECORD_START+4+epr*5)==1)
					OLED_ShowUnite(80,32+hangj*16,"已检查");
				else
					OLED_ShowUnite(80,32+hangj*16,"未检查");
      }
			else
				  OLED_ShowUnite(80,32+hangj*16,"未检查");
		}
	display_shuzi_zijie(16,32+hangj*16, tempdata, Green, Yellow);
	//hangj=hangj-48;
	if(epi>=6)
	{
		//temp_f_CurSel=1;
		k++;                                               //页数
		break;
		
	}
	}
  while(1)
  {
		
		//OLED_ShowUnite(32,0,lcd_buf2);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else{
			if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//delay_ms(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
	  if(nChar == KEYUP)	//上翻	++
		{
			  i=0;
			  j=0;
				if (k!=0)
				{
					OLED_Clear();
				  ShowMenu(JIANCHA);
          for(j=0,epi=0+7*k,hangj=0;epi<row;epi++,hangj++)
	        {
	           tempdata[0]=ReadData(AUTHOR_START+2+(epi+k)*24);
	           tempdata[1]=ReadData(AUTHOR_START+3+(epi+k)*24);
						 tempsuoma[0]=ReadData(AUTHOR_START+4+(epi+k)*24);
             tempsuoma[1]=ReadData(AUTHOR_START+5+(epi+k)*24);
	           tempsuoma[2]=ReadData(AUTHOR_START+6+(epi+k)*24);
	           tempsuoma[3]=ReadData(AUTHOR_START+7+(epi+k)*24);
							for(epr=0;epr<recordrow;epr++)
									{
										recordsuoma[0]=ReadData(RECORD_START+0+epr*5);
										recordsuoma[1]=ReadData(RECORD_START+1+epr*5);
										recordsuoma[2]=ReadData(RECORD_START+2+epr*5);
										recordsuoma[3]=ReadData(RECORD_START+3+epr*5);
										if(tempsuoma[0]==recordsuoma[0]&&tempsuoma[1]==recordsuoma[1]&&tempsuoma[2]==recordsuoma[2]&&tempsuoma[3]==recordsuoma[3])
										{
											if(ReadData(RECORD_START+4+epr*5)==1)
												OLED_ShowUnite(80,32+hangj*16,"已检查");
											else
												OLED_ShowUnite(80,32+hangj*16,"未检查");
										}
										else
				                OLED_ShowUnite(80,32+hangj*16,"未检查");
									}
	           display_shuzi_zijie(16,32+hangj*16, tempdata, Green, Yellow);
	           hangj=hangj-48;
	           if(epi>=6+7*k)
							{
								 k++;
								 jj=1;
								 //temp_f_CurSel=1;
								 Init_KeyNumuber();
								 break;
							}
		       }			 
							 if(epi<6+7*(k-jj))
									{
										//temp_f_CurSel=0;
										k=0;
										nChar = KEYINIT;
										delay_ms(1000);
									}	 
						}
		 if(k==0 && nChar == KEYUP)
		 {
					OLED_Clear();
				  ShowMenu(JIANCHA);
					for(epi=0,hangj=0;epi<row;epi++,hangj++)
						{
						tempdata[0]=ReadData(AUTHOR_START+2+epi*24);
						tempdata[1]=ReadData(AUTHOR_START+3+epi*24);
						tempsuoma[0]=ReadData(AUTHOR_START+4+epi*24);
            tempsuoma[1]=ReadData(AUTHOR_START+5+epi*24);
	          tempsuoma[2]=ReadData(AUTHOR_START+6+epi*24);
	          tempsuoma[3]=ReadData(AUTHOR_START+7+epi*24);
						for(epr=0;epr<recordrow;epr++)
									{
										recordsuoma[0]=ReadData(RECORD_START+0+epr*5);
										recordsuoma[1]=ReadData(RECORD_START+1+epr*5);
										recordsuoma[2]=ReadData(RECORD_START+2+epr*5);
										recordsuoma[3]=ReadData(RECORD_START+3+epr*5);
										if(!strcmp(tempsuoma,recordsuoma))
										{
											if(ReadData(RECORD_START+4+epr*5)==1)
												OLED_ShowUnite(80,32+hangj*16,"已检查");
											else
												OLED_ShowUnite(80,32+hangj*16,"未检查");
										}
										else
				                OLED_ShowUnite(80,32+hangj*16,"未检查");
									}
						display_shuzi_zijie(16,32+hangj*16, tempdata, Green, Yellow);
							if(epi>=6)
							{
								//temp_f_CurSel=1;
								k++;      								//页数
								break;
							}
						}
						Init_KeyNumuber();
			}
		}
// 	 if(nChar ==KEYDOWN)//下翻	--
// 		{
// 					i=0;
//		      j=0;
// 					//ResetTimer(TIMER_TIMEOUT);
// 					if (f_CurSel<HsMenu[0].col)
// 							++f_CurSel;
// 					else
// 							f_CurSel =  HsMenu[0].row;
// 					OLED_Clear();
// 					ShowMenu(HsMenu);
// 					OLED_ShowUnite(32,0,lcd_buf2);	
// 					/*if(Power_Cheak() ==0)
// 					{
// 				  OLED_ShowUnite(96,0,lcd_buf_batt0);	
// 						}
// 					if(Power_Cheak() ==1)
// 					{
// 				  OLED_ShowUnite(96,0,lcd_buf_batt3);	
// 						}
// 					if(Power_Cheak() ==2)
// 					{
// 				  OLED_ShowUnite(96,0,lcd_buf_batt1);	
// 						}
// 						if(Power_Cheak() ==3)
// 					{
// 				  OLED_ShowUnite(96,0,lcd_buf_batt2);	
// 						}
// 			if(USB_Cheak_init()==1)
// 			{
// 				OLED_ShowUnite(96,0,lcd_buf_signal);
//       }
// 			else
// 			{
// 				OLED_ShowUnite(96,0,lcd_buf_nosignal);
//       }*/
// 					Init_KeyNumuber();
// 					delay_ms(1);
//           GPIOC->IDR= GPIO_Pin_8;

// 		}
/*		 if(nChar ==KEYSURE) //确定
		{
					i=0;
			    j=0; 
					//KillTimer(TIMER_TIMEOUT);
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
if( flag==0&&nChar ==KEYCLEAR)//取消
		{
			    i=0;
			    j++;
					//KeyNum =KEYNO;
			    delay_ms(100);
		   

		}
		if(j>=30)//按下超过3秒钟
		      {
		  	    j=0;
					  temp_f_CurSel=f_CurSel;
					  Choose_MenuOp4_3();
						//if(f_CurSel==2)	
						//break;
						f_CurSel=temp_f_CurSel;				
						ShowMenu(TOOLMENU);	
		      }
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
		{
			    i=0;
			    j=0;
					//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}*/




/*
*@功能：屏保与关于菜单界面
*/
void Choose_MenuOp5(void)
{
  uint8_t bootup_flag=0,i=0,j=0,nChar,temp_f_CurSel;

  f_CurSel =SETMENU[0].row;

	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(SETMENU);		//显示菜单
	Main_Oled_Power();
	Main_Oled_Time();	
  while(1)
  {		
		//OLED_ShowUnite(32,0,lcd_buf2);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		
		if(guanjiflag==1)
			break;
		else
		{
			
			//超时关屏
			if(nChar ==KEYINIT)  
			{
					i++;
					delay_ms(100);
					if(i>=choosetime) //按下超过10秒钟
					{
						OLED_Display_Off();
						bootup_flag=1;
						i=0;
					}
			}
			//任意键开屏
			if(bootup_flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
			{
					OLED_Display_On();
					bootup_flag=0;
					delay_ms(1000);
					nChar =KEYINIT;
			}
			
			
			if(nChar == KEYUP)	//上翻	++
			{
					i=0;
					j=0;			
					//ResetTimer(TIMER_TIMEOUT);		  //还没实现
					if (f_CurSel>SETMENU[0].row)		  //HsMenu[0].row是最小
							--f_CurSel;
					else if(f_CurSel<=SETMENU[0].row)															//HsMenu[0].row是最小
							f_CurSel=SETMENU[0].col;		//HsMenu[0].col是最大
					OLED_Clear();
					ShowMenu(SETMENU);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
					Main_Oled_Power();
					Main_Oled_Time();						
					Init_KeyNumuber();
			}
			if(nChar ==KEYDOWN)//下翻	--
			{
					i=0;
					j=0;	
					//ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel<SETMENU[0].col)			//HsMenu[0].col是最大
							++f_CurSel;
					else if(f_CurSel>=SETMENU[0].col)
							f_CurSel=SETMENU[0].row;		//HsMenu[0].row是最小
					OLED_Clear();
					ShowMenu(SETMENU);
					Main_Oled_Power();
					Main_Oled_Time();						
					
					Init_KeyNumuber();
			}
			if(nChar ==KEYSURE) //确定
			{
					i=0;
					j=0;	 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
					temp_f_CurSel=f_CurSel;
					SETMENU[f_CurSel].pFunc();				//f_CurSel表示当前的行
					f_CurSel=temp_f_CurSel;
					ShowMenu(SETMENU);
					Main_Oled_Power();
					Main_Oled_Time();					
					Init_KeyNumuber();
			}
			if( bootup_flag==0&&nChar ==KEYCLEAR)//取消
			{
					i=0;
					j++;
					//KeyNum =KEYNO;
					delay_ms(100);
			}
			
			
			//长按关机判断
			if(j>=30)//按下超过3秒钟
			{
					j=0;
					temp_f_CurSel=f_CurSel;
					Choose_MenuOp4_3();
					//if(f_CurSel==2)	
					//break;
					f_CurSel=temp_f_CurSel;				
					ShowMenu(SETMENU);	
			}
			else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
			{
					i=0;
					j=0;
					//KeyNum =KEYNO;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;
			}
    }
  }
	
	
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}


/*
*@功能：设置屏保时间
*/
void Choose_MenuOp5_1(void)
{
  uint8_t flag=0,i=0,j=0,nChar,temp_f_CurSel;

  f_CurSel=TIME[0].row;


	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(TIME);		//显示菜单
	Main_Oled_Power();
	Main_Oled_Time();	
	
  while(1)
  {
		
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else
		{
			if(nChar ==KEYINIT)  //超时关屏
		  {
				i++;
				//delay_ms(100);
				delay_ms(100);
				if(i>=choosetime)//按下超过10秒钟
				{
					OLED_Display_Off();
					flag=1;
					i=0;
				}
		  }
			
			
			if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
			{
				OLED_Display_On();
				flag=0;
				delay_ms(1000);
				nChar =KEYINIT;
			}
			
			
			if(nChar == KEYUP)	//上翻	++
			{
					i=0;
					j=0;			
					//ResetTimer(TIMER_TIMEOUT);		//还没实现
					if (f_CurSel>TIME[0].row)		//HsMenu[0].row是最小
							--f_CurSel;
					else if(f_CurSel<=TIME[0].row) //HsMenu[0].row是最小
							f_CurSel = TIME[0].col;		//HsMenu[0].col是最大
					OLED_Clear();
					ShowMenu(TIME);		//此时f_CurSel已经改变，清屏之后再显示时反白的行会改变
					Main_Oled_Power();
					Main_Oled_Time();						
					Init_KeyNumuber();
			}
			if(nChar ==KEYDOWN)//下翻	--
			{
					i=0;
					j=0;
					//ResetTimer(TIMER_TIMEOUT);
					if (f_CurSel<TIME[0].col)
							++f_CurSel;
					else if(f_CurSel>=TIME[0].col)
							f_CurSel =  TIME[0].row;
					OLED_Clear();
					ShowMenu(TIME);
					Main_Oled_Power();
					Main_Oled_Time();										
					Init_KeyNumuber();
			}
			if(nChar ==KEYSURE) //确定
			{
					i=0;
					j=0; 

					//OLED_Clear();
					if(f_CurSel==3)
						choosetime=1000;
					else if(f_CurSel==4)
						choosetime=300;
				  else if(f_CurSel==5)
						choosetime=500;
					
					g_State = ST_IDLE;

					Choose_MenuOp5_2();
					OLED_Clear();
					break;		//返回上一界面
			}
			if( flag==0&&nChar ==KEYCLEAR)//取消
			{
						i=0;
						j++;
						//KeyNum =KEYNO;
						delay_ms(100);
			}
			
			
			if(j>=30)//按下超过3秒钟
			{
				j=0;
				temp_f_CurSel=f_CurSel;
				Choose_MenuOp4_3();
				//if(f_CurSel==2)	
				//break;
				f_CurSel=temp_f_CurSel;				
				ShowMenu(TIME);	
			}
			else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消
			{
						i=0;
						j=0;
						//KeyNum =KEYNO;
						OLED_Clear();
						g_State = ST_IDLE;
						ResetKey();
						break;
			}
		}
	}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}







/*
*@功能：屏保设置成功
*/
void Choose_MenuOp5_2(void)
{
  uint8_t flag=0,i=0,j=0,nChar,temp_f_CurSel;

  f_CurSel =VERSION[0].row;


	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(SETSUCCESS);		//显示菜单
	Main_Oled_Power();
	Main_Oled_Time();		
	
  while(1)
  {
		
		//OLED_ShowUnite(32,0,lcd_buf2);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else
		{
		if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//delay_ms(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
		
		//此时只有确定键有效，其他无效
		if(nChar ==KEYSURE) //确定
		{
					i=0;
          j=0; 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
          g_State = ST_IDLE;
					ResetKey();
					break;		
		}

		if(j>=30)//按下超过3秒钟
		{
			j=0;
			temp_f_CurSel=f_CurSel;
			Choose_MenuOp4_3();
			//if(f_CurSel==2)	
			//break;
			f_CurSel=temp_f_CurSel;				
			ShowMenu(VERSION);	
		}
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消 
		{
					//KeyNum =KEYNO;
			    i=0;
			    j=0;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}


/*
*@功能：版本界面
*/
void Choose_MenuOp5_3(void)
{
  uint8_t flag=0,i=0,j=0,nChar,temp_f_CurSel;

  f_CurSel =VERSION[0].row;


	delay_ms(100);
	OLED_Clear();	    //清屏
	ShowMenu(VERSION);		//显示菜单
	Main_Oled_Power();
	Main_Oled_Time();		
	
  while(1)
  {
		
		//OLED_ShowUnite(32,0,lcd_buf2);
		Init_KeyNumuber();			
		nChar = GetKey();   //读取按键缓冲区
		if(guanjiflag==1)
			break;
		else
		{
		if(nChar ==KEYINIT)  //超时关屏
		{
			i++;
			//delay_ms(100);
			delay_ms(100);
		  if(i>=choosetime)//按下超过10秒钟
		  {
		  	OLED_Display_Off();
			  flag=1;
		  	i=0;
		  }
		}
		if(flag==1&&(nChar ==KEYCLEAR||nChar ==KEYSURE||nChar ==KEYDOWN||nChar == KEYUP))
		{
			OLED_Display_On();
			flag=0;
			delay_ms(1000);
			nChar =KEYINIT;
    }
		
		//此时只有确定键有效，其他无效
		if(nChar ==KEYSURE) //确定
		{
					i=0;
          j=0; 
					//KillTimer(TIMER_TIMEOUT);
					//g_WaitTimeOut = 0;
					OLED_Clear();
          g_State = ST_IDLE;
					ResetKey();
					break;		
		}

		if(j>=30)//按下超过3秒钟
		{
			j=0;
			temp_f_CurSel=f_CurSel;
			Choose_MenuOp4_3();
			//if(f_CurSel==2)	
			//break;
			f_CurSel=temp_f_CurSel;				
			ShowMenu(VERSION);	
		}
		else if (nChar !=KEYCLEAR&&j>0&&j<20)//取消 
		{
					//KeyNum =KEYNO;
			    i=0;
			    j=0;
					OLED_Clear();
					g_State = ST_IDLE;
					ResetKey();
					break;

		}
  }
}
  //KillTimer(TIMER_TIMEOUT);
  g_State =ST_IDLE;
  return;
}



/*
*@功能：显示电池电量
*/
void Main_Oled_Power(void)
{

	OLED_ClearArea(96,0,128,2);
	OLED_DrawBMP(96,0,128,2,OLED_buf_batt0);	
	
// 	if(Power_Cheak() ==0)
// 	{
// 		OLED_DrawBMP(96,0,128,0,OLED_buf_batt0);	
// 	}
// 	else if(Power_Cheak() ==1)
// 	{
// 		OLED_DrawBMP(96,0,128,0,OLED_buf_batt1);
// 	}
// 	else if(Power_Cheak() ==2)
// 	{
// 		OLED_DrawBMP(96,0,128,0,OLED_buf_batt2);	
// 	}
// 	else if(Power_Cheak() ==3)
// 	{
// 		OLED_DrawBMP(96,0,128,0,OLED_buf_batt3);	
// 	}
					
// 	if((USB_CHEAK&(GPIO_Pin_12))!=0)  //USB_CHEAK ==(GPIO_Pin_12)
// 	{
// 		OLED_DrawBMP(96,0,127,15,lcd_buf_signal);
// 	}
// 	else if((USB_CHEAK&(GPIO_Pin_12))==0)
// 	{
// 		OLED_DrawBMP(96,0,127,15,lcd_buf_nosignal);
// 	}

}



void Main_Oled_Time(void)
{
	uint8_t  tempa;
  uint8_t  oled_buf_ymd[15],oled_buf_hms[15];		//oled_buf_ymd存年月日，oled_buf_hms存时分秒
	uint8_t oled_show_time[12];
  read_time();  																//读取实时时间,放到time_buffer中 
	oled_buf_ymd[0] = '2';
	oled_buf_ymd[1] = '0';
	
	//年
	tempa = (time_buff[0]>>4)&0x0f;
	oled_buf_ymd[2] = tempa+0x30;		//转化为字符
	tempa = (time_buff[0]&0x0f);
	oled_buf_ymd[3] = tempa + 0x30;
	
	oled_buf_ymd[4] = 0xc4;
	oled_buf_ymd[5] = 0xea;   //年的内码
	
	//月
	tempa = (time_buff[1]>>4)&0x0f;
	oled_buf_ymd[6] = tempa+0x30;
	tempa = time_buff[1]&0x0f;
	oled_buf_ymd[7] = tempa+0x30;
	
	oled_buf_ymd[8] = 0xd4;
	oled_buf_ymd[9] = 0xc2;   //月的内码
	
	//日
	tempa = (time_buff[2]>>4)&0x0f;
	oled_buf_ymd[10] = tempa+0x30;
	tempa = time_buff[2]&0x0f;
	oled_buf_ymd[11] = tempa+0x30;
	
	oled_buf_ymd[12] = 0xc8;
	oled_buf_ymd[13] = 0xd5;   //日的内码
	oled_buf_ymd[14] = 0;   //结束标志
	
	
	
	//时
	tempa = (time_buff[3]>>4)&0x0f;
	oled_buf_hms[0] = tempa+0x30;
	tempa = time_buff[3]&0x0f;
	oled_buf_hms[1] = tempa + 0x30;
	
	oled_buf_hms[2] = 0xCA;
	oled_buf_hms[3] = 0xB1;   //时的内码
	
	//分
	tempa = (time_buff[4]>>4)&0x0f;
	oled_buf_hms[4] = tempa+0x30;
	tempa = time_buff[4]&0x0f;
	oled_buf_hms[5] = tempa+0x30;
	
	oled_buf_hms[6] = 0xb7;
	oled_buf_hms[7] = 0xd6;   //分的内码
	
	//秒
	tempa = (time_buff[5]>>4)&0x0f;
	oled_buf_hms[8] = tempa+0x30;
	tempa = time_buff[5]&0x0f;
	oled_buf_hms[9] = tempa+0x30;
	
	oled_buf_hms[10] = 0xc3;
	oled_buf_hms[11] = 0xeb;   //秒的内码
	oled_buf_hms[12] = 0;   //结束标志
	
	
	oled_show_time[0]=oled_buf_ymd[6];
	oled_show_time[1]=oled_buf_ymd[7];
	oled_show_time[2]='/';
	oled_show_time[3]=oled_buf_ymd[10];
	oled_show_time[4]=oled_buf_ymd[11];	
	
	oled_show_time[5]=' ';
	
	oled_show_time[6]=oled_buf_hms[0];
	oled_show_time[7]=oled_buf_hms[1];
	oled_show_time[8]=':';
	oled_show_time[9]=oled_buf_hms[4];
	oled_show_time[10]=oled_buf_hms[5];
 	oled_show_time[11]=0;
	
	OLED_ClearArea(0,0,96,2);
	OLED_ShowUnite(0,0,oled_show_time);	
}


// void Main_Oled_Show(void)
// {












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
// 		if(enshow==0&&t<(len-1))
// 		{
// 			if(temp==0)
// 			{
// 				OLED_ShowChar(x+(size/2)*t,y,' ');
// 				continue;
// 			}
// 			else
// 				enshow=1; 
// 		}
		
		
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


















