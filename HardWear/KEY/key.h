
#ifndef _DELAY_H
#define _DELAY_H
#include "stm32f4xx.h"

enum KEY {
	key1_click = 1,
	key1_long
};


#define Key1_Press !(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))

#define Key_Mode     0  //1代表非阻塞   0代表有阻塞


void Key_Init(void);
u8 Key_Scan(void);
u8 Key_GetState(void);
void Key_Tick(void);

#endif
