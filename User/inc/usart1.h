
#ifndef __USART1_H__
#define __USART1_H__ // 条件编译，避免重复定义

#include "stm32f4xx.h"                  // Device header
#include <stdio.h>

#define Max_Size   256

typedef struct {
	u8 data[Max_Size];
	u16 index;
	u8 flag;
}USART1_INFO;



void USART1_Config(u32 Baud);
void USART1_SendByte(u8 Byte);
void USART1_SendStr(u8* str);
void RECV_USART1(void);

#endif
