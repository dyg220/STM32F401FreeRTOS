#ifndef _LOCK_H_
#define _LOCK_H_

#include "stm32f4xx.h"                  // Device header
#include "systick.h"

#define IB_H	GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define IB_L	GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define IA_H	GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define IA_L	GPIO_ResetBits(GPIOB,GPIO_Pin_4)


void Door_Init(void);
void Door_Open(void);
void Door_Close(void);
void Door_Start(u16 nms);

#endif

