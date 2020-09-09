#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 

#define MRST1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键1
#define MRST2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//读取按键2
#define MRST3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//读取按键3

#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//开启与暂停





#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO初始化
//u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
