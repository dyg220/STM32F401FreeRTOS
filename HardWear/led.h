#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"                  // Device header


#define LEDR_ON   GPIO_ResetBits(GPIOB,GPIO_Pin_8);
#define LEDR_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_8);
#define LEDB_ON   GPIO_ResetBits(GPIOB,GPIO_Pin_9);
#define LEDB_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_9);

void LED_Init(void);

#endif

