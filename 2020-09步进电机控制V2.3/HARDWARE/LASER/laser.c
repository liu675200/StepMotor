#include "laser.h"
#include "oled_display.h"
#include "stdio.h"

#include "timer.h"

u8 Num = 0;			//�������ݳ��ȱ��
u8 USART3_RX_STA[20] = { 0 };		//���ڽ������ݻ�����
u8 i;

//u8 Data[20];
u16 Distance = 0;			//���ν��յ������ݻ�����
u16 dataBuf[51] = {0};	//10���ڽ��յ������ݻ�����	
u16 Data[5] = {0};		//ÿ�ν��յ������ݻ�����
u16 length[15] = {0};		//15���ø߶Ȼ�����
extern u16 delay_times;

void LaserInit(u32 baudRate)	//���⴮�ڳ�ʼ��
{
	NVIC_InitTypeDef 	NVIC_InitStruct;//����һ�������жϵĽṹ��	
	USART_InitTypeDef USART_InitSturct;//���崮��1�ĳ�ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;//���崮�ڶ�Ӧ�ܽŵĽṹ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);//�򿪴��ڹܽ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	//USART3_Tx_Pin Configure 	B10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//��������ٶ�50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
	GPIO_Init(GPIOB , &GPIO_InitStruct);//����ʼ���õĽṹ��װ��Ĵ���

//USART3_Rx_Pin Configure  B11
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIOģʽ��������
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//��������
  GPIO_Init(GPIOB, &GPIO_InitStruct);//����ʼ���õĽṹ��װ��Ĵ���

//USART3 Configure	
	USART_InitSturct.USART_BaudRate = baudRate;//������115200
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;//���ݿ��8λ
	USART_InitSturct.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitSturct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��������
	USART_Init(USART3 , &USART_InitSturct);//����ʼ���õĽṹ��װ��Ĵ���	
	//USART1_INT Configure
	USART_ITConfig(USART3 , USART_IT_RXNE , ENABLE);//ʹ�ܽ����ж�
//	USART_ITConfig(USART3 , USART_IT_TXE , ENABLE);
	USART_Cmd(USART3 , ENABLE);//�򿪴���
	USART_ClearFlag(USART3 , USART_FLAG_TC);//�����һ�����ݷ���ʧ�ܵ�����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//�򿪸��ж�
	NVIC_Init(&NVIC_InitStruct);
	
	dataBuf[25] = 0;
}

void PutChar(u8 Data)		//����һ���ַ�
{
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART3 , Data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);//�ȴ��������
}

u16 LaserReadData(u8 ID)		//��ȡ���⴫�����ľ���
{
	u8 i = 0;
	
	PutChar(0x57);		//���͵�һ��ָ��  0x57
	delay_ms(2);		//��ʱ2ms
	PutChar(ID);			//���͵ڶ���ָ��  ���⴫������id
	delay_ms(2);		//��ʱ2ms
	while(1)
	{
			if(USART3_RX_STA[0] != 0x75)	//������ڽ��յ��ĵ�һ��������0x75����ô˵�����յ������ݣ��������½���
			{ 
					Num = 0;		//�����ݳ�������
			}
			if(Num == 8)		//������յ���8�����ݣ�˵��������ɣ�������num���㣬�Խ��յ������ݽ��д���
			{
					Num = 0;			//�����ݳ�������
					if(USART3_RX_STA[7] ==0x07)		//�ж��Ƿ���յ��˽����ַ�0x07�����������ɣ������յ������ݽ��д�����������ѭ��
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
	if(Distance>1000)		//������ݹ���������ݸ�ֵ1000���������ݣ�
	{
		Distance = 1000;
	}
	return Distance;	//���ؽ��յ�������
	
}

void ToRead(u16 data[],u16 len[],u8 num)		//��ȡһ�����ݣ�һ������5������  data <- len    num->����ƫ�Ƶ�ַ
{
		u8 i;
		for(i = 0; i < 5; i++)
		{
				data[i+num] = len[i];		
		//	printf("%d  %d \r\n",i,num);
 		}
}

void DataDelay(u16 data[])		//������������ʱ10��
{
		u8 j,k;
		if(0 == dataBuf[49])		//�ж��Ƿ���յ�10������		���һ�������Ƿ���Ч
		{
			dataBuf[50] = 0;
		}
		else{		//���������ɣ�dataBuf[50] ���б��
				if(dataBuf[50] < 20){
					dataBuf[50]++;
			}
		}
		if(0 == dataBuf[50])		//���û�н�����10������
		{
			ToRead(dataBuf,Data,i);		//��˳���һ������
			i+=5;
		}
		else{		//���򣬵ڶ�������ݸ�ֵ����һ�飬�������ƣ����һ������ݴ��ʵʱ����
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
					if(dataBuf[50] > 2)	//���ʱ�����12�룬���ڶ����������ݸ�ֵ��������
					{
							for(k = 0;k < 5; k++)
							{
									length[k+10] = length[k+5];
							}
					}
					if(dataBuf[50] > 1)		//���ʱ�����11�룬����һ���������ݸ�ֵ���ڶ���
					{
							for(k = 0;k < 5; k++)
							{
									length[k+5] = length[k];
							}
					}
					for(k = 0; k < 5;k++)//���ʱ�����10�룬����һ�����ݻ����������ݸ�ֵ����һ��
					{
							length[k] = dataBuf[k];
					}
			}
		}
	
}

void getLength(void)		//һ�λ�ȡ���鼤�⴫����������
{
	u8 i,j;
	u16 abs,high,val,len,maxlen,minlen;
	for(j = 0; j < 5;j++){
		val  = 0;
		for(i = 0; i < 7; i++){		//���Ĵε�ƽ������
			len = LaserReadData(0x01+j);  //��ȡ����
			if(len>maxlen)		//ȡ���ֵ
			{
				maxlen = len;
			}
			if(len < minlen)		//ȡ��Сֵ
			{
				minlen = len;
			}
			val += len;
		}
		val = val - minlen - maxlen;		//ȥ�����ֵ����Сֵ
		
		
		val = val/5 - 100;		//�õ�ƽ��ֵ���߶Ⱥ�Ϊ35�� Ҫ��õ��߶Ⱥ�Ϊ25 �� ������ݼ� 100 ��λ mm
		
		
		if(val>500)		//������ݹ��������ݵ���500
		{
			val = 500;
		}
		
		len = val+100;		//��ø߶�		
		high = 350 - len;	//����߶�

		if(Data[j] > val)		//����������ж������Ƿ�仯   �������С����һ������
		{
			abs = Data[j] - val;
		}
		else if(Data[j] < val)
		{
			abs = val - Data[j];
		}
		else{
			abs = 0;		//�����ޱ仯
		}
		if(abs > 2){		//������ݱ仯����2mm�����ݶ�ȡ			
			Data[j] = val;
		}
		printf("id<%d> ��ø߶� <%d> ����߶� <%d> ����ƶ��߶� <%d> ʵ��߶� <%d> \r\n",j+1,len,high,Data[j], val);
		len = 0;

	}
	

	DataDelay(Data);		//������ʱ
	
	printf("------end--------\r\n");
}

void USART3_IRQHandler(void)		//�������ж�
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		USART3_RX_STA[Num++] =USART_ReceiveData(USART3);	//��ȡ���յ�������
	} 	
}


