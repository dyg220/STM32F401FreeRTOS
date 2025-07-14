#ifndef _MG200_H_
#define _MG200_H_

#include "stm32f4xx.h"                  // Device header
#include "usart1.h"
#include "systick.h"
#include "voice.h"
#include "door.h"

#define MG200_PWE_ON     (GPIO_SetBits(GPIOC,GPIO_Pin_1))
#define MG200_PWE_OFF    (GPIO_ResetBits(GPIOC,GPIO_Pin_1))
#define MG200_DETECT     (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))


void USART6_Config(u32 Baud);
void USART6_SendByte(u8 Byte);
void MG200_Init(void);
u8 USART6_RecvByte(void);
void MG200_Send_Cmd(u8 cmd, u8 parameter);
u8 MG200_Read_Cmd(u8 cmd, u8* parameter, u8* result);

u8 CaptureAndExtract(u8 parameter);//Ö¸ÎÆ²É¼¯


#endif

