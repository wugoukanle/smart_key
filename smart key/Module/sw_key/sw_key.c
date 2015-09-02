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
*
* 连线说明：
*				S1			PB5
*				S2			PC13
*				S3			PB8
*				S4			PA15
*
*     补充：在Scan_Key()中将外部按键引脚与硬件宏（KEYX）联系起来
*						再将硬件宏与功能宏（UP，DOWN,SURE,CLEAR）联系起来
*						
**********************************************************************/ 


#include "sw_key.h"
#include "stm32f10x_exti.h"
#include "delay.h"
#include "Globle_Var.h"









/*
*@功能：插入键值
*
*/
void InsertKey(uint8_t nChar)
{
	Key_Num =nChar;
}




/*
*@功能：初始化键值
*
*/
void Init_KeyNumuber(void)
{
   Key_Num =KEYINIT;
	
// GPIOC->IDR= GPIO_Pin_8;
// GPIOC->IDR= GPIO_Pin_9;
// GPIOC->IDR= GPIO_Pin_10;
// GPIOC->IDR= GPIO_Pin_11;
	
	
	//GPIOC->BSRR = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
}



/*
*@功能：复位键值
*
*/
void ResetKey(void)
{
	Key_Num =KEYNO;
}






/*
*@功能：读取按键值的存储缓冲区
*/
uint8_t GetKey(void)
{
	uint8_t KN;
  //if((Key_Num!=KEYNO) &&(Key_Num!=KEYINIT))		//KEYNO=0xFF,KEYINIT=0xFE
// 	if(Key_Num!=KEYNO)		//KEYNO=0xFF,KEYINIT=0xFE
//   {
//     Scan_Key();		//扫描按键，修改Key_Num
//   }
  KN =Key_Num;
  return KN;
}



void Scan_Key(void)
{
	/*这里是将按键IO口与宏（KEY）联系起来*/
// 	if(((GPIOB->IDR) & GPIO_Pin_5 )==0) Key_Num=KEY1; //看来是低电平表示按键按下
// 	if(((GPIOC->IDR) & GPIO_Pin_13)==0) Key_Num=KEY2;	
// 	if(((GPIOB->IDR) & GPIO_Pin_8 )==0) Key_Num=KEY3; 
// 	if(((GPIOA->IDR) & GPIO_Pin_15)==0) Key_Num=KEY4; 
	

	
	
// 	switch(Key_Num)		//需要可以改变Key_Num值的地方
// 	{	
// 	 //延时是为了去抖动
// 		case KEY1: delay_ms(50);  if(((GPIOB->IDR) & GPIO_Pin_5 )==0)  Key_Num=KEY1;break;		//看来是低电平表示按键按下
// 		case KEY2: delay_ms(50);	if(((GPIOC->IDR) & GPIO_Pin_13)==0)  Key_Num=KEY2;break;		
// 		case KEY3: delay_ms(50);	if(((GPIOB->IDR) & GPIO_Pin_8 )==0)  Key_Num=KEY3;break;
// 		case KEY4: delay_ms(50);	if(((GPIOA->IDR) & GPIO_Pin_15)==0)  Key_Num=KEY4;break;
// 	}
//  //修改Key_Num这个文件中的全局变量

	
	//delay_ms(200);
	//修改之后，Key_Num的改变将在按键中断之中发生
	
}




// void Scan_Key() 
// { 
//         unsigned char num_key=0;//按键号 
//         unsigned char temp=0;//用于读取P2线上按键值 
//         static unsigned char temp_code=0;//保存按键值 
//         static unsigned char num_check=0;//低电平有效次数 
//         static unsigned char key_flag=0;//按键有效标识 

//         temp=GPIOC->IDR&0x0E00;//读取P2线数据 

//         if(temp!=0x0E00)//低电平判断 
//         { 
//                 num_check++; 
//                 if(num_check>9)//连续10次(10ms)低电平有效,则认为按键有效 
//                 { 
//                         key_flag=1;//使能按键有效标识 
//                         temp_code=temp;//保存按键值 
//                 } 
//         } 
//         else//松手时判断 
//         { 
//                 num_check=0; 

//                 if(key_flag==1)//按键有效 
//                 { 
//                         key_flag=0; 

//                         switch(temp_code)//读取按键号 
//                         { 
// //                              case 0x0E00: num_key=1; 
// //                                            break; 
//                                 case 0x0C00: num_key=2; 
//                                            break; 
//                                 case 0x0A00: num_key=3; 
//                                            break; 
//                                 case 0x0600: num_key=4; 
//                                            break; 
//                         } 
//                 } 
//         } 
//         Key_Num=num_key; 
// } 













/*
*@功能：外部中断的配置
*       下降沿触发中断的模式
 */
 void Exit_Line_Init(void)
 {	
  
   EXTI_InitTypeDef EXTI_InitStructure;
//	 NVIC_InitTypeDef NVIC_InitStructure;   //定义一个结构体	 
	 
	 
	/* Connect Key Button EXTI Line to Key Button GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY1_BUTTON, GPIO_PIN_SOURCE_KEY1_BUTTON);			

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY1_BUTTON;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling ;//EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  //EXTI_GenerateSWInterrupt(EXTI_LINE_KEY2_BUTTON);

	 
	 
	 
	 
   /* Connect Key Button EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY2_BUTTON, GPIO_PIN_SOURCE_KEY2_BUTTON);

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY2_BUTTON;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  //EXTI_GenerateSWInterrupt(EXTI_LINE_KEY2_BUTTON);




   /* Connect Key Button EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY3_BUTTON, GPIO_PIN_SOURCE_KEY3_BUTTON);

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY3_BUTTON;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

/* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  //EXTI_GenerateSWInterrupt(EXTI_LINE_KEY3_BUTTON);





   /* Connect Key Button EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY4_BUTTON, GPIO_PIN_SOURCE_KEY4_BUTTON);

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY4_BUTTON;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  //EXTI_GenerateSWInterrupt(EXTI_LINE_KEY4_BUTTON);
	
	
	
	
//   /* Enable the EXTI9_5 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);


//   /* Enable the EXTI15_10 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);		
	
}





void Init_Sw_Key_Io(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE);//GPIO做EXTI外部中断时必须开启复用时钟
	
	
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY1_BUTTON;		//按键KEY1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY2_BUTTON;		//按键KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY3_BUTTON;		//按键KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
 	
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY4_BUTTON;		//按键KEY4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}


/*
*@功能：外部中断线的屏蔽设置
*@参数：EXTI_Linex:x=0~15

中断屏蔽寄存器EXTI_IMR上的位MRx
0：屏蔽来自线x上的中断请求；
1：开放来自线x上的中断请求。
*/
void EXTILine_Mask(uint32_t EXTI_Linex)
{
	
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));	
	
	//EXTI->PR=EXTI_Linex;  //清除LINE11上的中断标志位
  EXTI->IMR&=~(EXTI_Linex);//对应位置零，屏蔽line11上的中断   
}

/*
*@功能：外部中断线的不屏蔽设置
*@参数：EXTI_Linex:x=0~15

中断屏蔽寄存器EXTI_IMR上的位MRx
0：屏蔽来自线x上的中断请求；
1：开放来自线x上的中断请求。
*/
void EXTILine_Unmask(uint32_t EXTI_Linex)
{
	
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));
	
	//EXTI->PR=EXTI_Linex;  //清除LINE11上的中断标志位
	EXTI->IMR|=EXTI_Linex;//对应位置一，不屏蔽line11上的中断即开放对应的中断
}



void Mask_Key_Interrupt(void)
{
		EXTILine_Mask(EXTI_Line5);
		EXTILine_Mask(EXTI_Line8);
		EXTILine_Mask(EXTI_Line13);
		EXTILine_Mask(EXTI_Line15);
}


void Unmask_Key_Interrupt(void)
{
		EXTILine_Unmask(EXTI_Line5);
		EXTILine_Unmask(EXTI_Line8);
		EXTILine_Unmask(EXTI_Line13);
		EXTILine_Unmask(EXTI_Line15);
}



