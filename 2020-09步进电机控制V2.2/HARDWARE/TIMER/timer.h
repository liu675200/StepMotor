#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


/*-------1--------*/
#define MOTOR_PWM1 PCout(4)	// PA
#define MOTOR_DIR1 PCout(5)	// PA

#define MOTOR_PWM2 PAout(6)	// PA
#define MOTOR_DIR2 PAout(7)	// PA


#define MOTOR_PWM3 PAout(4)	// PA
#define MOTOR_DIR3 PAout(5)	// PA

#define MOTOR_PWM4 PAout(2)	// PA
#define MOTOR_DIR4 PAout(3)	// PA

#define MOTOR_PWM5 PAout(11)	// PA
#define MOTOR_DIR5 PAout(12)	// PA


/*------2---------*/

#define MOTOR_PWM6 PCout(2)	// PA
#define MOTOR_DIR6 PCout(3)	// PA

#define MOTOR_PWM7 PCout(1)	// PA
#define MOTOR_DIR7 PCout(0)	// PA

#define MOTOR_PWM8 PBout(12)	// PA
#define MOTOR_DIR8 PBout(14)	// PA

#define MOTOR_PWM9 PCout(6)	// PA
#define MOTOR_DIR9 PCout(7)	// PA

#define MOTOR_PWM10 PCout(8)	// PA
#define MOTOR_DIR10 PCout(9)	// PA


/*------3---------*/
#define MOTOR_PWM11 PBout(0)	// PA
#define MOTOR_DIR11 PBout(1)	// PA

#define MOTOR_PWM12 PCout(11)	// PA
#define MOTOR_DIR12 PCout(10)	// PA

#define MOTOR_PWM13 PBout(4)	// PA
#define MOTOR_DIR13 PBout(3)	// PA

#define MOTOR_PWM14 PBout(6)	// PA
#define MOTOR_DIR14 PBout(5)	// PA

#define MOTOR_PWM15 PBout(8)	// PA
#define MOTOR_DIR15 PBout(7)	// PA

/******/
#define StartDelayTimes 	1		//启动实际时间为10+1秒
#define DelayMotor1Times 	2		//启动实际时间为10+2秒
#define DelayMotor2Times 	3		//启动实际时间为10+3秒

void TIM3_Int_Init(u16 arr,u16 psc); 
void PWM_GPIO_Init(void);
void MotorId(u8 id,u8 dir);

#endif


