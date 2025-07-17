#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f4xx.h"                  // Device header


/***********SPI2*********/
void SPI2_Init(void);
u8 SPI2_Transfer(u8 Data);


/***********SPI3*********/
void SPI3_Init(void);
u8 SPI3_Transfer(u8 Data);

#endif

