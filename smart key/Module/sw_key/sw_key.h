/**********************************************************************
*												smart key
*										 Module：按键模块
*						Copyright(c) 2015-2020 武汉百景互动有限公司
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       沈涛     2015-06-29 19:03     1.0             build this file 
*
* 模块功能：
*				按键
*				
**********************************************************************/ 



#ifndef __SWKEY_H
#define __SWKEY_H

#include "stm32f10x.h"


#define  KEY1	     0X01
#define  KEY2		   0X02
#define  KEY3		   0X03	
#define  KEY4		   0X04				 
#define  KEYNO		 0XFF		
#define  KEYINIT	 0XFE				 



/*
按键对应的功能定义
*/
#define  KEYUP         KEY1		 //按键确认
#define  KEYCLEAR      KEY2		 //按键向上
#define  KEYDOWN       KEY3		 //按键向下
#define  KEYSURE       KEY4	   //按键取消


// #define  KEYSURE       KEY1		 //按键确认
// #define  KEYUP         KEY2		 //按键向上
// #define  KEYDOWN       KEY3		 //按键向下
// #define  KEYCLEAR      KEY4	   //按键取消


#define GPIO_PIN_KEY1_BUTTON               GPIO_Pin_5
#define GPIO_PIN_KEY2_BUTTON               GPIO_Pin_13
#define GPIO_PIN_KEY3_BUTTON               GPIO_Pin_8
#define GPIO_PIN_KEY4_BUTTON               GPIO_Pin_15


#define EXTI_LINE_KEY1_BUTTON              EXTI_Line5
#define GPIO_PORT_SOURCE_KEY1_BUTTON       GPIO_PortSourceGPIOB
#define GPIO_PIN_SOURCE_KEY1_BUTTON        GPIO_PinSource5

#define EXTI_LINE_KEY2_BUTTON              EXTI_Line13
#define GPIO_PORT_SOURCE_KEY2_BUTTON       GPIO_PortSourceGPIOC
#define GPIO_PIN_SOURCE_KEY2_BUTTON        GPIO_PinSource13

#define EXTI_LINE_KEY3_BUTTON              EXTI_Line8
#define GPIO_PORT_SOURCE_KEY3_BUTTON       GPIO_PortSourceGPIOB
#define GPIO_PIN_SOURCE_KEY3_BUTTON        GPIO_PinSource8

#define EXTI_LINE_KEY4_BUTTON              EXTI_Line15
#define GPIO_PORT_SOURCE_KEY4_BUTTON       GPIO_PortSourceGPIOA
#define GPIO_PIN_SOURCE_KEY4_BUTTON        GPIO_PinSource15




#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入





#define WKUP_KD PCin(13)  //PC15 检测是否外部WK_UP按键按下









extern uint8_t Key_Num;


void Init_KeyNumuber(void);
void Init_Sw_Key_Io(void);
void Exit_Line_Init(void);
uint8_t GetKey(void);
void InsertKey(uint8_t nChar);
void ResetKey(void);
void Scan_Key(void);




void EXTILine_Mask(uint32_t EXTI_Linex);
void EXTILine_Unmask(uint32_t EXTI_Linex);


void Mask_Key_Interrupt(void);
void Unmask_Key_Interrupt(void);


#endif
