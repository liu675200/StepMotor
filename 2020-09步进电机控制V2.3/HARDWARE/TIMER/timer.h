#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
 


/*-------1--------*/
#define MOTOR_PWM1 PCout(4)	// 
#define MOTOR_DIR1 PCout(5)	// 

#define MOTOR_PWM2 PAout(6)	// 
#define MOTOR_DIR2 PAout(7)	// 


#define MOTOR_PWM3 PAout(4)	// 
#define MOTOR_DIR3 PAout(5)	// 

#define MOTOR_PWM4 PAout(2)	// 
#define MOTOR_DIR4 PAout(3)	// 

#define MOTOR_PWM5 PAout(11)	// 
#define MOTOR_DIR5 PAout(12)	//


/*------2---------*/

#define MOTOR_PWM6 PCout(2)	// 
#define MOTOR_DIR6 PCout(3)	// 

#define MOTOR_PWM7 PCout(1)	// 
#define MOTOR_DIR7 PCout(0)	// 

#define MOTOR_PWM8 PBout(12)	// 
#define MOTOR_DIR8 PBout(14)	// 

#define MOTOR_PWM9 PCout(6)	// 
#define MOTOR_DIR9 PCout(7)	// 

#define MOTOR_PWM10 PCout(8)	// 
#define MOTOR_DIR10 PCout(9)	// 


/*------3---------*/
#define MOTOR_PWM11 PBout(0)	// 
#define MOTOR_DIR11 PBout(1)	// 

#define MOTOR_PWM12 PCout(11)	// 
#define MOTOR_DIR12 PCout(10)	// 

#define MOTOR_PWM13 PBout(4)	// 
#define MOTOR_DIR13 PBout(3)	// 

#define MOTOR_PWM14 PBout(6)	// 
#define MOTOR_DIR14 PBout(5)	// 

#define MOTOR_PWM15 PBout(8)	// 
#define MOTOR_DIR15 PBout(7)	// 

/******/
#define StartDelayTimes 	1		//启动实际时间为10+1秒
#define DelayMotor1Times 	2		//启动实际时间为10+2秒
#define DelayMotor2Times 	3		//启动实际时间为10+3秒

void TIM3_Int_Init(u16 arr,u16 psc); 		//定时器初始化
void PWM_GPIO_Init(void);		//电机控制引脚初始化
void MotorId(u8 id,u8 dir);		//电机脉冲函数

#endif


