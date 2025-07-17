#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f4xx.h"                  // Device header
#include "systick.h"

#define Key1_Press  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define Key2_Press  !(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))

void Key_Init(void);
uint8_t Key_Scan(void);

#endif

