#include "timer.h"
#include "led.h"
#include "usart.h"
#include "laser.h"
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
u16 t = 0;
u16 times[16] = {0};			//电机脉冲计数缓冲
u16 delay_times = 0;
u16 mlength[16]={0};  //当前的高度


extern u16 length[15];		//测得高度
extern u16 dataBuf[51];		//数据缓冲
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}




void PWM_GPIO_Init(void)		//接口初始化
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				 //端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化
 GPIO_ResetBits(GPIOA,GPIO_Pin_All);						 //输出高
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				 //端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化
 GPIO_ResetBits(GPIOC,GPIO_Pin_All);						 //输出高	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;				 //端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化
 GPIO_ResetBits(GPIOB,GPIO_Pin_All);						 //输出低
 

}

void MotorId(u8 id,u8 dir)		//传入两个参数，一个电机ID，一个电机方向dir，单独控制15个电机的速度和方向   id 1~15,  dir 0 1		
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
void MotorMove(u8 uid,u8 mid)	//步进电机移动函数   id 每组五个步进电机id	    mid 单个电机id
{
	static u8 dir = 0;
	if(length[uid] > 245)		//如果高度小于5mm，则无动作
	{
		return;
	}
	if(length[uid] != mlength[mid]){		//如果电机当前高度与物体高度不相同
			
			times[mid]++;				//发出一次脉冲，时间加1
				if(times[mid]>50)		//250个脉冲，上升或下降10mm
				{
					times[mid] = 0;			//对时间缓冲区清空
					if(length[uid] < mlength[mid])		//如果物体高度大于电机高度，电机下降
					{
						dir = 1;		
					}
					else if(length[uid] > mlength[mid])	//物体高度小于电机高度，电机上升
					{	
						dir = 0;
					}
					if(dir == 1){			
						mlength[mid] --;		//电机高度	减1mm;
					}
					else {
						mlength[mid] ++;		//电机高度 加1mm
					}
				}
					MotorId(mid+1,dir);		//执行电机ID函数，传入参数ID，控制1~15个电机， 传入参数dir，控制电机方向
		}
		
}

void MotorMain(u8 id)		// 0 5 10		电机移动主函数
{
	u8 i = 0;
	for(i = 0;i < 5; i ++)
			{
					MotorMove(i+id*5,i+id*5);
			}
}

void TIM3_IRQHandler(void)   //TIM3中断1
{
	u8 b = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{	

			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
			if(dataBuf[50] > StartDelayTimes)			//如果时间大于10秒
			{
				
				MotorMain(0);		//第一组电机开始移动
			
				if(dataBuf[50] > DelayMotor1Times)		//如果时间大于11秒
				{
					
						MotorMain(1);		//第二组电机开始移动
						if(dataBuf[50] > DelayMotor2Times)		//如果时间大于12秒
						{
						
								MotorMain(2);		//第二组电机开始移动
						}
				}
			}
			t ++;
			if(t>1200){
					t = 0;
				for(b = 0;b < 15;b++){
					printf(" id < %d >  length <%d> mlength < %d >  delay_times <%d>\r\n  ",b,length[b],mlength[b],dataBuf[50]);	//串口打印信息
				}
				printf(".....times out....\r\n");
			}
	}
}





/*
25个脉冲，上升或下降1mm，当距离值发生变化时，变化的值大于3mm，步进电机开始运动

*/






