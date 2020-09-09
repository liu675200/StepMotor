#include "laser.h"
#include "oled_display.h"
#include "stdio.h"

#include "timer.h"

u8 Num = 0;			//接收数据长度标记
u8 USART3_RX_STA[20] = { 0 };		//串口接收数据缓冲区
u8 i;

//u8 Data[20];
u16 Distance = 0;			//单次接收到的数据缓冲区
u16 dataBuf[51] = {0};	//10秒内接收到的数据缓冲区	
u16 Data[5] = {0};		//每次接收到的数据缓冲区
u16 length[15] = {0};		//15组测得高度缓冲区
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
	
	PutChar(0x57);		//发送第一条指令  0x57
	delay_ms(2);		//延时2ms
	PutChar(ID);			//发送第二条指令  激光传感器的id
	delay_ms(2);		//延时2ms
	while(1)
	{
			if(USART3_RX_STA[0] != 0x75)	//如果串口接收到的第一个数据是0x75，那么说明接收到了数据，否则重新接收
			{ 
					Num = 0;		//对数据长度清零
			}
			if(Num == 8)		//如果接收到了8个数据，说明接收完成，对数据num清零，对接收到的数据进行处理
			{
					Num = 0;			//对数据长度清零
					if(USART3_RX_STA[7] ==0x07)		//判断是否接收到了结束字符0x07，如果接收完成，将接收到的数据进行处理，否则跳出循环
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
	if(Distance>1000)		//如果数据过大，则对数据赋值1000（错误数据）
	{
		Distance = 1000;
	}
	return Distance;	//返回接收到的数据
	
}

void ToRead(u16 data[],u16 len[],u8 num)		//读取一组数据，一组里有5个数据  data <- len    num->数据偏移地址
{
		u8 i;
		for(i = 0; i < 5; i++)
		{
				data[i+num] = len[i];		
		//	printf("%d  %d \r\n",i,num);
 		}
}

void DataDelay(u16 data[])		//开机读数据延时10秒
{
		u8 j,k;
		if(0 == dataBuf[49])		//判断是否接收到10组数据		最后一组数据是否有效
		{
			dataBuf[50] = 0;
		}
		else{		//如果接收完成，dataBuf[50] 进行标记
				if(dataBuf[50] < 20){
					dataBuf[50]++;
			}
		}
		if(0 == dataBuf[50])		//如果没有接收完10组数据
		{
			ToRead(dataBuf,Data,i);		//按顺序读一组数据
			i+=5;
		}
		else{		//否则，第二组的数据赋值到第一组，依次类推，最后一组的数据存放实时数据
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
					if(dataBuf[50] > 2)	//如果时间大于12秒，将第二组电机的数据赋值到第三组
					{
							for(k = 0;k < 5; k++)
							{
									length[k+10] = length[k+5];
							}
					}
					if(dataBuf[50] > 1)		//如果时间大于11秒，将第一组电机的数据赋值到第二组
					{
							for(k = 0;k < 5; k++)
							{
									length[k+5] = length[k];
							}
					}
					for(k = 0; k < 5;k++)//如果时间大于10秒，将第一组数据缓冲区的数据赋值到第一组
					{
							length[k] = dataBuf[k];
					}
			}
		}
	
}

void getLength(void)		//一次获取五组激光传感器的数据
{
	u8 i,j;
	u16 abs,high,val,len,maxlen,minlen;
	for(j = 0; j < 5;j++){
		val  = 0;
		for(i = 0; i < 7; i++){		//求四次的平均长度
			len = LaserReadData(0x01+j);  //获取数据
			if(len>maxlen)		//取最大值
			{
				maxlen = len;
			}
			if(len < minlen)		//取最小值
			{
				minlen = len;
			}
			val += len;
		}
		val = val - minlen - maxlen;		//去掉最大值与最小值
		
		
		val = val/5 - 100;		//得到平均值，高度和为35， 要想得到高度和为25 ， 则对数据减 100 单位 mm
		
		
		if(val>500)		//如果数据过大，则数据等于500
		{
			val = 500;
		}
		
		len = val+100;		//测得高度		
		high = 350 - len;	//物体高度

		if(Data[j] > val)		//对数据作差，判断数据是否变化   这次数据小于上一次数据
		{
			abs = Data[j] - val;
		}
		else if(Data[j] < val)
		{
			abs = val - Data[j];
		}
		else{
			abs = 0;		//数据无变化
		}
		if(abs > 2){		//如果数据变化大于2mm则将数据读取			
			Data[j] = val;
		}
		printf("id<%d> 测得高度 <%d> 物体高度 <%d> 电机移动高度 <%d> 实测高度 <%d> \r\n",j+1,len,high,Data[j], val);
		len = 0;

	}
	

	DataDelay(Data);		//数据延时
	
	printf("------end--------\r\n");
}

void USART3_IRQHandler(void)		//串口三中断
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		USART3_RX_STA[Num++] =USART_ReceiveData(USART3);	//读取接收到的数据
	} 	
}


