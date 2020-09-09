#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "led.h"
#include "timer.h"
#include "exti.h" 
#include "key.h"
#include "laser.h"
#include "oled_display.h"
#include "wdg.h"

extern u8 mode;
	
extern u16 delay_times;


void SysInitTest(void)			//三组电机开机初始化
{
	u8 mid[3];
	u8 n = 0;
	mid[0] = 1;
	mid[1] = 6;
	mid[2] = 11;
	while(1)
	{
			for(n = 0; n < 3; n ++)
			{
				MotorId(mid[n],1);
				delay_ms(1);
			}
			if(mid[0] != 0){
				if(MRST1 == 0)
				{
					delay_ms(10);
					while(MRST1 == 0)	//如果第一组电机复位成功，则上升到没有复位的位置
					{
						MotorId(mid[0],0);
						delay_ms(5);
					}
					mid[0]++;
					printf("mid<0>[%d] \r\n",mid[0]);
					if(mid[0] > 5)		//第一组全部电机初始化完成
					{
						mid[0] = 0;
						printf("电机1组初始化完成... \r\n");
					}
				}
			}
			if(mid[1] != 0)
			{
				if(MRST2 == 0)
				{
					delay_ms(10);
					while(MRST2 == 0)	//如果第二组电机复位成功，则上升到没有复位的位置
					{
						MotorId(mid[1],0);
						delay_ms(5);
					}
					mid[1]++;
					printf("mid<1>[%d] \r\n",mid[1]);
					if(mid[1] > 10)	//第二组全部电机初始化完成
					{
						mid[1] = 0;
						printf("电机2组初始化完成... \r\n");
					}
				}
			}
			if(mid[2] != 0)
			{
				if(MRST3 == 0)
				{
					delay_ms(10);
					while(MRST3 == 0)//如果第三组电机复位成功，则上升到没有复位的位置
					{
						MotorId(mid[2],0);
						delay_ms(5);
					}
					mid[2]++;
					printf("mid<2>[%d] \r\n",mid[2]);
					if(mid[2] > 15)	//第三组全部电机初始化完成
					{
						mid[2] = 0;
						printf("电机3组初始化完成... \r\n");
					}

				}
			}
			if((mid[0]==0)&&(mid[1]==0)&&(mid[2]==0)){		//如果全部电机初始化完成，则结束初始化程序，进入控制程序
				printf("System Inti OK... \r\n");
				break;
			}
	}
}


int main(void)
{
	u8 temp = 0;
	u8 m_time = 0;
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	
	delay_ms(100);
	PWM_GPIO_Init();		//PWM输出引脚初始化
	OLED_Init();				//OLED初始化
	OLED_Clear();				//OLED清屏
	delay_ms(400);
	OLED_ShowString(0,4,"System Init...  ",8);	//提示系统正在初始化

	uart_init(115200);	 //串口1初始化为115200
	LaserInit(115200);			//串口3初始化为115200
	LED_Init();		  	//初始化与LED连接的硬件接口
	
	
	
	
	 KEY_Init();			//轻触开关初始化，后面电机初始化

	
	delay_times = 0;
	
	printf("system init .....\r\n");
		SysInitTest();			//电机初始化
	IWDG_Init(6,625);			//开启独立看门狗，避免程序卡死
	getLength();					//得到一组数据

	EXTIX_Init();		//外部中断初始化
		TIM3_Int_Init(999,59);//72000/44/1000 = 400Hz计数频率
	TIM_Cmd(TIM3, DISABLE);  //使能TIMx外设
	OLED_ShowString(0,4,"System Init OK! ",8);	//提示初始化完成
	while(1)
	{
		m_time++;
		IWDG_Feed();		//进行喂狗操作，如果长时间没有喂狗操作，则说明程序卡死，系统将自动复位
					delay_ms(10);
		if(m_time > 20)			//如果时间大于200ms，则进行一次数据读取（读取数据会占用CPU的一些时间），LED1闪烁，
		{
			m_time = 0;
			LED1=!LED1;			//LED低电平点亮
			temp++;
			getLength();
		}
		if(mode == 0)		//如果未开始，则失能定时器3
		{
			temp = 0;
			TIM_Cmd(TIM3, DISABLE);  //使能TIMx外设
		}
		if(temp > 4)		//如果时间大于800ms，则进行一次数据读取
		{
			temp = 0;
			if(mode !=0)
			{
			//	TIM_Cmd(TIM3, DISABLE);  //使能TIMx外设
				getLength(); 
			//	TIM_Cmd(TIM3, ENABLE);	
			}
		}
	}	
	
}
