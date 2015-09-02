/**********************************************************************
*												smart key
*										 Module������ģ��
*						Copyright(c) 2015-2020 �人�پ��������޹�˾
*										All Rights Reserved. 
*
* History:         
*				<author>    <time>           <version >             <desc>
*       ����     2015-06-29 19:03     1.0             build this file 
*
* ģ�鹦�ܣ�
*				����
*				
*
* ����˵����
*				S1			PB5
*				S2			PC13
*				S3			PB8
*				S4			PA15
*
*     ���䣺��Scan_Key()�н��ⲿ����������Ӳ���꣨KEYX����ϵ����
*						�ٽ�Ӳ�����빦�ܺ꣨UP��DOWN,SURE,CLEAR����ϵ����
*						
**********************************************************************/ 


#include "sw_key.h"
#include "stm32f10x_exti.h"
#include "delay.h"
#include "Globle_Var.h"









/*
*@���ܣ������ֵ
*
*/
void InsertKey(uint8_t nChar)
{
	Key_Num =nChar;
}




/*
*@���ܣ���ʼ����ֵ
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
*@���ܣ���λ��ֵ
*
*/
void ResetKey(void)
{
	Key_Num =KEYNO;
}






/*
*@���ܣ���ȡ����ֵ�Ĵ洢������
*/
uint8_t GetKey(void)
{
	uint8_t KN;
  //if((Key_Num!=KEYNO) &&(Key_Num!=KEYINIT))		//KEYNO=0xFF,KEYINIT=0xFE
// 	if(Key_Num!=KEYNO)		//KEYNO=0xFF,KEYINIT=0xFE
//   {
//     Scan_Key();		//ɨ�谴�����޸�Key_Num
//   }
  KN =Key_Num;
  return KN;
}



void Scan_Key(void)
{
	/*�����ǽ�����IO����꣨KEY����ϵ����*/
// 	if(((GPIOB->IDR) & GPIO_Pin_5 )==0) Key_Num=KEY1; //�����ǵ͵�ƽ��ʾ��������
// 	if(((GPIOC->IDR) & GPIO_Pin_13)==0) Key_Num=KEY2;	
// 	if(((GPIOB->IDR) & GPIO_Pin_8 )==0) Key_Num=KEY3; 
// 	if(((GPIOA->IDR) & GPIO_Pin_15)==0) Key_Num=KEY4; 
	

	
	
// 	switch(Key_Num)		//��Ҫ���Ըı�Key_Numֵ�ĵط�
// 	{	
// 	 //��ʱ��Ϊ��ȥ����
// 		case KEY1: delay_ms(50);  if(((GPIOB->IDR) & GPIO_Pin_5 )==0)  Key_Num=KEY1;break;		//�����ǵ͵�ƽ��ʾ��������
// 		case KEY2: delay_ms(50);	if(((GPIOC->IDR) & GPIO_Pin_13)==0)  Key_Num=KEY2;break;		
// 		case KEY3: delay_ms(50);	if(((GPIOB->IDR) & GPIO_Pin_8 )==0)  Key_Num=KEY3;break;
// 		case KEY4: delay_ms(50);	if(((GPIOA->IDR) & GPIO_Pin_15)==0)  Key_Num=KEY4;break;
// 	}
//  //�޸�Key_Num����ļ��е�ȫ�ֱ���

	
	//delay_ms(200);
	//�޸�֮��Key_Num�ĸı佫�ڰ����ж�֮�з���
	
}




// void Scan_Key() 
// { 
//         unsigned char num_key=0;//������ 
//         unsigned char temp=0;//���ڶ�ȡP2���ϰ���ֵ 
//         static unsigned char temp_code=0;//���水��ֵ 
//         static unsigned char num_check=0;//�͵�ƽ��Ч���� 
//         static unsigned char key_flag=0;//������Ч��ʶ 

//         temp=GPIOC->IDR&0x0E00;//��ȡP2������ 

//         if(temp!=0x0E00)//�͵�ƽ�ж� 
//         { 
//                 num_check++; 
//                 if(num_check>9)//����10��(10ms)�͵�ƽ��Ч,����Ϊ������Ч 
//                 { 
//                         key_flag=1;//ʹ�ܰ�����Ч��ʶ 
//                         temp_code=temp;//���水��ֵ 
//                 } 
//         } 
//         else//����ʱ�ж� 
//         { 
//                 num_check=0; 

//                 if(key_flag==1)//������Ч 
//                 { 
//                         key_flag=0; 

//                         switch(temp_code)//��ȡ������ 
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
*@���ܣ��ⲿ�жϵ�����
*       �½��ش����жϵ�ģʽ
 */
 void Exit_Line_Init(void)
 {	
  
   EXTI_InitTypeDef EXTI_InitStructure;
//	 NVIC_InitTypeDef NVIC_InitStructure;   //����һ���ṹ��	 
	 
	 
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
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE);//GPIO��EXTI�ⲿ�ж�ʱ���뿪������ʱ��
	
	
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY1_BUTTON;		//����KEY1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY2_BUTTON;		//����KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
   
    
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY3_BUTTON;		//����KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
 	
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY4_BUTTON;		//����KEY4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}


/*
*@���ܣ��ⲿ�ж��ߵ���������
*@������EXTI_Linex:x=0~15

�ж����μĴ���EXTI_IMR�ϵ�λMRx
0������������x�ϵ��ж�����
1������������x�ϵ��ж�����
*/
void EXTILine_Mask(uint32_t EXTI_Linex)
{
	
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));	
	
	//EXTI->PR=EXTI_Linex;  //���LINE11�ϵ��жϱ�־λ
  EXTI->IMR&=~(EXTI_Linex);//��Ӧλ���㣬����line11�ϵ��ж�   
}

/*
*@���ܣ��ⲿ�ж��ߵĲ���������
*@������EXTI_Linex:x=0~15

�ж����μĴ���EXTI_IMR�ϵ�λMRx
0������������x�ϵ��ж�����
1������������x�ϵ��ж�����
*/
void EXTILine_Unmask(uint32_t EXTI_Linex)
{
	
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));
	
	//EXTI->PR=EXTI_Linex;  //���LINE11�ϵ��жϱ�־λ
	EXTI->IMR|=EXTI_Linex;//��Ӧλ��һ��������line11�ϵ��жϼ����Ŷ�Ӧ���ж�
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



