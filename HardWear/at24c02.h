#ifndef _AT24C02_H_
#define _AT24C02_H_

#include "stm32f4xx.h"                  // Device header
#include "systick.h"
#include "iic.h"
#include "stdio.h"


#define Addr_Write  0xA0
#define Addr_Read   0xA1

void AT24C02_Init(void);
u8 AT24C02_WriteByte(u8 addr,u8 data);
u8 AT24C02_ReadByte(u8 addr,u8*data);
u8 AT24C02_ReadBytes(u8 addr,u16 len,u8*buff);
u8 AT24C02_WritePage(u8 addr,u8 len,u8 *buff);
u8 AT24C02_WritePages(u8 addr,u16 len,u8 *buff);


#endif

