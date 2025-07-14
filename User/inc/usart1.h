#ifndef _USART1_H_
#define _USART1_H_

#include "stm32f4xx.h"                  // Device header
#include "stdio.h"
#include "string.h"

#define Max_Size   256

typedef struct {
	u8 data[Max_Size];
	u16 index;
	u8 flag;
}USART_INFO;


void USART1_Config(u32 Baud);
void USART1_SendByte(u8 Byte);
u8 USART1_RecvByte(void);
void RECV_USART1(void);


#endif

