#ifndef _MG200_H_
#define _MG200_H_
#include "stm32f4xx.h"
#include "systick.h"

#define MG200_PWE_ON     (GPIO_SetBits(GPIOC,GPIO_Pin_1))
#define MG200_PWE_OFF    (GPIO_ResetBits(GPIOC,GPIO_Pin_1))
#define MG200_DETECT     (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))

void USART6_Config(u32 Baud);


#endif
