#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f4xx.h"                  // Device header

void SPI3_Init(void);
u8 SPI3_Transfer(u8 data);

#endif 
