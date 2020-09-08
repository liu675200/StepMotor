#include "laser.h"
#include "oled_display.h"
#include "stdio.h"

#include "timer.h"

u8 Num = 0;
u8 USART3_RX_STA[20] = { 0 };
u8 i;

//u8 Data[20];
u16 Distance = 0;
u16 dataBuf[51] = {0};
u16 Data[5] = {0};
u16 length[15] = {0};
extern u16 delay_times;

void LaserInit(u32 baudRate)	//激光串口初始化
{
	NVIC_InitTypeDef 	NVIC_InitStruct;//定义一个设置中断的结构体	
	USART_InitTypeDef USART_InitSturct;//定义串口1的初始化结构体
	GPIO_InitTypeDef GPIO_InitStruct;//定义串口对应管脚的结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);//打开串口管脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	//USART3_Tx_Pin Configure 	B10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//输出引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//设置最高速度50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//推挽复用输出
	GPIO_Init(GPIOB , &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3_Rx_Pin Configure  B11
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO模式悬浮输入
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//输入引脚
  GPIO_Init(GPIOB, &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3 Configure	
	USART_InitSturct.USART_BaudRate = baudRate;//波特率115200
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;//数据宽度8位
	USART_InitSturct.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitSturct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送与接收
	USART_Init(USART3 , &USART_InitSturct);//将初始化好的结构体装入寄存器	
	//USART1_INT Configure
	USART_ITConfig(USART3 , USART_IT_RXNE , ENABLE);//使能接收中断
//	USART_ITConfig(USART3 , USART_IT_TXE , ENABLE);
	USART_Cmd(USART3 , ENABLE);//打开串口
	USART_ClearFlag(USART3 , USART_FLAG_TC);//解决第一个数据发送失败的问题
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//打开该中断
	NVIC_Init(&NVIC_InitStruct);
	
	dataBuf[25] = 0;
}

void PutChar(u8 Data)		//发送一个字符
{
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART3 , Data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);//等待发送完毕
}

u16 LaserReadData(u8 ID)		//读取激光传感器的距离
{
	u8 i = 0;
	
	PutChar(0x57);
	delay_ms(2);
	PutChar(ID);
	delay_ms(2);
	while(1)
	{
			if(USART3_RX_STA[0] != 0x75)
			{ 
					Num = 0;
			}
			if(Num == 8)
			{
					Num = 0;
					if(USART3_RX_STA[7] ==0x07)
					{
							Distance = USART3_RX_STA[1]<<8|USART3_RX_STA[2];
//							Distance = Distance*255 + USART3_RX_STA[2];
						break;
					}
					break;
			}
			else
			{
					i++;
					delay_ms(2);
				if(i > 10)
				{
					Num = 0;break;
				}
			}
	}
	if(Distance>1000)
	{
		Distance = 1000;
	}
	return Distance;
	
}

void ToRead(u16 data[],u16 len[],u8 num)
{
		u8 i;
		for(i = 0; i < 5; i++)
		{
				data[i+num] = len[i];
		//	printf("%d  %d \r\n",i,num);
 		}
}

void DataDelay(u16 data[])
{
		u8 j,k;
		if(0 == dataBuf[49])
		{
			dataBuf[50] = 0;
		}
		else{
				if(dataBuf[50] < 20){
					dataBuf[50]++;
			}
		}
		if(0 == dataBuf[50])
		{
			ToRead(dataBuf,Data,i);
			i+=5;
		}
		else{
			for(j = 0; j < 9;j++)
			{	
				for(k = 0; k < 5; k++)
				{
					dataBuf[j*5+k] = dataBuf[k+j*5+5];
				}
			}
			ToRead(dataBuf,Data,45);
		
			if(dataBuf[50] > 0 && dataBuf[50] < 5)
			{
					if(dataBuf[50] > 2)
					{
							for(k = 0;k < 5; k++)
							{
									length[k+10] = length[k+5];
							}
					}
					if(dataBuf[50] > 1)
					{
							for(k = 0;k < 5; k++)
							{
									length[k+5] = length[k];
							}
					}
					for(k = 0; k < 5;k++)
					{
							length[k] = dataBuf[k];
					}
			}
		}
//		for(j = 0; j < 10;j++)
//		{
//			for(k = 0; k < 5; k++){
//				printf("dataBUf %d  ->",dataBuf[j*5+k]);
//			//	delay_ms(20);
//			}
//				printf("delay_times %d \r\n",dataBuf[50]);
//		}
	
}

void getLength(void)
{
	u8 i,j;
	u16 len,high,val,abs,maxlen = 0, minlen = 1000;
//	if(delay_times>DelayMotor1Times)
//	{
//			for(i = 0; i < 5;i ++)
//			{
//					length[i+5] = length[i];
//			}
//			if(delay_times>DelayMotor2Times)
//			{
//					for(i = 0; i < 5;i ++)
//					{
//							length[i+10] = length[i+5];
//					}
//			}
//	}
	for(j = 0; j < 5;j++){
		val  = 0;
		for(i = 0; i < 7; i++){		//求四次的平均长度
			len = LaserReadData(0x01+j);
			//printf("len is %d \r\n ",len);
//			delay_ms(5);
			if(len>maxlen)
			{
				maxlen = len;
			}
			if(len < minlen)
			{
				minlen = len;
			}
			val += len;
		}
		val = val - minlen - maxlen;
		
		
		val = val/5 - 100;
		
		if(val>500)
		{
			val = 500;
		}
		
		len = val+100;		//测得高度
		high = 350 - len;	//物体高度

		if(Data[j] > val)
		{
			abs = Data[j] - val;
		}
		else if(Data[j] < val)
		{
			abs = val - Data[j];
		}
		else{
			abs = 0;
		}
		if(abs > 3){
			Data[j] = val;
		}
			
		printf("id<%d> 测得高度 <%d> 物体高度 <%d> 电机移动高度 <%d> 实测高度 <%d> \r\n",j+1,len,high,Data[j], val);
		len = 0;

	}
	

	DataDelay(Data);
	
	printf("------end--------\r\n");

//	OLED_Clear();
//	for(j = 0; j < 4; j++)
	//{
//		memset(Data,0,20);
	//	sprintf(Data,"%d > %d",j+1,length[j]);
//delay_ms(1);
	//	OLED_ShowString(0,j*2,Data,8);
//	}

 
//	for(j = 0; j < 4; j++)
//	{
//		memset(Data,0,20);
	//	sprintf(Data,"%d > %d  ",j+1,length[j]);
//		OLED_ShowString(0,j*2,Data,8);
//	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		USART3_RX_STA[Num++] =USART_ReceiveData(USART3);	//读取接收到的数据
	} 	
}


