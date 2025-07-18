#ifndef _W25Q64_H_
#define _W25Q64_H_

#include "stm32f4xx.h"                  // Device header
#include "spi.h"
#include "systick.h"
#include "stdio.h"
#include "key.h"

#define W25Q64_CS_H	 GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define W25Q64_CS_L	 GPIO_ResetBits(GPIOA,GPIO_Pin_4)


void W25Q64_Init(void);
void Write_Enable(void);
u8 Read_Status(void);
void Write_Status(u8 data);
void W25Q64_Read_Bytes(u32 addr,u32 len, u8*buf);
void W25Q64_Write_Page(u32 addr,u32 len, u8*buf);
void W25Q64_Sector_Erase(u32 addr);
void W25Q64_Write_Pages(u32 addr,u32 len, u8*buf);
void W25Q64_block_Erase(u32 addr);
void W25Q64_Erase_Blockn(u32 addr,u8 num);
void W25Q64_Chip_Erase(void);
void W25Q64_Zk_Updata(void);

#endif

