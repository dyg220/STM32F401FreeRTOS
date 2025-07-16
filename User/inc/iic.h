#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f4xx.h"                  // Device header
#include "systick.h"

#define IIC3_SCL_H 	(GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET))
#define IIC3_SCL_L 	(GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET))
#define IIC3_SDA_H 	(GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET))
#define IIC3_SDA_L 	(GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET))
#define IIC3_SDA_R	(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))


#define IIC1_SCL_H 	(GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_SET))
#define IIC1_SCL_L 	(GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET))
#define IIC1_SDA_H 	(GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_SET))
#define IIC1_SDA_L 	(GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET))
#define IIC1_SDA_R	(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))

#define ACK 	 0
#define NO_ACK   1

void IIC3_Init(void);
void IIC3_Start(void);
u8 IIC3_SendByte(u8 Byte);
u8 IIC3_RecvByte(u8 ack);
u8 IIC3_RecvAck(void);
void IIC3_SendAck(u8 ack);
void IIC3_Stop(void);

void IIC1_Init(void);
void IIC1_Start(void);
u8 IIC1_SendByte(u8 Byte);
u8 IIC1_RecvByte(void);
u8 IIC1_RecvAck(void);
void IIC1_SendAck(u8 ack);
void IIC1_Stop(void);

#endif

