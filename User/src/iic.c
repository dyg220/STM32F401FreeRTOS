#include "iic.h"

void IIC3_Init(void)
{
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // SCL:PB6, SDA:PB7
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;       // 50%占空比
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;                // 主机地址（可忽略）
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;              // 使能ACK
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 100000;               // 100kHz
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);                                // 使能I2C



}