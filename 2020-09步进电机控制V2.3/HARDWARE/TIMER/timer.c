#include "timer.h"
#include "led.h"
#include "usart.h"
#include "laser.h"
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
u16 t = 0;
u16 times[16] = {0};			//��������������
u16 delay_times = 0;
u16 mlength[16]={0};  //��ǰ�ĸ߶�


extern u16 length[15];		//��ø߶�
extern u16 dataBuf[51];		//���ݻ���
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}




void PWM_GPIO_Init(void)		//�ӿڳ�ʼ��
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				 //�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��
 GPIO_ResetBits(GPIOA,GPIO_Pin_All);						 //�����
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				 //�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��
 GPIO_ResetBits(GPIOC,GPIO_Pin_All);						 //�����	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				 //�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��
 GPIO_ResetBits(GPIOB,GPIO_Pin_All);						 //�����
 

}

void MotorId(u8 id,u8 dir)		//��������������һ�����ID��һ���������dir����������15��������ٶȺͷ���   id 1~15,  dir 0 1		
{
	switch(id)
	{
		case 1:
				MOTOR_PWM1 =! MOTOR_PWM1,MOTOR_DIR1 = dir; break;
		case 2:
				MOTOR_PWM2 =! MOTOR_PWM2,MOTOR_DIR2 = dir; break;
		case 3:
				MOTOR_PWM3 =! MOTOR_PWM3,MOTOR_DIR3 = dir; break;
		case 4:
				MOTOR_PWM4 =! MOTOR_PWM4,MOTOR_DIR4 = dir; break;
		case 5:
				MOTOR_PWM5 =! MOTOR_PWM5,MOTOR_DIR5 = dir; break;
		case 6:
				MOTOR_PWM6 =! MOTOR_PWM6,MOTOR_DIR6 = dir; break;
		case 7:
				MOTOR_PWM7 =! MOTOR_PWM7,MOTOR_DIR7 = dir; break;
		case 8:
				MOTOR_PWM8 =! MOTOR_PWM8,MOTOR_DIR8 = dir; break;
		case 9:
				MOTOR_PWM9 =! MOTOR_PWM9,MOTOR_DIR9 = dir; break;
		case 10:
				MOTOR_PWM10 =! MOTOR_PWM10,MOTOR_DIR10 = dir; break;
		case 11:
				MOTOR_PWM11 =! MOTOR_PWM11,MOTOR_DIR11 = dir; break;
		case 12:
				MOTOR_PWM12 =! MOTOR_PWM12,MOTOR_DIR12 = dir; break;
		case 13:
				MOTOR_PWM13 =! MOTOR_PWM13,MOTOR_DIR13 = dir; break;
		case 14:
				MOTOR_PWM14 =! MOTOR_PWM14,MOTOR_DIR14 = dir; break;
		case 15:
				MOTOR_PWM15 =! MOTOR_PWM15,MOTOR_DIR15 = dir; break;
		default :break;
	}
}
void MotorMove(u8 uid,u8 mid)	//��������ƶ�����   id ÿ������������id	    mid �������id
{
	static u8 dir = 0;
	if(length[uid] > 245)		//����߶�С��5mm�����޶���
	{
		return;
	}
	if(length[uid] != mlength[mid]){		//��������ǰ�߶�������߶Ȳ���ͬ
			
			times[mid]++;				//����һ�����壬ʱ���1
				if(times[mid]>50)		//250�����壬�������½�10mm
				{
					times[mid] = 0;			//��ʱ�仺�������
					if(length[uid] < mlength[mid])		//�������߶ȴ��ڵ���߶ȣ�����½�
					{
						dir = 1;		
					}
					else if(length[uid] > mlength[mid])	//����߶�С�ڵ���߶ȣ��������
					{	
						dir = 0;
					}
					if(dir == 1){			
						mlength[mid] --;		//����߶�	��1mm;
					}
					else {
						mlength[mid] ++;		//����߶� ��1mm
					}
				}
					MotorId(mid+1,dir);		//ִ�е��ID�������������ID������1~15������� �������dir�����Ƶ������
		}
		
}

void MotorMain(u8 id)		// 0 5 10		����ƶ�������
{
	u8 i = 0;
	for(i = 0;i < 5; i ++)
			{
					MotorMove(i+id*5,i+id*5);
			}
}

void TIM3_IRQHandler(void)   //TIM3�ж�1
{
	u8 b = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{	

			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			if(dataBuf[50] > StartDelayTimes)			//���ʱ�����10��
			{
				
				MotorMain(0);		//��һ������ʼ�ƶ�
			
				if(dataBuf[50] > DelayMotor1Times)		//���ʱ�����11��
				{
					
						MotorMain(1);		//�ڶ�������ʼ�ƶ�
						if(dataBuf[50] > DelayMotor2Times)		//���ʱ�����12��
						{
						
								MotorMain(2);		//�ڶ�������ʼ�ƶ�
						}
				}
			}
			t ++;
			if(t>1200){
					t = 0;
				for(b = 0;b < 15;b++){
					printf(" id < %d >  length <%d> mlength < %d >  delay_times <%d>\r\n  ",b,length[b],mlength[b],dataBuf[50]);	//���ڴ�ӡ��Ϣ
				}
				printf(".....times out....\r\n");
			}
	}
}





/*
25�����壬�������½�1mm��������ֵ�����仯ʱ���仯��ֵ����3mm�����������ʼ�˶�

*/






