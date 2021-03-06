#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "stdio.h"
#include "oled_display.h"
#include "laser.h"

 u8 mode;
 extern u16 delay_times;
 extern u16 dataBuf[51];
 extern u8 i;

//外部中断初始化函数
void EXTIX_Init(void)
{
 
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

	  KEY_Init();//初始化按键对应io模式

    //GPIOC.5 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.15	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.0	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

   	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
 
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
}

 
void EXTI0_IRQHandler(void)		//开启与暂停按钮初始化
{
  delay_ms(10);    //消抖
	if(WK_UP==1)		//如果按键被按下
	{	  	
		mode=!mode;		//进行模式切换
		
		if(mode!=0){			//开始
			TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
			LED0=0;
			i = 0;
			delay_times = 0;		//对延时时间清零
			dataBuf[50] = 0;
			dataBuf[49] = 0;
			OLED_ShowString(0,4,"System start... ",8);		//OLED提示系统开始运行
			printf("system start...\r\n");
//			MOTOR_DIR1=!MOTOR_DIR1;
		}else		//停止
		{
			TIM_Cmd(TIM3, DISABLE);  //失能TIMx外设
			LED0=1;
			OLED_ShowString(0,4,"System stop...  ",8);//OLED提示系统暂停
			printf("system stop...\r\n");
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
 void EXTI9_5_IRQHandler(void)
{			
//	delay_ms(10);   //消抖			 
//	if(KEY0==0)	{
//		
//	}
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}


void EXTI15_10_IRQHandler(void)
{
//  delay_ms(10);    //消抖			 
//  if(KEY1==0)	{
//		LED1=!LED1;
//	}
	 EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
}
