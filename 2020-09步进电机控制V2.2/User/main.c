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


void SysInitTest(void)
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
					while(MRST1 == 0)
					{
						MotorId(mid[0],0);
						delay_ms(5);
					}
					mid[0]++;
					printf("mid<0>[%d] \r\n",mid[0]);
					if(mid[0] > 5)
					{
						mid[0] = 0;
						printf("���1���ʼ�����... \r\n");
					}
				}
			}
			if(mid[1] != 0)
			{
				if(MRST2 == 0)
				{
					delay_ms(10);
					while(MRST2 == 0)
					{
						MotorId(mid[1],0);
						delay_ms(5);
					}
					mid[1]++;
					printf("mid<1>[%d] \r\n",mid[1]);
					if(mid[1] > 10)
					{
						mid[1] = 0;
						printf("���2���ʼ�����... \r\n");
					}
				}
			}
			if(mid[2] != 0)
			{
				if(MRST3 == 0)
				{
					delay_ms(10);
					while(MRST3 == 0)
					{
						MotorId(mid[2],0);
						delay_ms(5);
					}
					mid[2]++;
					printf("mid<2>[%d] \r\n",mid[2]);
					if(mid[2] > 15)
					{
						mid[2] = 0;
						printf("���3���ʼ�����... \r\n");
					}

				}
			}
			if((mid[0]==0)&&(mid[1]==0)&&(mid[2]==0)){
				printf("System Inti OK... \r\n");
				break;
			}
	}
}


int main(void)
{
	u8 temp = 0;
	u8 m_time = 0;
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	
	delay_ms(100);
	PWM_GPIO_Init();		//PWM������ų�ʼ��
	OLED_Init();
	OLED_Clear();
	delay_ms(400);
	//OLED_ShowString(63,6,"CODE:",8);
		OLED_ShowString(0,4,"System Init...  ",8);

	uart_init(115200);	 //����1��ʼ��Ϊ115200
	LaserInit(115200);			//����3��ʼ��Ϊ115200
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	
	
	
	
	 KEY_Init();

	
	delay_times = 0;
	
	//SysInit();
	printf("system init .....\r\n");
		SysInitTest();			//�����ʼ��
	IWDG_Init(6,625);
	getLength();
//	while(1)
//	{
//		for(id = 0; id < 16; id++)
//		{
//			MotorId(id,1);
//		}
//		delay_ms(400);
//		for(id = 0; id < 16; id++)
//		{
//			MotorId(id,0);
//		}
//		delay_ms(400);
//	}
	EXTIX_Init();		//�ⲿ�жϳ�ʼ��
		TIM3_Int_Init(999,59);//72000/44/1000 = 400Hz����Ƶ��
	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx����
	OLED_ShowString(0,4,"System Init OK! ",8);
	while(1)
	{
		m_time++;
		IWDG_Feed();
					delay_ms(10);
		if(m_time > 20)
		{
			m_time = 0;
			LED1=!LED1;			//LED�͵�ƽ����
			temp++;
			getLength();
		}
		if(mode == 0)
		{
			temp = 0;
			TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx����
		}
		if(temp > 4)
		{
			temp = 0;
			if(mode !=0)
			{
			//	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx����
				getLength(); 
			//	TIM_Cmd(TIM3, ENABLE);
			}
						//
		}
	}	
	
}
