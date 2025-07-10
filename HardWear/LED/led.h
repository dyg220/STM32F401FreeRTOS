
#ifndef _LED_H_
#define _LED_H_
#include "stm32f4xx.h"

#define LED1_ON  GPIO_ResetBits(GPIOB, GPIO_Pin_8); 
#define LED1_OFF  GPIO_SetBits(GPIOB, GPIO_Pin_8); 

#define LED2_ON  GPIO_ResetBits(GPIOB, GPIO_Pin_9); 
#define LED2_OFF  GPIO_SetBits(GPIOB, GPIO_Pin_9); 


void LED_Init(void);
void LEDALL_ON(void);
void LEDALL_OFF(void);

void LED1_FZ(void);
void LED2_FZ(void);


#endif // !_LED_H_

