#ifndef _CY8CMBR3116_H_
#define _CY8CMBR3116_H_

#include "stm32f4xx.h"                  // Device header
#include "iic.h"
#include "stdio.h"
#include "voice.h"
#include "string.h"
#include "door.h"

typedef struct{
	u8 password[10];//输入缓冲区
	u8 index;//输入位置
}CY_INFO;

#define CY8CMBR3116_ADDR	(0x37<<1)   //器件地址空出读写位

#define CY8CMBR3116_RST_H	(GPIO_SetBits(GPIOA,GPIO_Pin_1))
#define CY8CMBR3116_RST_L	(GPIO_ResetBits(GPIOA,GPIO_Pin_1))

#define BUTTON_STA			0xAA		//按键状态寄存器地址
#define SYSTEM_STATUS		0x8a		//0: 非默认的出厂配置	1: 默认的出厂配置
#define CTRL_CMD 			0x86			
#define CTRL_CMD_STATUS		0x88		//0: 没有任何错误		1: 发生了错误
#define CTRL_CMD_ERR		0x89		
//0: 指令执行成功   0xfd：对闪存写入失败  0xfe：CONFIG_CRC 所存储的配置 CRC 校验和与计算得到的配置 CRC 校验和不一致  0xff：指令无效
#define REGCFG_ADDR			0x00 		//配置寄存器起始


u8 CY8CMBR3116_regcfg(u8 dfg_flag);
void CY8CMBR3116_Init(void);
u8 CY8CMBR3116_Read_Key(void);
u8 CY8CMBR3116_Key_Scan(void);
void CY8CMBR3116_Open_Door(void);

#endif

