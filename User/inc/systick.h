#ifndef __SYSTICK_H
#define __SYSTICK_H
#include "stm32f4xx.h"
void delay_us(u32 us);
void delay_ms(u32 ms);
void Delay_Ms(u32 ms);
void delay_init(u8 SYSCLK);
void delay_xms(u32 nms);
#define SYSTEM_SUPPORT_OS 1
#endif
