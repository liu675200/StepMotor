#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
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
#define StartDelayTimes 	1		//����ʵ��ʱ��Ϊ10+1��
#define DelayMotor1Times 	2		//����ʵ��ʱ��Ϊ10+2��
#define DelayMotor2Times 	3		//����ʵ��ʱ��Ϊ10+3��

void TIM3_Int_Init(u16 arr,u16 psc); 
void PWM_GPIO_Init(void);
void MotorId(u8 id,u8 dir);

#endif


