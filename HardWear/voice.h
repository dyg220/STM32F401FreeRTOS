#ifndef _VOICE_H_
#define _VOICE_H_

#include "stm32f4xx.h"                  // Device header
#include "systick.h"

#define VOICE_DATA_H	GPIO_SetBits(GPIOC,GPIO_Pin_5)
#define VOICE_DATA_L	GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define VOICE_BUSY		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)


void Voice_Init(void);
void Voice_SendCmd(u8 cmd);

#endif

